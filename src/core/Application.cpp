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
#include "SceneTransitionManager.h"
#include "SettingsManager.h"
#include "SplashScene.h"
#include "UIManager.h"
#include "WindowManager.h"
#include "version.h"

// Initializes the Application with all the Managers; holding final ownership over the provided settings.
void Application::Init()
{
#if defined(_MSC_VER) && defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    LogManager::Instance().Init();

    std::shared_ptr<Settings> settings = std::make_shared<Settings>();
    SettingsManager::Instance().LoadFromFile("config.json");

    m_settings = SettingsManager::Instance().GetSettings();

    UIManager::Instance().Init();
    WindowManager::Instance().Init(m_settings, sf::Style::Titlebar);
    InputManager::Instance().Init(m_settings);
    AssetManager::Instance().Init(m_settings);
    AudioManager::Instance().Init(m_settings);
    SceneManager::Instance().Init(m_settings);

    SceneManager::Instance().PushScene(SceneManager::Instance().Create(SceneID::Splash));

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

// Begin the main game loop.
void Application::Run()
{
    sf::Clock clock;

    while (m_isRunning && WindowManager::Instance().IsOpen() && SceneManager::Instance().HasActiveScene())
    {
        float dt = clock.restart().asSeconds();

        ProcessEvents();
        AudioManager::Instance().Update(dt);
        SceneManager::Instance().Update(dt);
        SceneTransitionManager::Instance().Update(dt);
        InputManager::Instance().PostUpdate();
        Render();
    }

    CT_LOG_INFO("No active scenes left. Shutting down application.");

    Shutdown();
}

// Shuts down the Application after shutting down any manager and resets internal state.
void Application::Shutdown()
{
    WindowManager::Instance().Shutdown();
    InputManager::Instance().Shutdown();
    AssetManager::Instance().Shutdown();
    SceneManager::Instance().Shutdown();
    AudioManager::Instance().Shutdown();
    UIManager::Instance().Shutdown();

    CT_LOG_INFO("Application shutting down.");
    LogManager::Instance().Shutdown();

    m_settings.reset();
    m_isInitialized = false;
}

// Requests any event processing that needs to be finalized in a game frame.
void Application::ProcessEvents()
{
    sf::Event event;

    while (WindowManager::Instance().PollEvent(event))
    {
        InputManager::Instance().Update(event);

        if (SceneManager::Instance().HasActiveScene())
        {
            SceneManager::Instance().GetActiveScene()->HandleEvent(event);
        }

        switch (event.type)
        {
            case sf::Event::Closed:
                m_isRunning = false;
                CT_LOG_INFO("Application closing from window close event.");
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                {
                    m_isRunning = false;
                    CT_LOG_INFO("Application closing from escape key.");
                }
                break;

            case sf::Event::Resized:
            {
                auto &window = WindowManager::Instance().GetWindow();
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

                CT_LOG_INFO("Window resized to {}x{}", event.size.width, event.size.height);

                // Optionally notify the scene to reposition UI
                if (SceneManager::Instance().HasActiveScene())
                {
                    SceneManager::Instance().GetActiveScene()->OnResize({event.size.width, event.size.height});
                }

                break;
            }

            default:
                break;
        }
    }
}

// Renders any necessary finalized imagery in the game frame.
void Application::Render()
{
    WindowManager::Instance().BeginDraw();

    SceneManager::Instance().Render();
    SceneTransitionManager::Instance().Render(WindowManager::Instance().GetWindow());

    WindowManager::Instance().EndDraw();
}