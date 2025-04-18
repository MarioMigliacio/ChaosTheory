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
#include "WindowManager.h"

MainMenuScene::MainMenuScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

// Initializes the MainMenuScene.
void MainMenuScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    AudioManager::Instance().PlayMusic(m_settings->m_audioDirectory + "RootMenu_clean.wav", true);
    InputManager::Instance().BindKey("MenuSelectNext", m_settings->m_keyBindings["MenuSelectNext"]);

    m_button = std::make_unique<Button>(sf::Vector2f(100, 200), sf::Vector2f(200, 60));
    m_button->SetText("Start", AssetManager::Instance().GetFont("default"));
    m_button->SetCallback(
        [this]()
        {
            CT_LOG_INFO("Start button pressed. Transitioning...");
            m_shouldExit = true;

            if (m_sceneChangeCallback)
            {
                m_sceneChangeCallback(SceneFactory::Instance().Create("Game"));
            }
        });

    m_isInitialized = true;

    CT_LOG_INFO("MainMenuScene initialized.");
}

// Shuts down this scene and resets internal state.
void MainMenuScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("MainMenuScene", "Shutdown");

    m_settings.reset();
    m_button.reset();
    m_isInitialized = false;

    CT_LOG_INFO("MainMenuScene shutdown.");
}

// Handles the exit criteria for this scene.
void MainMenuScene::OnExit()
{
    AudioManager::Instance().StopMusic();
    InputManager::Instance().UnbindKey("MenuSelectNext");

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
    const bool isLeftClick = InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left);

    m_button->Update(mousePos, isLeftClick);

    // if (InputManager::Instance().IsJustReleased("MenuSelectNext"))
    // {
    //     m_shouldExit = true;
    //     AudioManager::Instance().StopMusic(true, 1.0f);

    //     CT_LOG_INFO("MainMenuScene Transition triggered.");
    // }

    // if (m_shouldExit)
    // {
    //     if (!AudioManager::Instance().IsFadingOut())
    //     {
    //         CT_LOG_INFO("SceneChangeCallback 1/2 invoked from MainMenuScene.");
    //         if (m_sceneChangeCallback)
    //         {
    //             m_sceneChangeCallback(SceneFactory::Instance().Create("Game"));

    //             CT_LOG_INFO("SceneChangeCallback 2/2 invoked from MainMenuScene - and callback .");
    //         }
    //     }
    // }

    return;
}

// Handle any internal logic that should be done relevant to this scene.
void MainMenuScene::HandleEvent(const sf::Event &event)
{
    // Only if you want to catch window resize, close, etc.
}

// While this scene is active, render the necessary components to the Main Menu Scene.
void MainMenuScene::Render()
{
    CT_WARN_IF_UNINITIALIZED("MainMenuScene", "Render");

    sf::RenderWindow &window = WindowManager::Instance().GetWindow();

    sf::Text title;
    title.setString("Chaos Theory");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::White);
    title.setFont(AssetManager::Instance().GetFont("default"));
    title.setPosition(100.f, 100.f);

    window.draw(title);
    window.draw(*m_button);
}

// Callback to determine logic for the next action scene to take place.
void MainMenuScene::SetSceneChangeCallback(SceneChangeCallback callback)
{
    m_sceneChangeCallback = std::move(callback);
}

// Returns whether or not the MainMenuScene should exit.
bool MainMenuScene::ShouldExit()
{
    return m_shouldExit;
}
