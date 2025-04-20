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
#include "InputManager.h"
#include "Macros.h"
#include "MainMenuScene.h"
#include "SceneFactory.h"
#include "WindowManager.h"

GameScene::GameScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

// Initializes the GameScene.
void GameScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    // Load assets, music, and scene-specific setup
    AudioManager::Instance().SetVolume(50.f);
    AudioManager::Instance().PlayMusic(m_settings->m_audioDirectory + "Gametrack.wav", true);
    InputManager::Instance().BindKey("MenuSelectBack", m_settings->m_keyBindings["MenuSelectBack"]);

    m_isInitialized = true;
    CT_LOG_INFO("GameScene initialized.");
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
    if (AudioManager::Instance().IsInitialized())
    {
        AudioManager::Instance().StopMusic();
    }

    if (AssetManager::Instance().IsInitialized())
    {
        InputManager::Instance().UnbindKey("MenuSelectBack");
    }

    CT_LOG_INFO("GameScene OnExit.");
}

// Performs internal state management during a single frame.
void GameScene::Update(float dt)
{
    if (InputManager::Instance().IsJustReleased("MenuSelectBack"))
    {
        AudioManager::Instance().StopMusic(true, 1.0f);
        m_shouldExit = true;

        CT_LOG_INFO("GameScene Transition triggered.");
    }

    if (m_shouldExit)
    {
        if (!AudioManager::Instance().IsFadingOut())
        {
            CT_LOG_INFO("SceneChangeCallback 1/2 invoked from GameScene.");
            if (m_sceneChangeCallback)
            {
                m_sceneChangeCallback(SceneFactory::Instance().Create("MainMenu"));

                CT_LOG_INFO("SceneChangeCallback 2/2 invoked from GameScene - and callback .");
            }
        }
    }

    return;
}

// Handle any internal logic that should be done relevant to this scene.
void GameScene::HandleEvent(const sf::Event &event)
{
    // Only if you want to catch window resize, close, etc.
}

void GameScene::OnResize(const sf::Vector2u &newSize)
{
    // intentionally blank for now.
}

// While this scene is active, render the necessary components to the Game Scene.
void GameScene::Render()
{
    CT_WARN_IF_UNINITIALIZED("GameScene", "Render");

    sf::RenderWindow &window = WindowManager::Instance().GetWindow();

    sf::Text text;
    text.setString("Game Scene - Press [Space] to return to Menu");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Green);
    text.setFont(AssetManager::Instance().GetFont("default"));
    text.setPosition(80.f, 80.f);

    window.draw(text);
}

// Callback to determine logic for the next action scene to take place.
void GameScene::SetSceneChangeCallback(SceneChangeCallback callback)
{
    m_sceneChangeCallback = std::move(callback);
}
