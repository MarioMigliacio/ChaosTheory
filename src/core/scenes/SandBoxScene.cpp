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
#include "Allegiance.h"
#include "AssetManager.h"
#include "Assets.h"
#include "AudioManager.h"
#include "CollectableIconManager.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "MainMenuScene.h"
#include "PauseScene.h"
#include "ProjectileManager.h"
#include "ResolutionScaleManager.h"
#include "SceneFactory.h"
#include "SceneTransitionManager.h"
#include "ShipFactory.h"
#include "ShipManager.h"
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

/// @brief Fixed name constant to be used with BindActionKey to setup the skip chatbox dialog.
constexpr auto SKIP_CHAT_KEY = "Space";

/// @brief Fixed name constant to be used with the ship stats group label box.
constexpr auto SHIP_STATS_GROUPBOX_LABEL = "Ship Stats";

/// @brief Quick enabling of method.
constexpr bool TEST_ENABLED = true;

/// @brief Quick disabling of method.
constexpr bool TEST_DISABLED = false;

/// @brief Quick disabling of HUD methods.
constexpr bool HUD_MOCK_BOOL = true;

/// @brief Quick disabling of UNITS test methods.
constexpr bool UNITS_MOCK_BOOL = true;

/// @brief Quick disabling of PLAYER test methods.
constexpr bool PLAYER_MOCK_BOOL = true;
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

    if (!CollisionManager::Instance().IsInitialized())
    {
        CollisionManager::Instance().Init(128.f);
    }

    if (!ShipManager::Instance().IsInitialized())
    {
        ShipManager::Instance().Init();
    }

    if (!ProjectileManager::Instance().IsInitialized())
    {
        ProjectileManager::Instance().Init();
    }

    if (!CollectableIconManager::Instance().IsInitialized())
    {
        CollectableIconManager::Instance().Init();
    }

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

    for (const auto &[key, path] : SandBoxAssets::Sounds)
    {
        if (!AssetManager::Instance().LoadSound(key, path))
        {
            CT_LOG_ERROR("SandBoxScene failed to load sound asset: {} -> {}", key, path);
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

    ShipManager::Instance().Shutdown();
    ProjectileManager::Instance().Shutdown();
    CollectableIconManager::Instance().Shutdown();

    m_settings.reset();
    m_testChatBox.reset();
    m_dialogQueue.Clear();
    m_isInitialized = false;

    CT_LOG_INFO("SandBoxScene shutdown.");
}

/// @brief Handles the exit criteria for this scene.
void SandBoxScene::OnExit()
{
    InputManager::Instance().UnbindKey(PAUSE_BUTTON_KEY);
    InputManager::Instance().UnbindKey(SKIP_CHAT_KEY);
    InputManager::Instance().UnloadPlayerInput();

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
    UpdateHUD(dt, HUD_MOCK_BOOL);

    ShipManager::Instance().Update(dt);
    ProjectileManager::Instance().Update(dt);
    CollectableIconManager::Instance().Update(dt);

    CollisionManager &collisionMgr = CollisionManager::Instance();
    collisionMgr.Clear();

    ShipManager::Instance().RegisterForCollision(collisionMgr);
    ProjectileManager::Instance().RegisterForCollision(collisionMgr);
    CollectableIconManager::Instance().RegisterForCollision(collisionMgr);

    collisionMgr.Update(dt);

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

    ShipManager::Instance().Draw(window);
    ProjectileManager::Instance().Draw(window);
    CollectableIconManager::Instance().Draw(window);

    UIManager::Instance().Render(window);
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

/// @brief Sets up keyboard inputs that can be picked up during scene lifetime.
void SandBoxScene::BindInputKeys()
{
    InputManager::Instance().BindKey(PAUSE_BUTTON_KEY, sf::Keyboard::Key::Escape);
    InputManager::Instance().BindKey(SKIP_CHAT_KEY, sf::Keyboard::Key::Space);
    InputManager::Instance().LoadPlayerInput();
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

    if (m_testChatBox)
    {
        if (input.IsKeyJustPressed(SKIP_CHAT_KEY))
        {
            // Chatbox is not finished with its line; skip to line end.
            if (!m_testChatBox->IsTypingComplete())
            {
                m_testChatBox->SkipTyping();
            }

            // Chatbox is at the end of a line, and there's more dialog; skip to next line.
            else if (m_testChatBox->HasMoreLines())
            {
                m_testChatBox->StartNextLine();
            }

            // Chatbox is at the end of a dialog, but there may be another dialog in the queue; skip to next dialog.
            else if (m_dialogQueue.HasNext())
            {
                StartNextDialog();
            }

            // Fully done - remove chatbox
            else
            {
                m_testChatBox->Clear();
                UIManager::Instance().RemoveElement(m_testChatBox);
                m_testChatBox.reset();
            }
        }
    }
}

/// @brief Update the dialog queue to the next in line.
void SandBoxScene::StartNextDialog()
{
    DialogLine next = m_dialogQueue.Next();

    // Reconfigure title & icon first
    m_testChatBox->SetSpeaker(next.speakerName, next.showTitle, next.iconTextureKey, next.iconType);

    // Then clear any previous lines
    m_testChatBox->Clear();

    // Then add the new line text
    m_testChatBox->AddLine(next.text);
}

/// @brief Helper method to initialize necessary Scene components.
void SandBoxScene::SetupSceneComponents()
{
    PlayGameMusic();
    MockTitleText(TEST_ENABLED);
    MockHUDPanel(HUD_MOCK_BOOL);
    MockShipStatusComponent(TEST_ENABLED);
    MockIconComponents(TEST_ENABLED);
    MockChatBox(TEST_DISABLED);
    MockPlayerUnit(PLAYER_MOCK_BOOL);
    MockSpawnTestShips(UNITS_MOCK_BOOL);
}

/// @brief Helper method to create the Title string entity for this scene.
void SandBoxScene::MockTitleText(const bool enabled)
{
    if (!enabled)
    {
        return;
    }

    auto &scaleMgr = ResolutionScaleManager::Instance();

    const std::string titleLabel = TITLE_SCREEN_LABEL;
    const unsigned int titleFontSize = scaleMgr.ScaleFont(48);
    const sf::Vector2f titlePos = {WindowManager::Instance().GetWindow().getSize().x / 2.f,
                                   scaleMgr.ScaledReferenceY(0.15f)};

    const std::string helpLabel = PAUSE_GAME_LABEL;
    const unsigned int helpFontSize = scaleMgr.ScaleFont(20);
    const sf::Vector2f helpPos = {WindowManager::Instance().GetWindow().getSize().x / 2.f,
                                  scaleMgr.ScaledReferenceY(0.25f)};

    m_titleLabel = UIFactory::Instance().CreateTextLabel(TextLabelConfig{.text = titleLabel,
                                                                         .position = titlePos,
                                                                         .fontSize = titleFontSize,
                                                                         .scheme = UITextLabelScheme::MintyHerbScheme});

    m_helpLabel = UIFactory::Instance().CreateTextLabel(TextLabelConfig{.text = helpLabel,
                                                                        .position = helpPos,
                                                                        .fontSize = helpFontSize,
                                                                        .scheme = UITextLabelScheme::MintyHerbScheme});

    UIManager::Instance().AddElement(m_titleLabel);
    UIManager::Instance().AddElement(m_helpLabel);
}

/// @brief Initializes the HUD Panel ui component.
void SandBoxScene::MockHUDPanel(const bool enabled)
{
    if (!enabled)
    {
        return;
    }

    auto &scaleMgr = ResolutionScaleManager::Instance();

    // Relative position and size (top bar)
    const sf::Vector2f relativePos{0.f, 0.f};
    const sf::Vector2f relativeSize{1.0f, 0.05f}; // Full width, 8% height

    auto hudPanel = UIFactory::Instance().CreateHUDPanel(HUDPanelConfig{.position = relativePos, .size = relativeSize});
    hudPanel->SetInternalPadding(scaleMgr.ScaledReferenceY(0.15f)); // Space between labels
    hudPanel->SetEdgePadding(scaleMgr.ScaledReferenceY(0.01f));     // Padding around edges
    hudPanel->SetLayoutMode(LayoutMode::Horizontal);
    hudPanel->SetCenterChildren(false);

    const unsigned int fontSize = ResolutionScaleManager::Instance().ScaleFont(18);
    const sf::Vector2f gaugeRelativeSize = {0.2f, 0.015f}; // Width, Height in screen %

    m_scoreLabel = UIFactory::Instance().CreateTextLabel(TextLabelConfig{
        .text = HUD_SCORE_LABEL_INIT_STR, .position = relativePos, .fontSize = fontSize, .centerOrigin = false});
    m_timerLabel = UIFactory::Instance().CreateTextLabel(TextLabelConfig{
        .text = HUD_TIMER_LABEL_INIT_STR, .position = relativePos, .fontSize = fontSize, .centerOrigin = false});
    m_healthGauge =
        UIFactory::Instance().CreateFillableGauge(FillableGaugeConfig{.position = relativePos,
                                                                      .size = gaugeRelativeSize,
                                                                      .colorScheme = GaugeColorScheme::Health,
                                                                      .borderThickness = DEFAULT_GAUGE_BORDER_THICKNESS,
                                                                      .borderColor = DEFAULT_GAUGE_BORDER_COLOR,
                                                                      .showPercentage = true,
                                                                      .showTitle = true,
                                                                      .titleText = HUD_HEALTH_TAG});

    hudPanel->AddElement(m_healthGauge, HUDSlotAlignment::Left);
    hudPanel->AddElement(m_timerLabel, HUDSlotAlignment::Right);
    hudPanel->AddElement(m_scoreLabel, HUDSlotAlignment::Right);

    m_scoreLabel->SetText(HUD_SCORE_TAG + std::to_string(HUD_SCORE_LABEL_START_VALUE));
    m_timerLabel->SetText(HUD_TIMER_START_VALUE);

    UIManager::Instance().AddElement(hudPanel);
}

/// @brief Helper method to test fillable gauge ui components.
/// @param enabled Whether or not to use this MOCK in SandBox.
void SandBoxScene::MockShipStatusComponent(const bool enabled)
{
    if (!enabled)
    {
        return;
    }

    sf::Vector2f pos{0, 0};
    sf::Vector2f size{0.015f, 0.1f};

    // ----- Testing Groupbox ------ //
    auto &scaleMgr = ResolutionScaleManager::Instance();
    const auto &windowSize = WindowManager::Instance().GetWindow().getSize();

    // === GroupBox relative setup ===
    const sf::Vector2f relativePos{0.80f, 0.75f};
    const sf::Vector2f relativeSize{0.16f, 0.18f};

    // TODO - link with the ship health/gas values.
    GroupBoxConfig cfg{
        .position = relativePos, .size = relativeSize, .useTitle = true, .title = SHIP_STATS_GROUPBOX_LABEL};

    auto groupBox = UIFactory::Instance().CreateGroupBox(GroupBoxConfig{
        .position = relativePos, .size = relativeSize, .useTitle = true, .title = SHIP_STATS_GROUPBOX_LABEL});
    groupBox->SetLayoutMode(LayoutMode::Horizontal);
    groupBox->SetInternalPadding(scaleMgr.ScaledReferenceY(0.05f));
    groupBox->SetEdgePadding(scaleMgr.ScaledReferenceY(0.05f));
    groupBox->SetCenterChildren(true);

    // === HEALTH GAUGE ===
    auto healthGauge =
        UIFactory::Instance().CreateFillableGauge(FillableGaugeConfig{.position = pos,
                                                                      .size = size,
                                                                      .orientation = LayoutMode::Vertical,
                                                                      .colorScheme = GaugeColorScheme::Health,
                                                                      .initialValue = .25f,
                                                                      .borderThickness = DEFAULT_GAUGE_BORDER_THICKNESS,
                                                                      .borderColor = GAUGE_BORDER_COLOR_GOLD,
                                                                      .showPercentage = true,
                                                                      .showTitle = true,
                                                                      .titleText = "H",
                                                                      .titleScheme = UITextLabelScheme::CougarScheme,
                                                                      .titlePosition = GaugeTitlePosition::Above});
    groupBox->AddElement(healthGauge);

    // === GAS GAUGE ===
    auto gasGauge =
        UIFactory::Instance().CreateFillableGauge(FillableGaugeConfig{.position = pos,
                                                                      .size = size,
                                                                      .orientation = LayoutMode::Vertical,
                                                                      .colorScheme = GaugeColorScheme::Gas,
                                                                      .initialValue = .33f,
                                                                      .borderThickness = DEFAULT_GAUGE_BORDER_THICKNESS,
                                                                      .borderColor = GAUGE_BORDER_COLOR_GOLD,
                                                                      .showPercentage = true,
                                                                      .showTitle = true,
                                                                      .titleText = "G",
                                                                      .titleScheme = UITextLabelScheme::BlueSteelScheme,
                                                                      .titlePosition = GaugeTitlePosition::Above});
    groupBox->AddElement(gasGauge);

    // when in groupbox, this is required for correct fill orientation
    healthGauge->SetOrientation(LayoutMode::Vertical);
    gasGauge->SetOrientation(LayoutMode::Vertical);

    UIManager::Instance().AddElement(groupBox);
}

/// @brief Helper method to test Icon ui components.
/// @param enabled Whether or not to use this MOCK in SandBox.
void SandBoxScene::MockIconComponents(const bool enabled)
{
    if (!enabled)
    {
        return;
    }

    const float startY = 50.f;
    const float startX = 75.f;
    const float spacing = 48.f;
    const sf::Vector2f iconSize = {32.f, 32.f};

    std::vector<std::pair<IconType, std::string>> iconTypes = {
        {IconType::AtomicIcon, SpriteAssets::IconAssets::AtomicIconSpriteKey},
        {IconType::FireRateIcon, SpriteAssets::IconAssets::FireRateIconSpriteKey},
        {IconType::GasIcon, SpriteAssets::IconAssets::GasIconSpriteKey},
        {IconType::LifeIcon, SpriteAssets::IconAssets::LifeIconSpriteKey},
        {IconType::PowerIcon, SpriteAssets::IconAssets::PowerIconSpriteKey},
        {IconType::UpgradeIcon, SpriteAssets::IconAssets::UpgradeIconSpriteKey},
        {IconType::WarpIcon, SpriteAssets::IconAssets::WarpIconSpriteKey},
    };

    for (size_t i = 0; i < iconTypes.size(); i++)
    {
        sf::Vector2f pos{startX + (i * spacing), startY};
        CollectableIconManager::Instance().SpawnIcon({pos, iconSize, iconTypes[i].second, iconTypes[i].first});
    }
}

/// @brief Composes a UIChatBox entity and runs a mock dialog.
/// @param enabled Whether or not to use this MOCK in SandBox.
void SandBoxScene::MockChatBox(const bool enabled)
{
    if (!enabled)
    {
        return;
    }

    // Clear any previous
    m_dialogQueue.Clear();

    // Add all lines from predefined vector
    for (const auto &line : DialogConstants::IntroDialog::INTRO_SEQUENCE)
    {
        m_dialogQueue.AddLine(line);
    }

    if (m_dialogQueue.HasNext())
    {
        DialogLine next = m_dialogQueue.Next();

        m_testChatBox =
            UIFactory::Instance().CreateChatBox(ChatBoxConfig{.position = DEFAULT_CHATBOX_POSITION,
                                                              .size = DEFAULT_CHATBOX_SIZE,
                                                              .showTitle = next.showTitle,
                                                              .title = next.speakerName,
                                                              .typeSpeed = DEFAULT_CHATBOX_DIALOG_SPEED,
                                                              .textScheme = UITextLabelScheme::DefaultScheme,
                                                              .titleScheme = UITextLabelScheme::DefaultScheme,
                                                              .useSpeakerIcon = !next.iconTextureKey.empty(),
                                                              .iconTextureKey = next.iconTextureKey,
                                                              .iconType = next.iconType});

        m_testChatBox->AddLine(next.text);
        UIManager::Instance().AddElement(m_testChatBox);
    }
}

void SandBoxScene::MockSpawnTestShips(const bool enabled)
{
    if (!enabled)
    {
        return;
    }

    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    const sf::Vector2f startPos = {winSize.x * 0.25f, 25.f};

    for (int i = 0; i < 5; ++i)
    {
        ShipManager::Instance().SpawnBasicEnemy({startPos.x + i * 100.f, startPos.y});
    }

    for (int i = 5; i < 10; ++i)
    {
        ShipManager::Instance().SpawnAlienEnemy({startPos.x + i * 100.f, startPos.y});
    }
}

void SandBoxScene::MockPlayerUnit(const bool enabled)
{
    if (!enabled)
    {
        return;
    }

    auto winSize = WindowManager::Instance().GetWindow().getSize();

    // Spawn player through ShipManager to ensure it is tracked and registered for collisions
    ShipManager::Instance().SpawnPlayer({winSize.x / 2.f, winSize.y - 100.f});
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

/// @brief Updates the HUD Panel and all children entities.
/// @param dt Delta time since last update.
/// @param enabled Whether or not the HUD is enabled or not for this SandBox.
void SandBoxScene::UpdateHUD(float dt, const bool enabled)
{
    if (!enabled)
    {
        return;
    }

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
