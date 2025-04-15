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

InputManager &InputManager::Instance()
{
    static InputManager instance;
    return instance;
}

void InputManager::Init(std::shared_ptr<Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;
    LoadBindings();

    m_isInitialized = true;

    CT_LOG_INFO("InputManager initialized.");
}

void InputManager::LoadBindings()
{
    // Initialize key bindings from settings or defaults
    m_keyBindings["MoveUp"] = sf::Keyboard::W;
    m_keyBindings["MoveDown"] = sf::Keyboard::S;
    m_keyBindings["MoveLeft"] = sf::Keyboard::A;
    m_keyBindings["MoveRight"] = sf::Keyboard::D;
}

bool InputManager::GetKeyState(const std::unordered_map<sf::Keyboard::Key, bool> &stateMap, sf::Keyboard::Key key) const
{
    auto it = stateMap.find(key);
    return it != stateMap.end() && it->second;
}

void InputManager::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "Shutdown");

    m_keyBindings.clear();
    m_currentState.clear();
    m_previousState.clear();
    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("InputManager shutdown.");
}

void InputManager::Update(const sf::Event &event)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "Update");

    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
    {
        bool isDown = (event.type == sf::Event::KeyPressed);
        m_currentState[event.key.code] = isDown;

        // Ensure the key has a previous state entry too
        if (m_previousState.find(event.key.code) == m_previousState.end())
        {
            m_previousState[event.key.code] = !isDown;
        }
    }
}

void InputManager::PostUpdate()
{
    m_previousState = m_currentState;
}

bool InputManager::IsKeyPressed(const std::string &action) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsKeyPressed", false);

    auto it = m_keyBindings.find(action);

    if (it == m_keyBindings.end())
    {
        return false;
    }

    sf::Keyboard::Key code = it->second;
    auto currentIt = m_currentState.find(code);

    return (currentIt != m_currentState.end()) && currentIt->second;
}

bool InputManager::IsJustPressed(const std::string &action) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsJustPressed", false);

    auto it = m_keyBindings.find(action);
    if (it == m_keyBindings.end())
    {
        return false;
    }

    sf::Keyboard::Key code = it->second;

    const bool curr = GetKeyState(m_currentState, code);
    const bool prev = GetKeyState(m_previousState, code);

    return curr && !prev;
}

bool InputManager::IsJustReleased(const std::string &action) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsJustReleased", false);

    auto it = m_keyBindings.find(action);
    if (it == m_keyBindings.end())
        return false;

    sf::Keyboard::Key code = it->second;

    const bool curr = GetKeyState(m_currentState, code);
    const bool prev = GetKeyState(m_previousState, code);

    return !curr && prev;
}

void InputManager::BindKey(const std::string &action, sf::Keyboard::Key key)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "BindKey");

    m_keyBindings[action] = key;
}

void InputManager::UnbindKey(const std::string &action)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "UnbindKey");

    m_keyBindings.erase(action);
}

sf::Keyboard::Key InputManager::GetBoundKey(const std::string &action) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "GetBoundKey", sf::Keyboard::Unknown);

    auto it = m_keyBindings.find(action);

    if (it != m_keyBindings.end())
    {
        return it->second;
    }

    return sf::Keyboard::Unknown;
}