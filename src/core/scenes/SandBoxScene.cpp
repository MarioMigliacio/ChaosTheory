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

/// @brief Fixed name constant to be used with the ship stats group label box.
constexpr auto SHIP_STATS_GROUPBOX_LABEL = "Ship Stats";
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
    for (const auto &[key, path] : SandBoxAssets::Backgrounds)
    {
        if (!AssetManager::Instance().LoadTexture(key, path))
        {
            CT_LOG_ERROR("SandBoxScene failed to load texture asset: {} -> {}", key, path);
        }
    }

    for (const auto &[key, path] : SandBoxAssets::Sprites)
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
    MockIconComponents();
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
                                   scaleMgr.ScaledReferenceY(0.15f)};

    const std::string helpLabel = PAUSE_GAME_LABEL;
    const unsigned int helpFontSize = scaleMgr.ScaleFont(20);
    const sf::Vector2f helpPos = {WindowManager::Instance().GetWindow().getSize().x / 2.f,
                                  scaleMgr.ScaledReferenceY(0.25f)};

    m_titleLabel = UIFactory::Instance().CreateTextLabel(
        INIT_TEXTLABEL_CONFIG(titleLabel, titlePos, titleFontSize, true, UITextLabelScheme::MintyHerbScheme));

    m_helpLabel = UIFactory::Instance().CreateTextLabel(
        INIT_TEXTLABEL_CONFIG(helpLabel, helpPos, helpFontSize, true, UITextLabelScheme::MintyHerbScheme));

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

    auto hudPanel = UIFactory::Instance().CreateHUDPanel(
        INIT_HUDPANEL_CONFIG(relativePos, relativeSize, DEFAULT_HUD_COLOR, DEFAULLT_HUD_BACKGROUND_COLOR, 0.f));
    hudPanel->SetInternalPadding(scaleMgr.ScaledReferenceY(0.15f)); // Space between labels
    hudPanel->SetEdgePadding(scaleMgr.ScaledReferenceY(0.01f));     // Padding around edges
    hudPanel->SetLayoutMode(LayoutMode::Horizontal);
    hudPanel->SetCenterChildren(false);

    const unsigned int fontSize = ResolutionScaleManager::Instance().ScaleFont(18);
    const sf::Vector2f gaugeRelativeSize = {0.2f, 0.015f}; // Width, Height in screen %

    m_scoreLabel = UIFactory::Instance().CreateTextLabel(INIT_TEXTLABEL_CONFIG(
        HUD_SCORE_LABEL_INIT_STR, relativePos, fontSize, false, UITextLabelScheme::DefaultScheme));
    m_timerLabel = UIFactory::Instance().CreateTextLabel(INIT_TEXTLABEL_CONFIG(
        HUD_TIMER_LABEL_INIT_STR, relativePos, fontSize, false, UITextLabelScheme::DefaultScheme));
    m_healthGauge = UIFactory::Instance().CreateFillableGauge(INIT_CUSTOM_FILLABLE_GAUGE_CONFIG(
        relativePos, gaugeRelativeSize, LayoutMode::Horizontal, GaugeColorScheme::Health, DEFAULT_GAUGE_FULL_VALUE,
        DEFAULT_GAUGE_BORDER_THICKNESS, DEFAULT_GAUGE_BORDER_COLOR, true, true, HUD_HEALTH_TAG, DEFAULT_GAUGE_FONT_SIZE,
        UITextLabelScheme::DefaultScheme, GaugeTitlePosition::Left, DEFAULT_GAUGE_TITLE_PADDING));

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

/// @brief Helper method to test fillable gauge ui components.
void SandBoxScene::MockFillableGaugeComponents()
{
    // --- NON HUD FillableGauge testing --- //
    sf::Vector2f pos(0.33f, 0.33f);
    sf::Vector2f size(0.015f, 0.2f);

    auto looseGasGauge = UIFactory::Instance().CreateFillableGauge(INIT_CUSTOM_FILLABLE_GAUGE_CONFIG(
        pos, size, LayoutMode::Vertical, GaugeColorScheme::Gas, .33f, DEFAULT_GAUGE_BORDER_THICKNESS,
        GAUGE_BORDER_COLOR_GOLD, true, true, "Gas-Vert", DEFAULT_GAUGE_FONT_SIZE, UITextLabelScheme::MintyHerbScheme,
        GaugeTitlePosition::Left, DEFAULT_GAUGE_TITLE_PADDING));

    UIManager::Instance().AddElement(looseGasGauge);

    pos = {.5f, .5f};
    size = {0.2f, 0.015f};

    auto looseGasGauge2 = UIFactory::Instance().CreateFillableGauge(INIT_CUSTOM_FILLABLE_GAUGE_CONFIG(
        pos, size, LayoutMode::Horizontal, GaugeColorScheme::Gas, .33f, DEFAULT_GAUGE_BORDER_THICKNESS,
        GAUGE_BORDER_COLOR_GOLD, true, true, "Gas-Horiz", DEFAULT_GAUGE_FONT_SIZE, UITextLabelScheme::MintyHerbScheme,
        GaugeTitlePosition::Above, DEFAULT_GAUGE_TITLE_PADDING));

    UIManager::Instance().AddElement(looseGasGauge2);

    // ----- Testing Groupbox ------ //
    auto &scaleMgr = ResolutionScaleManager::Instance();
    const auto &windowSize = WindowManager::Instance().GetWindow().getSize();

    // === GroupBox relative setup ===
    const sf::Vector2f relativePos{0.80f, 0.75f};
    const sf::Vector2f relativeSize{0.16f, 0.18f};

    auto groupBox = UIFactory::Instance().CreateGroupBox(INIT_GROUPBOX_CONFIG(
        relativePos, relativeSize, true, SHIP_STATS_GROUPBOX_LABEL, true, UITextLabelScheme::DefaultScheme));
    groupBox->SetLayoutMode(LayoutMode::Horizontal);
    groupBox->SetInternalPadding(scaleMgr.ScaledReferenceY(0.05f));
    groupBox->SetEdgePadding(scaleMgr.ScaledReferenceY(0.05f));
    groupBox->SetCenterChildren(true);

    // === HEALTH GAUGE ===
    pos = {0, 0};
    size = {0.015f, 0.1f};

    auto healthGauge = UIFactory::Instance().CreateFillableGauge(INIT_CUSTOM_FILLABLE_GAUGE_CONFIG(
        pos, size, LayoutMode::Vertical, GaugeColorScheme::Health, .25f, DEFAULT_GAUGE_BORDER_THICKNESS,
        GAUGE_BORDER_COLOR_GOLD, true, true, "H", DEFAULT_GAUGE_FONT_SIZE, UITextLabelScheme::CougarScheme,
        GaugeTitlePosition::Above, DEFAULT_GAUGE_TITLE_PADDING));

    groupBox->AddElement(healthGauge);

    // === MANA GAUGE ===
    auto manaGauge = UIFactory::Instance().CreateFillableGauge(INIT_CUSTOM_FILLABLE_GAUGE_CONFIG(
        pos, size, LayoutMode::Vertical, GaugeColorScheme::Mana, .10f, DEFAULT_GAUGE_BORDER_THICKNESS,
        GAUGE_BORDER_COLOR_GOLD, true, true, "M", DEFAULT_GAUGE_FONT_SIZE, UITextLabelScheme::MintyHerbScheme,
        GaugeTitlePosition::Above, DEFAULT_GAUGE_TITLE_PADDING));

    groupBox->AddElement(manaGauge);

    // === GAS GAUGE ===
    auto gasGauge = UIFactory::Instance().CreateFillableGauge(INIT_CUSTOM_FILLABLE_GAUGE_CONFIG(
        pos, size, LayoutMode::Vertical, GaugeColorScheme::Gas, .66f, DEFAULT_GAUGE_BORDER_THICKNESS,
        GAUGE_BORDER_COLOR_GOLD, true, true, "G", DEFAULT_GAUGE_FONT_SIZE, UITextLabelScheme::BlueSteelScheme,
        GaugeTitlePosition::Above, DEFAULT_GAUGE_TITLE_PADDING));

    groupBox->AddElement(gasGauge);

    // when in groupbox, this is required for correct fill orientation
    healthGauge->SetOrientation(LayoutMode::Vertical);
    manaGauge->SetOrientation(LayoutMode::Vertical);
    gasGauge->SetOrientation(LayoutMode::Vertical);

    UIManager::Instance().AddElement(groupBox);
}

/// @brief Helper method to test Icon ui components.
void SandBoxScene::MockIconComponents()
{
    const auto &window = WindowManager::Instance().GetWindow();
    const float startY = 50.f;
    const sf::Vector2f iconSize = {32.f, 32.f};
    const float spacing = 48.f;
    const float startX = 75.f;

    struct IconSpawnData
    {
        IconType type;
        std::string spriteKey;
    };

    std::vector<IconSpawnData> iconTypesToTest = {
        {IconType::AtomicIcon, SpriteAssets::AtomicIconSpriteKey},
        {IconType::FireRateIcon, SpriteAssets::FireRateIconSpriteKey},
        {IconType::GasIcon, SpriteAssets::GasIconSpriteKey},
        {IconType::LifeIcon, SpriteAssets::LifeIconSpriteKey},
        {IconType::PowerIcon, SpriteAssets::PowerIconSpriteKey},
        {IconType::UpgradeIcon, SpriteAssets::UpgradeIconSpriteKey},
        {IconType::WarpIcon, SpriteAssets::WarpIconSpriteKey},
    };

    for (std::size_t i = 0; i < iconTypesToTest.size(); i++)
    {
        const auto &entry = iconTypesToTest[i];
        sf::Vector2f pos{startX + (i * spacing), startY};

        auto icon = UIFactory::Instance().CreateIcon(INIT_ICON_CONFIG(pos, iconSize, entry.spriteKey, entry.type));
        UIManager::Instance().AddElement(icon);
    }
}
