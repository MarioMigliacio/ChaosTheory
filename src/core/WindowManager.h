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

// ============================================================================
//  Class       : WindowManager
//  Purpose     : Singleton class that manages the application window.
//
//  Responsibilities:
//      - Initializes and shuts down
//      - Returns SFML Render window, handles PollEvents,
//                size adjustments, and rendering, and fullsize.
//
// ============================================================================
class WindowManager
{
  public:
    static WindowManager &Instance();

    void Init(std::shared_ptr<Settings> settings, sf::Uint32 style = sf::Style::Default);
    void Shutdown();

    bool IsInitialized() const;
    bool IsOpen() const;

    void BeginDraw();
    void EndDraw();

    void Recreate(const unsigned int width, const unsigned int height, const std::string &title, sf::Uint32 style);
    void ApplySettings(sf::Uint32 style);
    void SetClearColor(const sf::Color &color);
    void ToggleFullscreen();

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

    std::string m_title;
    sf::Uint32 m_style;

    sf::Color m_clearColor = sf::Color::Black;
};
