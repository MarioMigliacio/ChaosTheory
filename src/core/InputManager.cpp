// ============================================================================
//  File        : InputManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Input Manager is the CT library dedicated manager of
//                user inputs from any source
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "InputManager.h"
#include "LogManager.h"
#include <SFML/Window/Keyboard.hpp>

InputManager &InputManager::Instance()
{
    static InputManager instance;
    return instance;
}

void InputManager::Init(const Settings &settingsRef)
{
    settings = &settingsRef;

    CT_LOG_INFO("InputManager initialized.");
}

void InputManager::Shutdown()
{
    CT_LOG_INFO("InputManager shutdown.");
    settings = nullptr;
}

void InputManager::Update()
{
}

bool InputManager::IsKeyPressed(int key) const
{
    return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
}
