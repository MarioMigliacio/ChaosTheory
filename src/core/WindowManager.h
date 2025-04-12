// ============================================================================
//  File        : WindowManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Window Manager is the CT library wrapper for the SFML
//                window object, with configurable settings
//  
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include "Settings.h"

class WindowManager {
public:
    static WindowManager& Instance();

    void Init(const Settings& settings);
    void Shutdown();

    void Update();
    void BeginDraw();
    void EndDraw();

    bool IsOpen() const;
    sf::RenderWindow& GetWindow();

private:
    WindowManager() = default;
    ~WindowManager() = default;

    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;

private:
    sf::RenderWindow window;
    const Settings* settings = nullptr;
};
