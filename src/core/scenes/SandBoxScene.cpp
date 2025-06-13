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
constexpr auto PAUSE_GAME_LABEL = "Press Spacebar to Pause";

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
    hudPanel->SetInternalPadding(scaleMgr.ScaledReferenceY(0.2f)); // Space between labels
    hudPanel->SetEdgePadding(scaleMgr.ScaledReferenceY(0.01f));    // Padding around edges
    hudPanel->SetLayoutMode(LayoutMode::Horizontal);
    hudPanel->SetCenterChildren(false);

    const unsigned int fontSize = ResolutionScaleManager::Instance().ScaleFont(18);

    m_healthLabel = UIFactory::Instance().CreateTextLabel(HUD_HEALTH_LABEL_INIT_STR, {0.f, 0.f}, fontSize, false);
    m_scoreLabel = UIFactory::Instance().CreateTextLabel(HUD_SCORE_LABEL_INIT_STR, {0.f, 0.f}, fontSize, false);
    m_timerLabel = UIFactory::Instance().CreateTextLabel(HUD_TIMER_LABEL_INIT_STR, {0.f, 0.f}, fontSize, false);

    hudPanel->AddElement(m_healthLabel, HUDSlotAlignment::Left);
    hudPanel->AddElement(m_timerLabel, HUDSlotAlignment::Right);
    hudPanel->AddElement(m_scoreLabel, HUDSlotAlignment::Right);

    m_healthLabel->SetText(HUD_HEALTH_TAG + std::to_string(HUD_HEALTH_LABEL_START_VALUE));
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
    InputManager::Instance().BindKey(PAUSE_BUTTON_KEY, sf::Keyboard::Key::Space);
}

/// @brief Determines if any configured keyboard input has been pressed during scene update.
void SandBoxScene::CheckActionsPressed()
{
    auto &input = InputManager::Instance();

    if (input.IsKeyJustPressed(PAUSE_BUTTON_KEY))
    {
        CT_LOG_INFO("SandBoxScene: Toggle Button Pressed.");

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

        // Every 5 seconds, reduce HP and increase score
        if (m_secondsPassed % 5 == 0)
        {
            m_currentHealth = std::max(0, m_currentHealth - 1);
            m_currentScore += 100;

            m_healthLabel->SetText(HUD_HEALTH_TAG + std::to_string(m_currentHealth));
            m_scoreLabel->SetText(HUD_SCORE_TAG + std::to_string(m_currentScore));
        }
    }
}
