// ============================================================================
//  File        : MainMenuScene.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-13
//  Description : Hosts the definitions for Main Menu Scene Object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "MainMenuScene.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "Macros.h"
#include "MainMenuAssets.h"
#include "ResolutionScaleManager.h"
#include "SceneManager.h"
#include "SceneTransitionManager.h"
#include "UIFactory.h"
#include "UIManager.h"
#include "WindowManager.h"

/// @brief Constructor for the MainMenuScene.
/// @param settings Internal settings to initialize with.
MainMenuScene::MainMenuScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

/// @brief  Initializes the MainMenuScene.
void MainMenuScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    auto &window = WindowManager::Instance().GetWindow();
    auto desiredSetting = m_settings->m_resolution;
    auto desiredSize = WindowManager::Instance().GetResolutionSize(desiredSetting);
    auto currentSize = window.getSize();

    // Only re-apply resolution if needed
    if (currentSize != desiredSize)
    {
        WindowManager::Instance().ApplyResolution(desiredSetting);
    }

    UIManager::Instance().Clear();
    SceneTransitionManager::Instance().StartFadeIn();

    LoadRequiredAssets();
    SetupSceneComponents();

    m_isInitialized = true;

    CT_LOG_INFO("MainMenuScene initialized.");
}

/// @brief Load any required assets listed in the MainMenuAssets namespace.
void MainMenuScene::LoadRequiredAssets()
{
    auto &assets = AssetManager::Instance();

    for (const auto &[key, path] : MainMenuAssets::Textures)
    {
        if (!assets.LoadTexture(key, path))
        {
            CT_LOG_ERROR("MainMenuScene::LoadRequiredAssets::LoadTexture failed to load Asset: {}, {}", key, path);
        }
    }

    for (const auto &[key, path] : MainMenuAssets::Fonts)
    {
        if (!assets.LoadFont(key, path))
        {
            CT_LOG_ERROR("MainMenuScene::LoadRequiredAssets::LoadFont failed to load Asset: {}, {}", key, path);
        }
    }

    CT_LOG_INFO("MainMenuScene finished LoadRequiredAssets.");
}

/// @brief Shuts down this scene and resets internal state.
void MainMenuScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("MainMenuScene", "Shutdown");

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("MainMenuScene Shutdown.");
}

/// @brief Handles the exit criteria for this scene.
void MainMenuScene::OnExit()
{
    if (AudioManager::Instance().IsInitialized())
    {
        AudioManager::Instance().StopMusic();
    }

    CT_LOG_INFO("MainMenuScene OnExit.");
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update.
void MainMenuScene::Update(float dt)
{
    const auto mousePos = InputManager::Instance().GetMousePosition();
    const bool isPressed = InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left);
    const bool isJustPressed = InputManager::Instance().IsMouseButtonJustPressed(sf::Mouse::Left);

    UIManager::Instance().Update(mousePos, isPressed, isJustPressed, dt);

    // Handle button scene request change
    if (m_hasPendingTransition)
    {
        CT_LOG_INFO("MainMenuScene Requesting Scene Change to '{}'", ToString(m_requestedScene));
        m_hasPendingTransition = false;
        SceneTransitionManager::Instance().ForceFullyOpaque();
        SceneManager::Instance().RequestSceneChange(m_requestedScene);
    }

    // Handle exit request from the scene
    else if (m_shouldExit)
    {
        CT_LOG_INFO("MainMenuScene requested exit. Popping scene...");
        SceneManager::Instance().PopScene();
    }

    return;
}

/// @brief Not used in MainMenuScene context.
/// @param event bubbled down from caller, not needed.
void MainMenuScene::HandleEvent(const sf::Event &event)
{
}

/// @brief Not used in MainMenuScene context.
/// @param newSize bubbled down from caller, not needed.
void MainMenuScene::OnResize(const sf::Vector2u &newSize)
{
}

/// @brief While this scene is active, render the necessary components to the Main Menu Scene.
void MainMenuScene::Render()
{
    CT_WARN_IF_UNINITIALIZED("MainMenuScene", "Render");

    auto &window = WindowManager::Instance().GetWindow();
    window.clear();

    if (m_backgroundSprite)
    {
        window.draw(*m_backgroundSprite);
    }

    UIManager::Instance().Render(window);
}

/// @brief Helper method to clear up clutter from main Init.
void MainMenuScene::SetupSceneComponents()
{
    CreateTitleText();
    CreateButtons();
    LoadBackground();
    PlayIntroMusic();
}

/// @brief Assists with the loading of the TitleText for this MainMenuScene.
void MainMenuScene::CreateTitleText()
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const std::string titleText = DEFAULT_TITLE_STR;
    const unsigned int fontSize = scaleMgr.ScaleFont(DEFAULT_TITLE_FONT_SIZE);
    const sf::Vector2f centerPos = {WindowManager::Instance().GetWindow().getSize().x / 2.f,
                                    scaleMgr.ScaledReferenceY(DEFAULT_TITLE_HEIGHT_PERCENT)};

    m_titleLabel = UIFactory::Instance().CreateTextLabel(titleText, centerPos, fontSize, true);
    m_titleLabel->SetColor(DEFAULT_TITLE_COLOR);
    UIManager::Instance().AddElement(m_titleLabel);
}

/// @brief Assists with creating the Buttons for this MainMenuScene.
void MainMenuScene::CreateButtons()
{
    const auto winSize = WindowManager::Instance().GetWindow().getSize();

    const float scaledButtonWidth = ResolutionScaleManager::Instance().ScaleX(MAIN_MENU_BUTTON_WIDTH_PIXEL);
    const float scaledButtonHeight = ResolutionScaleManager::Instance().ScaleY(MAIN_MENU_BASE_BUTTON_HEIGHT_PIXEL);
    const float scaledSpacing = scaledButtonHeight * BASE_BUTTON_SPACING_PERCENT;
    const float startY = winSize.y * 0.7f;
    const float centerX = (winSize.x - scaledButtonWidth) / 2.f;

    const sf::Vector2f playPos{centerX, startY};
    const sf::Vector2f settingsPos{centerX, playPos.y + scaledButtonHeight + scaledSpacing};
    const sf::Vector2f exitPos{centerX, settingsPos.y + scaledButtonHeight + scaledSpacing};

    UIManager::Instance().AddElement(MakeMenuButton(ButtonType::Classic, playPos, "Play",
                                                    [this]()
                                                    {
                                                        CT_LOG_INFO("Play button clicked!");
                                                        m_hasPendingTransition = true;
                                                        m_requestedScene = SceneID::Game;
                                                    }));

    UIManager::Instance().AddElement(MakeMenuButton(ButtonType::Classic, settingsPos, "Settings",
                                                    [this]()
                                                    {
                                                        CT_LOG_INFO("Settings button clicked!");
                                                        m_hasPendingTransition = true;
                                                        m_requestedScene = SceneID::Settings;
                                                    }));

    UIManager::Instance().AddElement(MakeMenuButton(ButtonType::Classic, exitPos, "Exit",
                                                    [this]()
                                                    {
                                                        CT_LOG_INFO("Exit button clicked!");
                                                        m_shouldExit = true;
                                                    }));
}

/// @brief Loads the main background image for this MainMenuScene.
void MainMenuScene::LoadBackground()
{
    sf::Texture &bgTexture = AssetManager::Instance().GetTexture(MainMenuAssets::MenuBackground);
    m_backgroundSprite = std::make_unique<sf::Sprite>(bgTexture);

    // Scale to window size
    const auto windowSize = WindowManager::Instance().GetWindow().getSize();
    const auto texSize = bgTexture.getSize();

    const float scaleX = static_cast<float>(windowSize.x) / texSize.x;
    const float scaleY = static_cast<float>(windowSize.y) / texSize.y;

    m_backgroundSprite->setScale(scaleX, scaleY);
    m_backgroundSprite->setPosition(0.f, 0.f);

    CT_LOG_INFO("Menu background loaded and scaled.");
}

/// @brief Plays the background music for this MainMenuScene.
void MainMenuScene::PlayIntroMusic()
{
    if (!AudioManager::Instance().IsMusicPlaying() ||
        AudioManager::Instance().GetCurrentMusicName() != MainMenuAssets::MenuSong)
    {
        CT_LOG_INFO("MainMenuScene: Starting or resuming menu music.");
        AudioManager::Instance().PlayMusic(MainMenuAssets::MenuSong, true);
    }

    else
    {
        CT_LOG_INFO("MainMenuScene: Menu music already playing, no action needed.");
    }
}

/// @brief simplistic helper method to help make a menu button.
/// @param type Which type of Button requested.
/// @param pos Vector2f position location.
/// @param label String used for setting button text.
/// @param onClick Callback function behavior when button clicked.
/// @return A shared pointer to a UIElement for the requested button.
std::shared_ptr<UIElement> MainMenuScene::MakeMenuButton(ButtonType type, const sf::Vector2f &pos,
                                                         const std::string &label, std::function<void()> onClick)
{
    return UIFactory::Instance().CreateButton(
        type, pos, {MAIN_MENU_BUTTON_WIDTH_PIXEL, MAIN_MENU_BASE_BUTTON_HEIGHT_PIXEL}, label, std::move(onClick));
}