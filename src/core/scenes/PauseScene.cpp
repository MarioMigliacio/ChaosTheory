// ============================================================================
//  File        : PauseScene.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-109
//  Description : Hosts the definitions for Pause Scene Object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "PauseScene.h"
#include "AssetManager.h"
#include "Assets.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "Macros.h"
#include "MainMenuScene.h"
#include "ResolutionScaleManager.h"
#include "SceneManager.h"
#include "SceneTransitionManager.h"
#include "SettingsScene.h"
#include "UIFactory.h"
#include "UIManager.h"
#include "WindowManager.h"

/// @brief Constants that can be adjusted throughout the PauseScene.
namespace
{
/// @brief Fixed name constant for the pause title label.
constexpr auto PAUSE_TITLE_LABEL = "Paused";

/// @brief Fixed name constant for the settings button label.
constexpr auto SETTINGS_BTN_LABEL = "Settings";

/// @brief Fixed name constant for the resume button label.
constexpr auto RESUME_BTN_LABEL = "Resume";

/// @brief Fixed name constant for the return button label.
constexpr auto RETURN_BTN_LABEL = "Return to Menu";
} // namespace

/// @brief Constructor for the PauseScene.
/// @param settings Internal settings to initialize with.
PauseScene::PauseScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

/// @brief Initializes the PauseScene.
void PauseScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    UIManager::Instance().Clear();

    LoadRequiredAssets();
    PauseAudio();
    SetupUI();

    m_isInitialized = true;

    CT_LOG_INFO("PauseScene initialized.");
}

/// @brief Load any required assets relevant to the PauseScene.
void PauseScene::LoadRequiredAssets()
{
    auto &assets = AssetManager::Instance();

    for (const auto &[key, path] : PauseAssets::UI)
    {
        if (!AssetManager::Instance().LoadTexture(key, path))
        {
            CT_LOG_ERROR("PauseScene::LoadRequiredAssets::LoadTexture failed to load asset: {}, {}", key, path);
        }
    }

    for (const auto &[key, path] : FontAssets::Fonts)
    {
        if (!assets.LoadFont(key, path))
        {
            CT_LOG_ERROR("PauseScene::LoadRequiredAssets::LoadFont failed to load Asset: {}, {}", key, path);
        }
    }

    CT_LOG_INFO("PauseScene finished LoadRequiredAssets.");
}

/// @brief Shuts down this scene and resets internal state.
void PauseScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("PauseScene", "Shutdown");

    UIManager::Instance().Clear();
    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("PauseScene Shutdown.");
}

/// @brief Handle any relevent exit criteria if needed.
void PauseScene::OnExit()
{
    AudioManager::Instance().ResumeMusic();

    CT_LOG_INFO("PauseScene OnExit.");
}

/// @brief Resumes Scene in event of a Pause. This can be triggered by returning from the Pause Menu's Setting page go
/// back.
void PauseScene::OnResume()
{
    UIManager::Instance().Clear();
    SetupUI();
    CT_LOG_INFO("PauseScene resumed and UI restored.");
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update.
void PauseScene::Update(float dt)
{
    const auto mousePos = InputManager::Instance().GetMousePosition();
    const bool isPressed = InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left);
    const bool isJustPressed = InputManager::Instance().IsMouseButtonJustPressed(sf::Mouse::Left);

    UIManager::Instance().Update(mousePos, isPressed, isJustPressed, dt);

    if (m_hasPendingTransition)
    {
        if (m_requestedScene == SceneID::Settings)
        {
            CT_LOG_INFO("PauseScene pushing Settings page.");
            m_hasPendingTransition = false;

            SceneManager::Instance().PushScene(
                std::make_unique<SettingsScene>(m_settings, false,
                                                []()
                                                {
                                                    SceneManager::Instance().DeferPopScene(); // Return to PauseScene
                                                }));
        }

        else if (m_requestedScene == SceneID::MainMenu)
        {
            CT_LOG_INFO("PauseScene Requesting Scene Change to '{}'", SceneIDToString(m_requestedScene));
            m_hasPendingTransition = false;
            SceneID tempState = m_requestedScene;

            // <-- kill this scene = sandbox scene exists, then request that be changed.
            SceneTransitionManager::Instance().ForceFullyOpaque();
            SceneManager::Instance().PopScene();
            SceneManager::Instance().RequestSceneChange(tempState);
        }
    }

    else if (m_shouldExit)
    {
        m_shouldExit = false;
        SceneManager::Instance().PopScene();

        CT_LOG_INFO("PauseScene requested exit. Popping scene...");
    }
}

/// @brief Handle any quick cancelation requests if present.
/// @param event bubbled down from caller, not needed.
void PauseScene::HandleEvent(const sf::Event &event)
{
}

/// @brief Not used in PauseScene context.
/// @param newSize bubbled down from caller, not needed.
void PauseScene::OnResize(const sf::Vector2u &newSize)
{
}

/// @brief While this scene is active, render the necessary components.
void PauseScene::Render()
{
    CT_WARN_IF_UNINITIALIZED("PauseScene", "Render");

    auto &window = WindowManager::Instance().GetWindow();

    // Optional: Draw dimming overlay
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150)); // 150 alpha to dim
    window.draw(overlay);

    UIManager::Instance().Render(window);
}

/// @brief Creates the necessary components for the PauseScene.
void PauseScene::SetupUI()
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const std::string title = PAUSE_TITLE_LABEL;
    const sf::Vector2f relativePos{0.375f, 0.30f}; // Centered box
    const sf::Vector2f relativeSize{0.25f, 0.40f}; // 1/4 width, 40% height

    // Create transparent group box
    auto groupBox = UIFactory::Instance().CreateGroupBox(title, relativePos, relativeSize);
    groupBox->SetEdgePadding(scaleMgr.ScaledReferenceY(.02f));
    groupBox->SetInternalPadding(scaleMgr.ScaledReferenceY(.08f * relativeSize.y));
    groupBox->SetOutlineColor(sf::Color::Transparent);
    groupBox->SetOutlineThickness(0.f);
    groupBox->SetFillColor(sf::Color(0, 0, 0, 0)); // Transparent background

    const sf::Vector2f buttonSize = {BASE_BUTTON_WIDTH_PIXEL, BASE_BUTTON_HEIGHT_PIXEL};

    // === 1. UNPAUSE GAME ===
    auto btnResume = UIFactory::Instance().CreateSkinnableButton(
        {0.f, 0.f}, buttonSize, RESUME_BTN_LABEL, UIAssets::UISkinButtonBlueIdleKey, UIAssets::UISkinButtonBlueHoverKey,
        UIButtonColorScheme::Blue,
        [this]()
        {
            m_shouldExit = true;
            CT_LOG_INFO("PauseScene: Unpause Button Clicked.");
        });

    // === 2. SETTINGS ===
    auto btnSettings = UIFactory::Instance().CreateSkinnableButton(
        {0.f, 0.f}, buttonSize, SETTINGS_BTN_LABEL, UIAssets::UISkinButtonGreenIdleKey,
        UIAssets::UISkinButtonGreenHoverKey, UIButtonColorScheme::Green,
        [this]()
        {
            m_hasPendingTransition = true;
            m_requestedScene = SceneID::Settings;
            CT_LOG_INFO("PauseScene: Settings Button Clicked.");
        });

    // === 3. RETURN TO MENU ===
    auto btnReturn = UIFactory::Instance().CreateSkinnableButton(
        {0.f, 0.f}, buttonSize, RETURN_BTN_LABEL, UIAssets::UISkinButtonRedIdleKey, UIAssets::UISkinButtonRedHoverKey,
        UIButtonColorScheme::Red,
        [this]()
        {
            m_hasPendingTransition = true;
            m_requestedScene = SceneID::MainMenu;
            CT_LOG_INFO("PauseScene: Return to Menu Button Clicked.");
        });

    groupBox->AddElement(btnResume);
    groupBox->AddElement(btnSettings);
    groupBox->AddElement(btnReturn);

    UIManager::Instance().AddElement(groupBox);
}

/// @brief Halts any current playing audio from the AudioManager.
void PauseScene::PauseAudio()
{
    if (AudioManager::Instance().IsInitialized())
    {
        AudioManager::Instance().PauseMusic();
    }
}
