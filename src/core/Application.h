// ============================================================================
//  File        : application.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Application is the entire library entry point of logic
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <memory>

struct Settings; // Forward Declaration vs #include "Settings.h"

class Application
{
  public:
    Application(std::shared_ptr<Settings> sharedSettings);
    ~Application() = default;

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    void Init();
    void Run();

  private:
    void Shutdown();
    void ProcessEvents();
    void Update(float dt);
    void Render();

    bool m_isRunning = false;
    std::shared_ptr<Settings> m_settings;
};