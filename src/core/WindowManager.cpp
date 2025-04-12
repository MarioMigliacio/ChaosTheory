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

WindowManager &WindowManager::Instance()
{
    static WindowManager instance;
    return instance;
}

void WindowManager::Init(const Settings &settingsRef)
{
    settings = &settingsRef;

    sf::VideoMode mode(settings->screenWidth, settings->screenHeight);
    sf::Uint32 style =
        settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Default;

    window.create(mode, "Chaos Theory", style);
    window.setFramerateLimit(settings->framerateLimit);

    CT_LOG_INFO("WindowManager initialized.");
}

void WindowManager::Shutdown()
{
    if (window.isOpen())
    {
        window.close();
    }

    CT_LOG_INFO("WindowManager shutdown.");
    settings = nullptr;
}

void WindowManager::Update()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void WindowManager::BeginDraw()
{
    window.clear();
}

void WindowManager::EndDraw()
{
    window.display();
}

bool WindowManager::IsOpen() const
{
    return window.isOpen();
}

sf::RenderWindow &WindowManager::GetWindow()
{
    return window;
}
