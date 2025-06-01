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
#include "AudioManager.h"
#include "GameAssets.h"
#include "InputManager.h"
#include "Macros.h"
#include "MainMenuScene.h"
#include "ResolutionScaleManager.h"
#include "SceneFactory.h"
#include "SceneTransitionManager.h"
#include "UIAssets.h"
#include "UIFactory.h"
#include "UIManager.h"
#include "UIPresets.h"
#include "WindowManager.h"

namespace
{
/// @brief Fixed name constant for the play button label.
constexpr auto PLAY_BTN_LABEL = "Play";

/// @brief Fixed name constant for the Settings button label.
constexpr auto SETTING_BTN_LABEL = "Settings";

/// @brief Fixed name constant for the Quit button label.
constexpr auto QUIT_BTN_LABEL = "Quit";

constexpr auto TOGGLE_BUTTON_KEY = "Toggle";

constexpr auto RETURN_BUTTON_KEY = "Return";
} // namespace

/// @brief Constructor for the SandBoxScene.
/// @param settings Internal settings to initialize with.
SandBoxScene::SandBoxScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

/// @brief  Initializes the SandBoxScene.
void SandBoxScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    UIManager::Instance().Clear();

    LoadRequiredAssets();
    BindInputKeys();
    SetupSceneComponents();

    SceneTransitionManager::Instance().StartFadeIn();
    m_isInitialized = true;

    CT_LOG_INFO("SandBoxScene initialized.");
}

/// @brief Load any required assets relevant to the SandBoxScene.
void SandBoxScene::LoadRequiredAssets()
{
    for (const auto &[key, path] : UIAssets::Textures)
    {
        if (!AssetManager::Instance().LoadTexture(key, path))
        {
            CT_LOG_ERROR("SandBoxScene failed to load texture asset: {} -> {}", key, path);
        }
    }

    for (const auto &[key, path] : GameAssets::Textures)
    {
        if (!AssetManager::Instance().LoadTexture(key, path))
        {
            CT_LOG_ERROR("SandBoxScene failed to load texture asset: {} -> {}", key, path);
        }
    }

    for (const auto &[key, path] : GameAssets::Fonts)
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
    InputManager::Instance().UnbindKey(TOGGLE_BUTTON_KEY);
    InputManager::Instance().UnbindKey(RETURN_BUTTON_KEY);

    CT_LOG_INFO("SandBoxScene OnExit.");
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

    // new
    CheckActionsPressed();

    // Handle button scene request change
    if (m_hasPendingTransition)
    {
        CT_LOG_INFO("SandBoxScene Requesting Scene Change to '{}'", SceneIDToString(m_requestedScene));
        m_hasPendingTransition = false;
        SceneTransitionManager::Instance().ForceFullyOpaque();
        SceneManager::Instance().RequestSceneChange(m_requestedScene);
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
    LoadBackground();
    CreateTitleText();
    PlayGameMusic();
}

/// @brief Helper method to load the Background for this Scene.
void SandBoxScene::LoadBackground()
{
    m_background = std::make_unique<Background>();
    m_background->InitParallax({{"GasPattern1", 2.f}, {"PlainStarBackground", 1.f}});

    m_background->SetLayerMotion("PlainStarBackground", {0.2f, -0.5f});
    m_background->SetLayerMotion("GasPattern1", {0.f, -0.1f});
}

/// @brief Helper method to create the Title string entity for this scene.
void SandBoxScene::CreateTitleText()
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const std::string title = "Sandbox Scene";
    const unsigned int fontSize = scaleMgr.ScaleFont(48);
    const sf::Vector2f centerPos = {WindowManager::Instance().GetWindow().getSize().x / 2.f,
                                    scaleMgr.ScaledReferenceY(0.08f)};

    m_titleLabel = UIFactory::Instance().CreateTextLabel(title, centerPos, fontSize, true);
    UIManager::Instance().AddElement(m_titleLabel);
}

/// @brief Helper method to load and play the game music for this scene. (intentionally blank for now)
void SandBoxScene::PlayGameMusic()
{
}

void SandBoxScene::BindInputKeys()
{
    InputManager::Instance().BindKey(TOGGLE_BUTTON_KEY, sf::Keyboard::Key::Space);
    InputManager::Instance().BindKey(RETURN_BUTTON_KEY, sf::Keyboard::Key::Enter);
}

void SandBoxScene::CheckActionsPressed()
{
    auto &input = InputManager::Instance();

    if (input.IsKeyJustPressed(TOGGLE_BUTTON_KEY))
    {
        m_toggler = !m_toggler;

        CT_LOG_INFO("SandBoxScene: Toggle switch: {}", m_toggler);
    }

    if (input.IsKeyJustPressed(RETURN_BUTTON_KEY))
    {
        m_hasPendingTransition = true;
        m_requestedScene = SceneID::MainMenu;

        CT_LOG_INFO("SandBoxScene: Enter event handled.");
    }
}
