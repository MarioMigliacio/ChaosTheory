// ============================================================================
//  File        : InputManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : InputManager is the CT library dedicated manager of
//                user inputs from any source
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "InputManager.h"
#include "KeyBindings.h"
#include "Macros.h"

/// @brief Get the current Instance for this InputManager singleton.
/// @return reference to existing InputManager interface.
InputManager &InputManager::Instance()
{
    static InputManager instance;
    return instance;
}

/// @brief Initializes the InputManager using the provided settings.
/// @param settings Settings object to initalize with.
void InputManager::Init(std::shared_ptr<Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;
    m_isInitialized = true;

    CT_LOG_INFO("InputManager initialized.");
}

/// @brief Shuts down the InputManager and resets internal state.
void InputManager::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "Shutdown");

    m_keyBindings.clear();
    m_currentState.clear();
    m_previousState.clear();
    m_mouseCurrent.clear();
    m_mousePrevious.clear();

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("InputManager shutdown.");
}

/// @brief Returns whether the InputManager has been initialized.
/// @return m_isInitialized.
bool InputManager::IsInitialized() const
{
    return m_isInitialized;
}

/// @brief Performs internal state management during a single frame.
/// @param event event to enact on.
void InputManager::Update(const sf::Event &event)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "Update");

    switch (event.type)
    {
        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased:
        {
            bool isDown = (event.type == sf::Event::KeyPressed);
            m_currentState[event.key.code] = isDown;

            // Ensure the key has a previous state entry too
            if (!m_previousState.contains(event.key.code))
            {
                m_previousState[event.key.code] = !isDown;
            }
        }

        break;

        case sf::Event::MouseMoved:
            m_mousePosition = {event.mouseMove.x, event.mouseMove.y};

            break;

        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased:
        {
            bool isDown = event.type == sf::Event::MouseButtonPressed;
            auto button = event.mouseButton.button;

            if (!m_mouseCurrent.contains(button))
            {
                m_mouseCurrent[button] = isDown;
            }

            if (!m_mousePrevious.contains(button))
            {
                m_mousePrevious[button] = !isDown;
            }

            m_mouseCurrent[button] = isDown;
        }

        break;

        default:
            break;
    }
}

/// @brief Completes state management during the end of a frame.
void InputManager::PostUpdate()
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "PostUpdate");

    m_previousState = m_currentState;
    m_mousePrevious = m_mouseCurrent;
}

/// @brief Returns the state of whether a key is still being pressed based on the input action.
/// @param action determine if the action is bound to key, and if it is being pressed.
/// @return true / false
bool InputManager::IsKeyPressed(const std::string &action) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsKeyPressed", false);

    if (!m_keyBindings.contains(action))
    {
        return false;
    }

    for (const auto &key : m_keyBindings.at(action))
    {
        if (m_currentState.contains(key) && m_currentState.at(key))
        {
            return true;
        }
    }

    return false;
}

/// @brief Returns the state of if a key has just been pressed, based on the input action.
/// @param action determine if the action is bound to key, and if it is just being pressed now.
/// @return true / false
bool InputManager::IsKeyJustPressed(const std::string &action) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsKeyJustPressed", false);

    if (!m_keyBindings.contains(action))
    {
        return false;
    }

    for (const auto &key : m_keyBindings.at(action))
    {
        const bool curr = m_currentState.contains(key) ? m_currentState.at(key) : false;
        const bool prev = m_previousState.contains(key) ? m_previousState.at(key) : false;

        if (curr && !prev)
        {
            return true;
        }
    }

    return false;
}

/// @brief Returns the state of if a key has just been released, based on the input action.
/// @param action determine if the action is bound to key, and if it is just being released now.
/// @return true / false
bool InputManager::IsKeyJustReleased(const std::string &action) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsKeyJustReleased", false);

    if (!m_keyBindings.contains(action))
    {
        return false;
    }

    for (const auto &key : m_keyBindings.at(action))
    {
        const bool curr = m_currentState.contains(key) ? m_currentState.at(key) : false;
        const bool prev = m_previousState.contains(key) ? m_previousState.at(key) : false;

        if (!curr && prev)
        {
            return true;
        }
    }

    return false;
}

/// @brief Returns the currently tracked position of the mouse.
/// @return m_mousePosition.
sf::Vector2i InputManager::GetMousePosition() const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "GetMousePosition", sf::Vector2i(0, 0));

    return m_mousePosition;
}

/// @brief Internally updates the mouse position during event processing.
/// @param position new m_mousePosition.
void InputManager::SetMousePosition(const sf::Vector2i &position)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "SetMousePosition");

    m_mousePosition = position;
}

/// @brief Updates internal for mouse button press, for testing.
/// @param button which Button.
/// @param isPressed IsPressed?
void InputManager::SetMouseButtonState(sf::Mouse::Button button, bool isPressed)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "SetMouseButtonState");

    m_mouseCurrent[button] = isPressed;
}

/// @brief Returns whether or not the Mouse button identified as 'button' is being pressed.
/// @param button the Button to check.
/// @return true / false
bool InputManager::IsMouseButtonPressed(sf::Mouse::Button button) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsMouseButtonPressed", false);

    return m_mouseCurrent.contains(button) ? m_mouseCurrent.at(button) : false;
}

/// @brief Returns whether or not the Mouse button identified as 'button' has just been pressed the first time.
/// @param button the Button to check.
/// @return true / false
bool InputManager::IsMouseButtonJustPressed(sf::Mouse::Button button) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsMouseButtonJustPressed", false);

    bool curr = m_mouseCurrent.contains(button) ? m_mouseCurrent.at(button) : false;
    bool prev = m_mousePrevious.contains(button) ? m_mousePrevious.at(button) : false;

    return curr && !prev;
}

/// @brief Returns whether or not the Mouse button identified as 'button' has just been released.
/// @param button the Button to check.
/// @return true / false
bool InputManager::IsMouseButtonJustReleased(sf::Mouse::Button button) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsMouseButtonJustReleased", false);

    bool curr = m_mouseCurrent.contains(button) ? m_mouseCurrent.at(button) : false;
    bool prev = m_mousePrevious.contains(button) ? m_mousePrevious.at(button) : false;

    return !curr && prev;
}

/// @brief Internally updates the state when SFML events are received
/// @param button Button being updated.
/// @param isDown State of Button to update.
void InputManager::UpdateMouseButton(sf::Mouse::Button button, bool isDown)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "UpdateMouseButton");

    if (!m_mousePrevious.contains(button))
    {
        m_mousePrevious[button] = !isDown;
    }

    m_mousePrevious[button] = isDown;
}

/// @brief Stores an input action and the matching SFML Key to an internal unordered map.
/// @param action The string representation for the action.
/// @param key Button key to bind action to.
void InputManager::BindKey(const std::string &action, sf::Keyboard::Key key)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "BindKey");

    auto &keys = m_keyBindings[action];

    if (std::find(keys.begin(), keys.end(), key) == keys.end())
    {
        keys.push_back(key);
    }

    CT_LOG_DEBUG("InputManager Bound key: {}.", action);
}

/// @brief Removes an input action and the matching SFML Key from the internal unordered map.
/// @param action The string representation for the action.
void InputManager::UnbindKey(const std::string &action)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "UnbindKey");

    m_keyBindings.erase(action);

    CT_LOG_DEBUG("InputManager Unbound key: {}.", action);
}

/// @brief Returns the matching SFML Key if the supplied action maps correctly to the internal unordered map.
/// @param action The string representation for the action.
/// @return The keyboard key which is mapped to the action.
std::vector<sf::Keyboard::Key> InputManager::GetBoundKeys(const std::string &action) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "GetBoundKey", {});

    if (m_keyBindings.contains(action))
    {
        return m_keyBindings.at(action);
    }

    return {}; // empty vector if nothing bound
}

/// @brief Completely clears all bindings for all actions.
void InputManager::ClearAllBindings()
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "ClearAllBindings");

    m_keyBindings.clear();

    CT_LOG_DEBUG("InputManager cleared all bindings.");
}

/// @brief Helper method to load Player related key bindings.
void InputManager::LoadPlayerInput()
{
    BindKey(KeyBindings::MoveUpConstantKey, sf::Keyboard::W);
    BindKey(KeyBindings::MoveUpConstantKey, sf::Keyboard::Up);

    BindKey(KeyBindings::MoveDownConstantKey, sf::Keyboard::S);
    BindKey(KeyBindings::MoveDownConstantKey, sf::Keyboard::Down);

    BindKey(KeyBindings::MoveLeftConstantKey, sf::Keyboard::A);
    BindKey(KeyBindings::MoveLeftConstantKey, sf::Keyboard::Left);

    BindKey(KeyBindings::MoveRightConstantKey, sf::Keyboard::D);
    BindKey(KeyBindings::MoveRightConstantKey, sf::Keyboard::Right);

    BindKey(KeyBindings::AccelerateConstantKey, sf::Keyboard::LShift);
    BindKey(KeyBindings::AccelerateConstantKey, sf::Keyboard::RShift);

    BindKey(KeyBindings::FireGunConstantKey, sf::Keyboard::Space);

    BindKey(KeyBindings::LaunchBombConstantKey, sf::Keyboard::LControl);
    BindKey(KeyBindings::LaunchBombConstantKey, sf::Keyboard::RControl);
}

/// @brief Helper method to unload Player related key bindings.
void InputManager::UnloadPlayerInput()
{
    UnbindKey(KeyBindings::MoveUpConstantKey);
    UnbindKey(KeyBindings::MoveDownConstantKey);
    UnbindKey(KeyBindings::MoveLeftConstantKey);
    UnbindKey(KeyBindings::MoveRightConstantKey);
    UnbindKey(KeyBindings::AccelerateConstantKey);
    UnbindKey(KeyBindings::FireGunConstantKey);
    UnbindKey(KeyBindings::LaunchBombConstantKey);
}
