// ============================================================================
//  File        : Application.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Application is the entire library entry point of logic
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "Application.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "GameScene.h"
#include "InputManager.h"
#include "Macros.h"
#include "MainMenuScene.h"
#include "SceneFactory.h"
#include "SceneManager.h"
#include "Settings.h"
#include "WindowManager.h"
#include "version.h"

Application::Application(std::shared_ptr<Settings> sharedSettings) : m_settings(std::move(sharedSettings))
{
}

void Application::Init()
{
#if defined(_MSC_VER) && defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    LogManager::Instance().Init();
    WindowManager::Instance().Init(m_settings);
    InputManager::Instance().Init(m_settings);
    AssetManager::Instance().Init(m_settings);
    AudioManager::Instance().Init(m_settings);

    m_sceneManager = std::make_unique<SceneManager>(m_settings);
    m_sceneManager->Init();

    // Register scenes with factory, syntax looks a bit gnarly because of the lambda expression and function callback
    SceneFactory::Instance().Register("MainMenu",
                                      [this]()
                                      {
                                          auto scene = std::make_unique<MainMenuScene>(m_settings);
                                          scene->SetSceneChangeCallback(
                                              [this](std::unique_ptr<Scene> next)
                                              { m_sceneManager->PushScene(std::move(next)); });
                                          return scene;
                                      });

    SceneFactory::Instance().Register("Game",
                                      [this]()
                                      {
                                          auto scene = std::make_unique<GameScene>(m_settings);
                                          scene->SetSceneChangeCallback(
                                              [this](std::unique_ptr<Scene> next)
                                              { m_sceneManager->PushScene(std::move(next)); });
                                          return scene;
                                      });
    m_sceneManager->PushScene(SceneFactory::Instance().Create("MainMenu"));
    // m_sceneManager->PushScene(SceneFactory::Instance().Create("Game"));

    if (!WindowManager::Instance().IsOpen())
    {
        CT_LOG_ERROR("App::Init() - Window failed to open. Aborting.");
        return;
    }

    m_isRunning = true;
    m_isInitialized = true;

    CT_LOG_INFO("Application initialized.");
    CT_LOG_INFO("ChaosTheory v{}", CT_VERSION_STRING);
}

void Application::Run()
{
    sf::Clock clock;

    while (m_isRunning && WindowManager::Instance().IsOpen())
    {
        float dt = clock.restart().asSeconds();

        ProcessEvents();
        InputManager::Instance().Update();
        AudioManager::Instance().Update(dt);
        m_sceneManager->Update(dt);
        Render();
    }

    Shutdown();
}

void Application::Shutdown()
{
    if (m_sceneManager)
    {
        m_sceneManager->Shutdown();
        m_sceneManager.reset();
    }

    WindowManager::Instance().Shutdown();
    InputManager::Instance().Shutdown();
    AssetManager::Instance().Shutdown();
    AudioManager::Instance().Shutdown();

    CT_LOG_INFO("Application shutting down.");
    LogManager::Instance().Shutdown();

    m_settings.reset();
    m_isInitialized = false;
}

void Application::ProcessEvents()
{
    sf::Event event;

    while (WindowManager::Instance().PollEvent(event))
    {
        m_sceneManager->HandleEvent(event); // <-- propagate to scene

        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
        {
            m_isRunning = false;

            CT_LOG_INFO("Application closing from escape or close event.");
        }
    }
}

void Application::Render()
{
    WindowManager::Instance().BeginDraw();
    m_sceneManager->Render();
    WindowManager::Instance().EndDraw();
}