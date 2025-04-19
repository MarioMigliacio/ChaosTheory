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
#include "UIFactory.h"
#include "UIManager.h"
#include "WindowManager.h"

MainMenuScene::MainMenuScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

// Initializes the MainMenuScene.
void MainMenuScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    CreateTitleText();
    CreateButtons();
    LoadBackground();
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

    CT_LOG_INFO("MainMenuScene shutdown.");
}

// Returns whether or not the MainMenuScene should exit.
bool MainMenuScene::ShouldExit()
{
    return m_shouldExit;
}

// Handles the exit criteria for this scene.
void MainMenuScene::OnExit()
{
    if (AudioManager::Instance().IsInitialized())
    {
        AudioManager::Instance().StopMusic();
    }

    if (AssetManager::Instance().IsInitialized())
    {
        InputManager::Instance().UnbindKey("MenuSelectNext");
    }

    CT_LOG_INFO("MainMenuScene OnExit.");
}

// Returns whether or not this scene has been initialized.
bool MainMenuScene::IsInitialized()
{
    return m_isInitialized;
}

// Performs internal state management during a single frame.
void MainMenuScene::Update(float dt)
{
    const auto mousePos = InputManager::Instance().GetMousePosition();
    const bool isPressed = InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left);

    UIManager::Instance().Update(mousePos, isPressed);

    // Handle exit request from the scene
    if (m_shouldExit && m_sceneChangeCallback)
    {
        CT_LOG_INFO("MainMenuScene requested exit.");
        m_sceneChangeCallback(nullptr); // or call SceneManager::PopScene() logic depending on your flow
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
        unsigned int newFontSize = std::max(24u, newSize.y / 15); // prevent going too small
        m_title.setCharacterSize(newFontSize);

        // Re-center the title horizontally
        sf::FloatRect bounds = m_title.getLocalBounds();
        m_title.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        m_title.setPosition(static_cast<float>(newSize.x) / 2.f, static_cast<float>(newSize.y) * 0.15f);
    }

    // Adjust buttons
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

// Callback to determine logic for the next action scene to take place.
void MainMenuScene::SetSceneChangeCallback(SceneChangeCallback callback)
{
    m_sceneChangeCallback = std::move(callback);
}

// Assists with the loading of the TitleText for this MainMenuScene.
void MainMenuScene::CreateTitleText()
{
    m_title.setFont(AssetManager::Instance().GetFont("default"));
    m_title.setString("Chaos Theory");
    m_title.setCharacterSize(64);                   // Bigger for drama
    m_title.setFillColor(sf::Color(102, 255, 102)); // Alien green
    m_title.setOutlineColor(sf::Color::Black);
    m_title.setOutlineThickness(3.f);

    auto bounds = m_title.getLocalBounds();
    m_title.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    auto windowSize = WindowManager::Instance().GetWindow().getSize();
    m_title.setPosition(windowSize.x / 2.f, windowSize.y * 0.15f);
}

// Assists with creating the Buttons for this MainMenuScene.
void MainMenuScene::CreateButtons()
{
    auto &window = WindowManager::Instance().GetWindow();
    auto font = AssetManager::Instance().GetFont("default");
    sf::Vector2u windowSize = window.getSize();

    sf::Vector2f buttonSize{180.f, 48.f};
    float spacing = 20.f;

    sf::Vector2f playPos{(windowSize.x - buttonSize.x) / 2.f, windowSize.y * 0.75f};
    sf::Vector2f exitPos{playPos.x, playPos.y + buttonSize.y + spacing};

    // Classic Play Button
    auto playBtn =
        UIFactory::Instance().CreateButton(ButtonType::Classic, playPos, buttonSize, "Play",
                                           [this]()
                                           {
                                               CT_LOG_INFO("Play button clicked!");
                                               if (m_sceneChangeCallback)
                                               {
                                                   m_sceneChangeCallback(std::make_unique<GameScene>(m_settings));
                                               }
                                           });

    // Rounded Exit Button
    auto exitBtn = UIFactory::Instance().CreateButton(ButtonType::Rounded, exitPos, buttonSize, "Exit",
                                                      [this]()
                                                      {
                                                          CT_LOG_INFO("Exit button clicked!");
                                                          m_shouldExit = true;
                                                      });

    UIManager::Instance().AddElement(playBtn);
    UIManager::Instance().AddElement(exitBtn);
}

// Loads the main background image for this MainMenuScene.
void MainMenuScene::LoadBackground()
{
    if (!AssetManager::Instance().LoadTexture("menu_bg", "assets/sprites/TitleBackground.png"))
    {
        CT_LOG_WARN("Failed to load menu background.");

        return;
    }

    sf::Texture &bgTexture = AssetManager::Instance().GetTexture("menu_bg");

    m_backgroundSprite = std::make_unique<sf::Sprite>(bgTexture);

    // Scale to window size
    sf::Vector2u windowSize = WindowManager::Instance().GetWindow().getSize();
    sf::Vector2u textureSize = bgTexture.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    m_backgroundSprite->setScale(scaleX, scaleY);
    m_backgroundSprite->setPosition(0.f, 0.f);

    CT_LOG_INFO("Menu background loaded and scaled.");
}

// Plays the background music for this MainMenuScene.
void MainMenuScene::PlayIntroMusic()
{
    AudioManager::Instance().PlayMusic(m_settings->m_audioDirectory + "RootMenu_clean.wav", true);
}
