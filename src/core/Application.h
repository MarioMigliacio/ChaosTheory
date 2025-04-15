// ============================================================================
//  File        : Application.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Application is the entire library entry point of logic
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "SceneManager.h"
#include "Settings.h"
#include <memory>

class Application
{
  public:
    explicit Application(std::shared_ptr<Settings> sharedSettings);
    ~Application() = default;

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    void Init();
    void Run();

  private:
    void Shutdown();
    void ProcessEvents();
    void Render();

    bool m_isRunning = false;
    bool m_isInitialized = false;
    std::shared_ptr<Settings> m_settings;
    std::unique_ptr<SceneManager> m_sceneManager;
};