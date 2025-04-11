#include "Application.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "Settings.h"
#include "LogManager.h"

#include <chrono>

#if defined(_MSC_VER) && defined(_DEBUG)
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
#endif

void Application::Init() {
#if defined(_MSC_VER) && defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    LogManager::Instance();
    WindowManager::Instance().Init(Settings::resolution, Settings::fullscreen, 60);
    AssetManager::Instance();
    InputManager::Instance();
    spdlog::info("Application initialized.");
}

void Application::Run() {
    Init();

    std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();
    while (isRunning && WindowManager::Instance().IsOpen()) {
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        ProcessEvents();
        Update(dt);
        Render();
    }

    Shutdown();
}

void Application::Shutdown() {
    WindowManager::Instance().Shutdown();
    LogManager::Instance().Shutdown();
}

void Application::ProcessEvents() {
    InputManager::Instance().Update();
}

void Application::Update(float dt) {
    // TODO: game logic here
}

void Application::Render() {
    WindowManager::Instance().BeginDraw();
    // TODO: draw game here
    WindowManager::Instance().EndDraw();
}