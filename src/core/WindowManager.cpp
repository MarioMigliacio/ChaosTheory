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
#include "LogManager.h"
#include "Settings.h"

WindowManager &WindowManager::Instance()
{
    static WindowManager instance;
    return instance;
}

void WindowManager::Init(std::shared_ptr<const Settings> settings)
{
    m_settings = std::move(settings);

    sf::VideoMode mode(m_settings->m_windowWidth, m_settings->m_windowHeight);
    sf::Uint32 style =
        m_settings->m_fullscreen ? sf::Style::Fullscreen : sf::Style::Default;

    m_window.create(mode, "Chaos Theory", style);
    m_window.setFramerateLimit(m_settings->m_framerateLimit);

    CT_LOG_INFO("WindowManager initialized.");
}

void WindowManager::Shutdown()
{
    if (m_window.isOpen())
    {
        m_window.close();
    }

    CT_LOG_INFO("WindowManager shutdown.");
}

void WindowManager::Update()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();
    }
}

void WindowManager::BeginDraw()
{
    m_window.clear();
}

void WindowManager::EndDraw()
{
    m_window.display();
}

bool WindowManager::IsOpen() const
{
    return m_window.isOpen();
}

sf::RenderWindow &WindowManager::GetWindow()
{
    return m_window;
}
