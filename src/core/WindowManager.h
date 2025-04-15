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

#include "Settings.h"
#include <SFML/Graphics.hpp>
#include <memory>

class WindowManager
{
  public:
    static WindowManager &Instance();

    void Init(std::shared_ptr<Settings> settings);
    void Shutdown();

    void BeginDraw();
    void EndDraw();

    bool IsOpen() const;
    void ToggleFullscreen();
    void ApplySettings();
    bool PollEvent(sf::Event &event);

    sf::RenderWindow &GetWindow();

  private:
    WindowManager() = default;
    ~WindowManager() = default;

    WindowManager(const WindowManager &) = delete;
    WindowManager &operator=(const WindowManager &) = delete;

  private:
    std::unique_ptr<sf::RenderWindow> m_window;
    std::shared_ptr<Settings> m_settings;
    bool m_isFullscreen = false;
    bool m_isInitialized = false;

  public:
    bool IsInitialized() const
    {
        return m_isInitialized;
    }
};
