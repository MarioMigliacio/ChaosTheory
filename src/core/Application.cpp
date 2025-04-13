// ============================================================================
//  File        : application.cpp
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
#include "InputManager.h"
#include "LogManager.h"
#include "Settings.h"
#include "WindowManager.h"
#include "version.h"

Application::Application(std::shared_ptr<Settings> sharedSettings)
    : m_settings(std::move(sharedSettings))
{
}

void Application::Init()
{
#if defined(_MSC_VER) && defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    LogManager::Instance().Init();
    WindowManager::Instance().Init(m_settings);
    InputManager::Instance().Init(m_settings);
    AssetManager::Instance().Init(m_settings);
    AudioManager::Instance().Init(m_settings);

    if (!WindowManager::Instance().IsOpen())
    {
        CT_LOG_ERROR("App::Init() - Window failed to open. Aborting.");
        return;
    }

    m_isRunning = true;

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
        Update(dt);
        Render();
    }

    Shutdown();
}

void Application::Shutdown()
{
    WindowManager::Instance().Shutdown();
    InputManager::Instance().Shutdown();
    AssetManager::Instance().Shutdown();
    AudioManager::Instance().Shutdown();

    CT_LOG_INFO("Application shutting down.");
    LogManager::Instance().Shutdown();

    m_settings.reset();
}

void Application::ProcessEvents()
{
    InputManager::Instance().Update();

    if (InputManager::Instance().IsKeyPressed(sf::Keyboard::Escape))
    {
        m_isRunning = false;
    }
}

void Application::Update(float dt)
{
    WindowManager::Instance().Update();
    // Game logic here
}

void Application::Render()
{
    WindowManager::Instance().BeginDraw();
    // TODO: draw game here
    WindowManager::Instance().EndDraw();
}