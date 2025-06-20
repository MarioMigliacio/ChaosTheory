// ============================================================================
//  File        : SandBoxScene.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-14
//  Description : Hosts the definitions for Game Scene Object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SandBoxScene.h"
#include "AssetManager.h"
#include "Assets.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "Macros.h"
#include "MainMenuScene.h"
#include "PauseScene.h"
#include "ResolutionScaleManager.h"
#include "SceneFactory.h"
#include "SceneTransitionManager.h"
#include "UIFactory.h"
#include "UIManager.h"
#include "WindowManager.h"

/// @brief Constants that can be adjusted throughout the SandBoxScene.
namespace
{
/// @brief Fixed name constant for the title of the Sandbox Scene.
constexpr auto TITLE_SCREEN_LABEL = "Sandbox Scene";

/// @brief Fixed name constant for a helpful placeholder to pause the game.
constexpr auto PAUSE_GAME_LABEL = "Press Escape to Pause";

/// @brief Fixed name constant to be used with BindActionKey to setup a pause key.
constexpr auto PAUSE_BUTTON_KEY = "Pause";
} // namespace

/// @brief Constructor for the SandBoxScene.
/// @param settings Internal settings to initialize with.
SandBoxScene::SandBoxScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

/// @brief Initializes the SandBoxScene.
void SandBoxScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    UIManager::Instance().Clear();

    LoadRequiredAssets();
    BindInputKeys();
    LoadBackground();
    SetupSceneComponents();

    SceneTransitionManager::Instance().StartFadeIn();
    m_isInitialized = true;

    CT_LOG_INFO("SandBoxScene initialized.");
}

/// @brief Load any required assets relevant to the SandBoxScene.
void SandBoxScene::LoadRequiredAssets()
{
    for (const auto &[key, path] : SandBoxAssets::Textures)
    {
        if (!AssetManager::Instance().LoadTexture(key, path))
        {
            CT_LOG_ERROR("SandBoxScene failed to load texture asset: {} -> {}", key, path);
        }
    }

    for (const auto &[key, path] : FontAssets::Fonts)
    {
        if (!AssetManager::Instance().LoadFont(key, path))
        {
            CT_LOG_ERROR("SandBoxScene failed to load font asset: {} -> {}", key, path);
        }
    }

    CT_LOG_INFO("SandBoxScene finished LoadRequiredAssets.");
}

/// @brief Shuts down this scene and resets internal state.
void SandBoxScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("SandBoxScene", "Shutdown");

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("SandBoxScene shutdown.");
}

/// @brief Handles the exit criteria for this scene.
void SandBoxScene::OnExit()
{
    InputManager::Instance().UnbindKey(PAUSE_BUTTON_KEY);

    CT_LOG_INFO("SandBoxScene OnExit.");
}

/// @brief Resumes Scene in event of a Pause.
void SandBoxScene::OnResume()
{
    UIManager::Instance().Clear();
    SetupSceneComponents();

    CT_LOG_INFO("SandBoxScene resumed and UI restored.");
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update.
void SandBoxScene::Update(float dt)
{
    const auto mousePos = InputManager::Instance().GetMousePosition();
    const bool isPressed = InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left);
    const bool isJustPressed = InputManager::Instance().IsMouseButtonJustPressed(sf::Mouse::Left);

    UIManager::Instance().Update(mousePos, isPressed, isJustPressed, dt);

    if (m_background)
    {
        m_background->Update(dt);
    }

    CheckActionsPressed();
    UpdateHUD(dt);

    // Handle button scene request change
    if (m_hasPendingTransition)
    {
        if (m_requestedScene == SceneID::Pause)
        {
            CT_LOG_INFO("SandBoxScene Pause Event Requested.");

            m_hasPendingTransition = false;
            SceneManager::Instance().PushScene(std::make_unique<PauseScene>(m_settings));
        }
    }
}

/// @brief Handle any relevent events at this scene level if needed.
/// @param event bubbled down from caller.
void SandBoxScene::HandleEvent(const sf::Event &event)
{
}

/// @brief Not used in SandBoxScene context.
/// @param newSize bubbled down from caller, not needed.
void SandBoxScene::OnResize(const sf::Vector2u &newSize)
{
}

/// @brief While this scene is active, render the necessary components.
void SandBoxScene::Render()
{
    auto &window = WindowManager::Instance().GetWindow();
    window.clear();

    if (m_background)
    {
        m_background->Draw(window);
    }

    UIManager::Instance().Render(window);
}

/// @brief Helper method to initialize necessary Scene components.
void SandBoxScene::SetupSceneComponents()
{
    CreateTitleText();
    PlayGameMusic();
    CreateHUDPanel();
    MockFillableGaugeComponents();
}

/// @brief Helper method to load the Background for this Scene.
void SandBoxScene::LoadBackground()
{
    m_background = std::make_unique<Background>();
    m_background->InitParallax(
        {{BackgroundAssets::GasPattern1BackgroundKey, 2.f}, {BackgroundAssets::PlainStarBackgroundKey, 1.f}});

    m_background->SetLayerMotion(BackgroundAssets::PlainStarBackgroundKey, {0.2f, -0.5f});
    m_background->SetLayerMotion(BackgroundAssets::GasPattern1BackgroundKey, {0.f, -0.1f});
}

/// @brief Helper method to create the Title string entity for this scene.
void SandBoxScene::CreateTitleText()
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const std::string titleLabel = TITLE_SCREEN_LABEL;
    const unsigned int titleFontSize = scaleMgr.ScaleFont(48);
    const sf::Vector2f titlePos = {WindowManager::Instance().GetWindow().getSize().x / 2.f,
                                   scaleMgr.ScaledReferenceY(0.08f)};

    const std::string helpLabel = PAUSE_GAME_LABEL;
    const unsigned int helpFontSize = scaleMgr.ScaleFont(20);
    const sf::Vector2f helpPos = {WindowManager::Instance().GetWindow().getSize().x / 2.f,
                                  scaleMgr.ScaledReferenceY(0.20f)};

    m_titleLabel = UIFactory::Instance().CreateTextLabel(titleLabel, titlePos, titleFontSize, true,
                                                         UITextLabelScheme::MintyHerbScheme);

    m_helpLabel = UIFactory::Instance().CreateTextLabel(helpLabel, helpPos, helpFontSize, true,
                                                        UITextLabelScheme::MintyHerbScheme);

    UIManager::Instance().AddElement(m_titleLabel);
    UIManager::Instance().AddElement(m_helpLabel);
}

/// @brief Initializes the HUD Panel ui component.
void SandBoxScene::CreateHUDPanel()
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    // Relative position and size (top bar)
    const sf::Vector2f relativePos{0.f, 0.f};
    const sf::Vector2f relativeSize{1.0f, 0.05f}; // Full width, 8% height

    auto hudPanel = UIFactory::Instance().CreateHUDPanel(relativePos, relativeSize);
    hudPanel->SetInternalPadding(scaleMgr.ScaledReferenceY(0.15f)); // Space between labels
    hudPanel->SetEdgePadding(scaleMgr.ScaledReferenceY(0.01f));     // Padding around edges
    hudPanel->SetLayoutMode(LayoutMode::Horizontal);
    hudPanel->SetCenterChildren(false);

    const unsigned int fontSize = ResolutionScaleManager::Instance().ScaleFont(18);
    const sf::Vector2f gaugeRelativeSize = {0.2f, 0.015f}; // Width, Height in screen %

    m_scoreLabel = UIFactory::Instance().CreateTextLabel(HUD_SCORE_LABEL_INIT_STR, {0.f, 0.f}, fontSize, false);
    m_timerLabel = UIFactory::Instance().CreateTextLabel(HUD_TIMER_LABEL_INIT_STR, {0.f, 0.f}, fontSize, false);

    FillableGaugeConfig gaugeCfg;
    gaugeCfg.relativePosition = {0.f, 0.f}; // handled by HUD
    gaugeCfg.relativeSize = {0.2f, 0.015f};
    gaugeCfg.colorScheme = GaugeColorScheme::Health;
    gaugeCfg.borderThickness = DEFAULT_GAUGE_BORDER_THICKNESS;
    gaugeCfg.borderColor = DEFAULT_GAUGE_BORDER_COLOR;
    gaugeCfg.showPercentage = true;
    gaugeCfg.initialValue = 1.f;
    gaugeCfg.showTitle = true;
    gaugeCfg.titleText = HUD_HEALTH_TAG;

    m_healthGauge = UIFactory::Instance().CreateFillableGauge(gaugeCfg);

    hudPanel->AddElement(m_healthGauge, HUDSlotAlignment::Left);
    hudPanel->AddElement(m_timerLabel, HUDSlotAlignment::Right);
    hudPanel->AddElement(m_scoreLabel, HUDSlotAlignment::Right);

    m_scoreLabel->SetText(HUD_SCORE_TAG + std::to_string(HUD_SCORE_LABEL_START_VALUE));
    m_timerLabel->SetText(HUD_TIMER_START_VALUE);

    UIManager::Instance().AddElement(hudPanel);
}

/// @brief Helper method to load and play the game music for this scene.
void SandBoxScene::PlayGameMusic()
{
    if (!AudioManager::Instance().IsMusicPlaying() ||
        AudioManager::Instance().GetCurrentMusicName() != SandBoxAssets::GameTrack)
    {
        CT_LOG_INFO("SandBoxScene: Starting or resuming menu music.");
        AudioManager::Instance().PlayMusic(SandBoxAssets::GameTrack, true);
    }

    else
    {
        CT_LOG_INFO("SandBoxScene: Menu music already playing, no action needed.");
    }
}

/// @brief Sets up keyboard inputs that can be picked up during scene lifetime.
void SandBoxScene::BindInputKeys()
{
    InputManager::Instance().BindKey(PAUSE_BUTTON_KEY, sf::Keyboard::Key::Escape);
}

/// @brief Determines if any configured keyboard input has been pressed during scene update.
void SandBoxScene::CheckActionsPressed()
{
    auto &input = InputManager::Instance();

    if (input.IsKeyJustPressed(PAUSE_BUTTON_KEY))
    {
        CT_LOG_INFO("SandBoxScene: Pause Button Pressed.");

        m_hasPendingTransition = true;
        m_requestedScene = SceneID::Pause;
    }
}

/// @brief Updates the HUD Panel and all children entities.
/// @param dt Delta time since last update.
void SandBoxScene::UpdateHUD(float dt)
{
    // Update HUD timers and simulate game state
    m_elapsedTime += dt;

    if (m_elapsedTime >= 1.f)
    {
        m_secondsPassed += 1;
        m_elapsedTime = 0.f;

        // Update timer display
        int minutes = m_secondsPassed / 60;
        int seconds = m_secondsPassed % 60;

        // place a cap on timer
        if (minutes > 99)
        {
            minutes = 99;
            seconds = 0;
        }

        std::stringstream ss;
        ss << HUD_TIMER_TAG << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0')
           << seconds;
        m_timerLabel->SetText(ss.str());

        m_currentHealth = std::max(0, m_currentHealth - 1);
        m_currentScore += 100;
        m_scoreLabel->SetText(HUD_SCORE_TAG + std::to_string(m_currentScore));

        float normalized = static_cast<float>(m_currentHealth) / 100.f;
        normalized = std::clamp(normalized, 0.f, 1.f); // Ensure within [0, 1]
        m_healthGauge->SetValue(normalized);
    }
}

void SandBoxScene::MockFillableGaugeComponents()
{
    // --- NON HUD FillableGauge testing --- //

    FillableGaugeConfig testCfg2;
    testCfg2.borderColor = GAUGE_BORDER_COLOR_GOLD;
    testCfg2.borderThickness = DEFAULT_GAUGE_BORDER_THICKNESS;
    testCfg2.colorScheme = GaugeColorScheme::Gas;
    testCfg2.initialValue = .33f;
    testCfg2.orientation = LayoutMode::Vertical;
    testCfg2.relativePosition = {.33f, .33f};
    testCfg2.relativeSize = {0.015f, 0.2f};
    testCfg2.showPercentage = true;
    testCfg2.showTitle = true;
    testCfg2.titleFontSize = DEFAULT_GAUGE_FONT_SIZE;
    testCfg2.titlePadding = DEFAULT_GAUGE_TITLE_PADDING;
    testCfg2.titlePosition = GaugeTitlePosition::Left;
    testCfg2.titleScheme = UITextLabelScheme::MintyHerbScheme;
    testCfg2.titleText = "Gas-Vert";

    auto testGauge2 = UIFactory::Instance().CreateFillableGauge(testCfg2);

    UIManager::Instance().AddElement(testGauge2);

    FillableGaugeConfig testCfg;
    testCfg.borderColor = GAUGE_BORDER_COLOR_GOLD;
    testCfg.borderThickness = DEFAULT_GAUGE_BORDER_THICKNESS;
    testCfg.colorScheme = GaugeColorScheme::Gas;
    testCfg.initialValue = .33f;
    testCfg.orientation = LayoutMode::Horizontal;
    testCfg.relativePosition = {.5f, .5f};
    testCfg.relativeSize = {0.2f, 0.015f};
    testCfg.showPercentage = true;
    testCfg.showTitle = true;
    testCfg.titleFontSize = DEFAULT_GAUGE_FONT_SIZE;
    testCfg.titlePadding = DEFAULT_GAUGE_TITLE_PADDING;
    testCfg.titlePosition = GaugeTitlePosition::Above;
    testCfg.titleScheme = UITextLabelScheme::MintyHerbScheme;
    testCfg.titleText = "Gas-Horiz";

    auto testGauge = UIFactory::Instance().CreateFillableGauge(testCfg);
    UIManager::Instance().AddElement(testGauge);

    // ----- Testing Groupbox ------ //
    auto &scaleMgr = ResolutionScaleManager::Instance();
    const auto &windowSize = WindowManager::Instance().GetWindow().getSize();

    // === GroupBox relative setup ===
    const sf::Vector2f relativePos{0.80f, 0.75f};
    const sf::Vector2f relativeSize{0.16f, 0.18f};

    auto groupBox = UIFactory::Instance().CreateGroupBox("Ship Stats", relativePos, relativeSize);
    groupBox->SetLayoutMode(LayoutMode::Horizontal);
    groupBox->SetInternalPadding(scaleMgr.ScaledReferenceY(0.05f));
    groupBox->SetEdgePadding(scaleMgr.ScaledReferenceY(0.05f));
    groupBox->SetCenterChildren(true);

    // === HEALTH GAUGE ===
    FillableGaugeConfig healthConfig;
    healthConfig.borderColor = GAUGE_BORDER_COLOR_GOLD;
    healthConfig.borderThickness = DEFAULT_GAUGE_BORDER_THICKNESS;
    healthConfig.colorScheme = GaugeColorScheme::Health;
    healthConfig.initialValue = .25f;
    healthConfig.orientation = LayoutMode::Vertical;
    healthConfig.relativePosition = {0, 0}; // let groupbox orient
    healthConfig.relativeSize = {0.015f, 0.1f};
    healthConfig.showPercentage = true;
    healthConfig.showTitle = true;
    healthConfig.titleFontSize = DEFAULT_GAUGE_FONT_SIZE;
    healthConfig.titlePadding = DEFAULT_GAUGE_TITLE_PADDING;
    healthConfig.titlePosition = GaugeTitlePosition::Above;
    healthConfig.titleScheme = UITextLabelScheme::CougarScheme;
    healthConfig.titleText = "H";

    auto healthGauge = UIFactory::Instance().CreateFillableGauge(healthConfig);
    groupBox->AddElement(healthGauge);

    // === MANA GAUGE ===
    FillableGaugeConfig manaConfig;
    manaConfig.borderColor = GAUGE_BORDER_COLOR_GOLD;
    manaConfig.borderThickness = DEFAULT_GAUGE_BORDER_THICKNESS;
    manaConfig.colorScheme = GaugeColorScheme::Mana;
    manaConfig.initialValue = .10;
    manaConfig.orientation = LayoutMode::Vertical;
    manaConfig.relativePosition = {0, 0};
    manaConfig.relativeSize = {0.015f, 0.1f};
    manaConfig.showPercentage = true;
    manaConfig.showTitle = true;
    manaConfig.titleFontSize = DEFAULT_GAUGE_FONT_SIZE;
    manaConfig.titlePadding = DEFAULT_GAUGE_TITLE_PADDING;
    manaConfig.titlePosition = GaugeTitlePosition::Above;
    manaConfig.titleScheme = UITextLabelScheme::MintyHerbScheme;
    manaConfig.titleText = "M";

    auto manaGauge = UIFactory::Instance().CreateFillableGauge(manaConfig);
    groupBox->AddElement(manaGauge);

    // === GAS GAUGE ===
    FillableGaugeConfig gasConfig;
    gasConfig.borderColor = GAUGE_BORDER_COLOR_GOLD;
    gasConfig.borderThickness = DEFAULT_GAUGE_BORDER_THICKNESS;
    gasConfig.colorScheme = GaugeColorScheme::Gas;
    gasConfig.initialValue = .66f;
    gasConfig.orientation = LayoutMode::Vertical;
    gasConfig.relativePosition = {0, 0};
    gasConfig.relativeSize = {0.015f, 0.1f};
    gasConfig.showPercentage = true;
    gasConfig.showTitle = true;
    gasConfig.titleFontSize = DEFAULT_GAUGE_FONT_SIZE;
    gasConfig.titlePadding = DEFAULT_GAUGE_TITLE_PADDING;
    gasConfig.titlePosition = GaugeTitlePosition::Above;
    gasConfig.titleScheme = UITextLabelScheme::BlueSteelScheme;
    gasConfig.titleText = "G";

    auto gasGauge = UIFactory::Instance().CreateFillableGauge(gasConfig);
    groupBox->AddElement(gasGauge);

    // when in groupbox, this is required for correct fill orientation
    healthGauge->SetOrientation(LayoutMode::Vertical);
    manaGauge->SetOrientation(LayoutMode::Vertical);
    gasGauge->SetOrientation(LayoutMode::Vertical);

    UIManager::Instance().AddElement(groupBox);
}
