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

#include "Settings.h"

class Application
{
  public:
    Application(Settings &sharedSettings);
    ~Application() = default;

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    void Run();

  private:
    void Init();
    void Shutdown();
    void ProcessEvents();
    void Update(float dt);
    void Render();

    bool isRunning = false;
    Settings &settings;
};