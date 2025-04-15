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
#include "Macros.h"
#include <SFML/Window/Keyboard.hpp>

InputManager &InputManager::Instance()
{
    static InputManager instance;
    return instance;
}

void InputManager::Init(std::shared_ptr<const Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;
    m_isInitialized = true;

    CT_LOG_INFO("InputManager initialized.");
}

void InputManager::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "Shutdown");

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("InputManager shutdown.");
}

void InputManager::Update()
{
}

bool InputManager::IsKeyPressed(int key) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsKeyPressed", false);

    return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
}
