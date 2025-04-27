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
#include "GameScene.h"
#include "InputManager.h"
#include "Macros.h"
#include "SceneFactory.h"
#include "SceneManager.h"
#include "SceneTransitionManager.h"
#include "SettingsScene.h"
#include "UIFactory.h"
#include "UIManager.h"
#include "WindowManager.h"

// Provides adjustable constants
namespace
{
constexpr float BUTTON_WIDTH = 180.f;
constexpr float BUTTON_HEIGHT = 48.f;
constexpr float BUTTON_SPACING = 20.f;
constexpr float TITLE_Y_RATIO = 0.15f;
constexpr unsigned int DEFAULT_TITLE_FONT_SIZE = 64;
} // namespace

MainMenuScene::MainMenuScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

// Initializes the MainMenuScene.
void MainMenuScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    // New: MainMenuScene is designed to recreate the window using the settings, because SplashScene locked it.
    WindowManager::Instance().Recreate(m_settings->m_windowWidth, m_settings->m_windowHeight, m_settings->m_windowTitle,
                                       sf::Style::Default);

    UIManager::Instance().Clear();
    SceneTransitionManager::Instance().StartFadeIn();

    SetupSceneAssets();
    PlayIntroMusic();

    m_isInitialized = true;

    CT_LOG_INFO("MainMenuScene initialized.");
}

// Shuts down this scene and resets internal state.
void MainMenuScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("MainMenuScene", "Shutdown");

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("MainMenuScene Shutdown.");
}

// Handles the exit criteria for this scene.
void MainMenuScene::OnExit()
{
    if (AudioManager::Instance().IsInitialized())
    {
        AudioManager::Instance().StopMusic();
    }

    CT_LOG_INFO("MainMenuScene OnExit.");
}

// Performs internal state management during a single frame.
void MainMenuScene::Update(float dt)
{
    const auto mousePos = InputManager::Instance().GetMousePosition();
    const bool isPressed = InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left);

    UIManager::Instance().Update(mousePos, isPressed);

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

// Handle any internal logic that should be done relevant to this scene.
void MainMenuScene::HandleEvent(const sf::Event &event)
{
    // Only if you want to catch window resize, close, etc.
}

// Adjust the current scene entities size and positioning.
void MainMenuScene::OnResize(const sf::Vector2u &newSize)
{
    CT_LOG_INFO("MainMenuScene OnResize: new size = {}x{}", newSize.x, newSize.y);

    // Adjust background
    if (m_backgroundSprite && m_backgroundSprite->getTexture())
    {
        sf::Vector2u textureSize = m_backgroundSprite->getTexture()->getSize();

        sf::Vector2f scale(static_cast<float>(newSize.x) / textureSize.x,
                           static_cast<float>(newSize.y) / textureSize.y);

        m_backgroundSprite->setScale(scale);
        m_backgroundSprite->setPosition(0.f, 0.f);

        CT_LOG_INFO("Resized background sprite to window size: {}x{}", newSize.x, newSize.y);
    }

    // Adjust title
    if (!m_title.getString().isEmpty())
    {
        // Optionally resize the title font based on window height
        const unsigned int fontSize = std::max(24u, newSize.y / 15); // prevent going too small
        m_title.setCharacterSize(fontSize);

        // Re-center the title horizontally
        const auto bounds = m_title.getLocalBounds();
        m_title.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        m_title.setPosition(static_cast<float>(newSize.x) / 2.f, static_cast<float>(newSize.y) * TITLE_Y_RATIO);
    }

    // Adjust UI elements
    UIManager::Instance().Clear();
    CreateButtons();
}

// While this scene is active, render the necessary components to the Main Menu Scene.
void MainMenuScene::Render()
{
    CT_WARN_IF_UNINITIALIZED("MainMenuScene", "Render");

    auto &window = WindowManager::Instance().GetWindow();
    window.clear();

    if (m_backgroundSprite)
    {
        window.draw(*m_backgroundSprite);
    }

    window.draw(m_title);
    UIManager::Instance().Render(window);
}

// Helper method to clear up clutter from main Init.
void MainMenuScene::SetupSceneAssets()
{
    LoadBackground();
    CreateTitleText();
    CreateButtons();
}

// Assists with the loading of the TitleText for this MainMenuScene.
void MainMenuScene::CreateTitleText()
{
    m_title.setFont(AssetManager::Instance().GetFont("Default.ttf"));
    m_title.setString("Chaos Theory");
    m_title.setCharacterSize(DEFAULT_TITLE_FONT_SIZE); // Bigger for drama
    m_title.setFillColor(sf::Color(102, 255, 102));    // Alien green
    m_title.setOutlineColor(sf::Color::Black);
    m_title.setOutlineThickness(3.f);

    const auto bounds = m_title.getLocalBounds();
    m_title.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);

    const auto windowSize = WindowManager::Instance().GetWindow().getSize();
    m_title.setPosition(windowSize.x / 2.f, windowSize.y * TITLE_Y_RATIO);
}

// Assists with creating the Buttons for this MainMenuScene.
void MainMenuScene::CreateButtons()
{
    auto &window = WindowManager::Instance().GetWindow();
    const auto windowSize = window.getSize();

    const sf::Vector2f playPos{(windowSize.x - BUTTON_WIDTH) / 2.f, windowSize.y * 0.70f};
    const sf::Vector2f settingsPos{playPos.x, playPos.y + BUTTON_HEIGHT + BUTTON_SPACING};
    const sf::Vector2f exitPos{settingsPos.x, settingsPos.y + BUTTON_HEIGHT + BUTTON_SPACING};

    // Play Button
    UIManager::Instance().AddElement(MakeMenuButton(ButtonType::Classic, playPos, "Play",
                                                    [this]()
                                                    {
                                                        CT_LOG_INFO("Play button clicked!");
                                                        m_hasPendingTransition = true;
                                                        m_requestedScene = SceneID::Game;
                                                    }));

    // Settings Button
    UIManager::Instance().AddElement(MakeMenuButton(ButtonType::Classic, settingsPos, "Settings",
                                                    [this]()
                                                    {
                                                        CT_LOG_INFO("Settings button clicked!");
                                                        m_hasPendingTransition = true;
                                                        m_requestedScene = SceneID::Settings;
                                                    }));

    // Exit Button
    UIManager::Instance().AddElement(MakeMenuButton(ButtonType::Classic, exitPos, "Exit",
                                                    [this]()
                                                    {
                                                        CT_LOG_INFO("Exit button clicked!");
                                                        m_shouldExit = true;
                                                    }));
}

// Loads the main background image for this MainMenuScene.
void MainMenuScene::LoadBackground()
{
    if (!AssetManager::Instance().LoadTexture("menu_bg", "assets/sprites/TitleBackground.png"))
    {
        CT_LOG_WARN("Failed to load menu background. Using default.");
        m_backgroundSprite = std::make_unique<sf::Sprite>(AssetManager::Instance().GetTexture("Default"));

        return;
    }

    sf::Texture &bgTexture = AssetManager::Instance().GetTexture("menu_bg");
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

// Plays the background music for this MainMenuScene.
void MainMenuScene::PlayIntroMusic()
{
    const std::string menuMusic = m_settings->m_audioDirectory + "RootMenu.wav";

    if (!AudioManager::Instance().IsMusicPlaying() || AudioManager::Instance().GetCurrentMusicName() != menuMusic)
    {
        CT_LOG_INFO("MainMenuScene: Starting or resuming menu music.");
        AudioManager::Instance().PlayMusic(menuMusic, true);
    }

    else
    {
        CT_LOG_INFO("MainMenuScene: Menu music already playing, no action needed.");
    }
}

std::shared_ptr<UIElement> MainMenuScene::MakeMenuButton(ButtonType type, const sf::Vector2f &pos,
                                                         const std::string &label, std::function<void()> onClick)
{
    return UIFactory::Instance().CreateButton(type, pos, {BUTTON_WIDTH, BUTTON_HEIGHT}, label, std::move(onClick));
}