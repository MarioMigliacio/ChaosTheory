// ============================================================================
//  File        : GameScene.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-14
//  Description : Hosts the definitions for Game Scene Object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "GameScene.h"
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

GameScene::GameScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

// Initializes the GameScene.
void GameScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    UIManager::Instance().Clear();

    LoadRequiredAssets();
    SetupSceneComponents();

    SceneTransitionManager::Instance().StartFadeIn();
    m_isInitialized = true;

    CT_LOG_INFO("GameScene initialized.");
}

void GameScene::LoadRequiredAssets()
{
    for (const auto &[key, path] : UIAssets::Textures)
    {
        if (!AssetManager::Instance().LoadTexture(key, path))
        {
            CT_LOG_ERROR("GameScene failed to load texture asset: {} -> {}", key, path);
        }
    }

    for (const auto &[key, path] : GameAssets::Textures)
    {
        if (!AssetManager::Instance().LoadTexture(key, path))
        {
            CT_LOG_ERROR("GameScene failed to load texture asset: {} -> {}", key, path);
        }
    }

    for (const auto &[key, path] : GameAssets::Fonts)
    {
        if (!AssetManager::Instance().LoadFont(key, path))
        {
            CT_LOG_ERROR("GameScene failed to load font asset: {} -> {}", key, path);
        }
    }

    CT_LOG_INFO("GameScene finished LoadRequiredAssets.");
}

// Shuts down this scene and resets internal state.
void GameScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("GameScene", "Shutdown");

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("GameScene shutdown.");
}

// Handles the exit criteria for this scene.
void GameScene::OnExit()
{
    CT_LOG_INFO("GameScene OnExit.");
}

// Performs internal state management during a single frame.
void GameScene::Update(float dt)
{
    const auto mousePos = InputManager::Instance().GetMousePosition();
    const bool isPressed = InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left);
    const bool isJustPressed = InputManager::Instance().IsMouseButtonJustPressed(sf::Mouse::Left);

    for (auto ele : UIManager::Instance().GetElements())
    {
        ele->SetEnabled(m_toggler);
    }

    UIManager::Instance().Update(mousePos, isPressed, isJustPressed, dt);

    if (m_background)
    {
        m_background->Update(dt);
    }

    // Handle button scene request change
    if (m_hasPendingTransition)
    {
        CT_LOG_INFO("GameScene Requesting Scene Change to '{}'", SceneIDToString(m_requestedScene));
        m_hasPendingTransition = false;
        SceneTransitionManager::Instance().ForceFullyOpaque();
        SceneManager::Instance().RequestSceneChange(m_requestedScene);
    }
}

// Handle any internal logic that should be done relevant to this scene.
void GameScene::HandleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Enter)
        {
            m_hasPendingTransition = true;
            m_requestedScene = SceneID::MainMenu;

            CT_LOG_INFO("GameScene: Enter event handled.");
        }

        else if (event.key.code == sf::Keyboard::Space)
        {
            m_toggler = !m_toggler;

            CT_LOG_INFO("GameScene: Toggle switch: {}", m_toggler);
        }
    }
}

/// @brief Not used in GameSceneScene context.
/// @param newSize bubbled down from caller, not needed.
void GameScene::OnResize(const sf::Vector2u &newSize)
{
}

// While this scene is active, render the necessary components to the Game Scene.
void GameScene::Render()
{
    auto &window = WindowManager::Instance().GetWindow();
    window.clear();

    if (m_background)
    {
        m_background->Draw(window);
    }

    UIManager::Instance().Render(window);
}

void GameScene::SetupSceneComponents()
{
    LoadBackground();
    CreateTitleText();
    CreateButtons();
    PlayGameMusic();
}

void GameScene::LoadBackground()
{
    m_background = std::make_unique<Background>();
    m_background->InitParallax({{"GasPattern1", 2.f}, {"PlainStarBackground", 1.f}});

    m_background->SetLayerMotion("PlainStarBackground", {0.2f, -0.5f});
    m_background->SetLayerMotion("GasPattern1", {0.f, -0.1f});
}

void GameScene::CreateTitleText()
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const std::string title = "Sandbox Scene";
    const unsigned int fontSize = scaleMgr.ScaleFont(48);
    const sf::Vector2f centerPos = {WindowManager::Instance().GetWindow().getSize().x / 2.f,
                                    scaleMgr.ScaledReferenceY(0.08f)};

    m_titleLabel = UIFactory::Instance().CreateTextLabel(title, centerPos, fontSize, true);
    UIManager::Instance().AddElement(m_titleLabel);
}

void GameScene::CreateButtons()
{
    const float buttonWidth = BASE_BUTTON_WIDTH_PIXEL;
    const float buttonHeight = BASE_BUTTON_HEIGHT_PIXEL;
    const float spacing = BASE_BUTTON_SPACING_PIXEL;

    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    const float totalWidth = (buttonWidth * 3.f) + (spacing * 2.f);
    const float startX = (winSize.x - totalWidth) / 2.f;
    const float centerY = winSize.y * 0.55f; // slightly lower than center

    const sf::Vector2f size{buttonWidth, buttonHeight};

    std::vector<std::tuple<std::string, std::string>> skins = {
        {UIAssets::UISkinButtonBlueIdleKey, UIAssets::UISkinButtonBlueHoverKey},
        {UIAssets::UISkinButtonGreenIdleKey, UIAssets::UISkinButtonGreenHoverKey},
        {UIAssets::UISkinButtonRedIdleKey, UIAssets::UISkinButtonRedHoverKey}};

    for (size_t i = 0; i < skins.size(); ++i)
    {
        const float x = startX + i * (size.x + spacing);

        auto button = UIFactory::Instance().CreateSkinnableButton({x, centerY}, size, std::get<0>(skins[i]),
                                                                  std::get<1>(skins[i]), [i]()
                                                                  { CT_LOG_INFO("Skinnable button {} clicked!", i); });

        UIManager::Instance().AddElement(button);
    }
}

void GameScene::PlayGameMusic()
{
}
