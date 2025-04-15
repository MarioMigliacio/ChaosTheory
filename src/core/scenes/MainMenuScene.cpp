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
#include "Macros.h"
#include "SceneFactory.h"
#include "SceneManager.h"
#include "WindowManager.h"

MainMenuScene::MainMenuScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

void MainMenuScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    AudioManager::Instance().PlayMusic(m_settings->m_audioDirectory + "RootMenu_clean.wav", true, true);

    m_isInitialized = true;

    CT_LOG_INFO("MainMenuScene initialized.");
}

void MainMenuScene::Update(float dt)
{
    if (m_shouldExit)
    {
        if (!AudioManager::Instance().IsFading())
        {
            CT_LOG_INFO("SceneChangeCallback 1/2 invoked from MainMenuScene.");
            if (m_sceneChangeCallback)
            {
                m_sceneChangeCallback(SceneFactory::Instance().Create("Game"));

                CT_LOG_INFO("SceneChangeCallback 2/2 invoked from MainMenuScene - and callback .");
            }
        }

        return;
    }
}

void MainMenuScene::HandleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Enter)
        {
            AudioManager::Instance().StopMusic(true, 1.0f);
            m_shouldExit = true;

            CT_LOG_INFO("MainMenuScene Transition triggered.");
        }
    }
}

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
}

void MainMenuScene::OnExit()
{
    AudioManager::Instance().StopMusic();

    CT_LOG_INFO("MainMenuScene OnExit.");
}

void MainMenuScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("MainMenuScene", "Shutdown");

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("MainMenuScene shutdown.");
}

void MainMenuScene::SetSceneChangeCallback(SceneChangeCallback callback)
{
    m_sceneChangeCallback = std::move(callback);
}