// ============================================================================
//  File        : WindowManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Window Manager is the CT library wrapper for the SFML
//                window object, with configurable settings
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "WindowManager.h"
#include "Macros.h"
#include "Settings.h"

// This static reference to sf window objects is used for short circuit logic where the WindowManager might not yet
// be initialized, so doing routine logic would be dangerous
namespace
{
static sf::RenderWindow dummyWindow;
} // namespace

WindowManager &WindowManager::Instance()
{
    static WindowManager instance;
    return instance;
}

void WindowManager::Init(std::shared_ptr<Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;
    m_isFullscreen = m_settings->m_isFullscreen;

    m_isInitialized = true;
    ApplySettings();

    CT_LOG_INFO("WindowManager initialized.");
}

void WindowManager::ApplySettings()
{
    CT_WARN_IF_UNINITIALIZED("WindowManager", "ApplySettings");

    sf::VideoMode mode(m_settings->m_windowWidth, m_settings->m_windowHeight);
    sf::Uint32 style = m_settings->m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default;

    m_window = std::make_unique<sf::RenderWindow>(mode, m_settings->m_windowTitle, style);

    m_window->setFramerateLimit(m_settings->m_targetFramerate);
}

void WindowManager::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("WindowManager", "Shutdown");

    if (m_window->isOpen())
    {
        m_window->close();
    }

    m_window.reset();
    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("WindowManager shutdown.");
}

void WindowManager::ToggleFullscreen()
{
    CT_WARN_IF_UNINITIALIZED("WindowManager", "ToggleFullscreen");

    m_isFullscreen = !m_isFullscreen;
    m_settings->m_isFullscreen = m_isFullscreen;

    ApplySettings();
}

void WindowManager::BeginDraw()
{
    CT_WARN_IF_UNINITIALIZED("WindowManager", "BeginDraw");

    m_window->clear();
}

void WindowManager::EndDraw()
{
    CT_WARN_IF_UNINITIALIZED("WindowManager", "EndDraw");

    m_window->display();
}

bool WindowManager::IsOpen() const
{
    CT_WARN_IF_UNINITIALIZED_RET("WindowManager", "IsOpen", false);

    return m_window && m_window->isOpen();
}

bool WindowManager::PollEvent(sf::Event &event)
{
    CT_WARN_IF_UNINITIALIZED_RET("WindowManager", "PollEvent", false);

    return m_window && m_window->pollEvent(event);
}

sf::RenderWindow &WindowManager::GetWindow()
{
    CT_WARN_IF_UNINITIALIZED_RET("WindowManager", "GetWindow", dummyWindow);

    return *m_window;
}
