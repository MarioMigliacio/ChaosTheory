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
#include "InputManager.h"
#include "LogManager.h"
#include "Settings.h"
#include "WindowManager.h"
#include "version.h"

#include <chrono>

#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

Application::Application(Settings &sharedSettings) : settings(sharedSettings)
{
}

void Application::Init()
{
#if defined(_MSC_VER) && defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    isRunning = true;
    LogManager::Instance().Init(settings);
    WindowManager::Instance().Init(settings);
    InputManager::Instance().Init(settings);
    AssetManager::Instance().Init(settings);

    CT_LOG_INFO("Application initialized.");
    CT_LOG_INFO("ChaosTheory v{}", CT_VERSION_STRING);
}

void Application::Run()
{
    Init();

    std::chrono::high_resolution_clock::time_point lastTime =
        std::chrono::high_resolution_clock::now();

    if (!WindowManager::Instance().IsOpen())
    {
        CT_LOG_ERROR("Window failed to open. Aborting.");
        return;
    }

    while (isRunning && WindowManager::Instance().IsOpen())
    {
        std::chrono::high_resolution_clock::time_point currentTime =
            std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

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

    CT_LOG_INFO("Application shutting down.");
    LogManager::Instance().Shutdown();
}

void Application::ProcessEvents()
{
    InputManager::Instance().Update();
    WindowManager::Instance().Update();
}

void Application::Update(float dt)
{
}

void Application::Render()
{
    WindowManager::Instance().BeginDraw();
    // TODO: draw game here
    WindowManager::Instance().EndDraw();
}