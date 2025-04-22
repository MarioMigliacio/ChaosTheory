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

// Initializes the Input manager using the provided settings.
void InputManager::Init(std::shared_ptr<Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;
    LoadBindings();

    m_isInitialized = true;

    CT_LOG_INFO("InputManager initialized.");
}

// Shuts down the Input manager and resets internal state.
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

// Returns whether the Input manager has been initialized.
bool InputManager::IsInitialized() const
{
    return m_isInitialized;
}

// Performs internal state management during a single frame.
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

// Completes state management during the end of a frame.
void InputManager::PostUpdate()
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "PostUpdate");

    m_previousState = m_currentState;
    m_mousePrevious = m_mouseCurrent;
}

// Returns the state of whether a key is still being pressed based on the input action.
bool InputManager::IsKeyPressed(const std::string &action) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsKeyPressed", false);

    if (!m_keyBindings.contains(action))
    {
        return false;
    }

    sf::Keyboard::Key key = m_keyBindings.at(action);

    return m_currentState.contains(key) ? m_currentState.at(key) : false;
}

// Returns the state of if a key has just been pressed, based on the input action.
bool InputManager::IsJustPressed(const std::string &action) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsJustPressed", false);

    if (!m_keyBindings.contains(action))
    {
        return false;
    }

    sf::Keyboard::Key key = m_keyBindings.at(action);

    const bool curr = m_currentState.contains(key) ? m_currentState.at(key) : false;
    const bool prev = m_previousState.contains(key) ? m_previousState.at(key) : false;

    return curr && !prev;
}

// Returns the state of if a key has just been released, based on the input action.
bool InputManager::IsJustReleased(const std::string &action) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsJustReleased", false);

    if (!m_keyBindings.contains(action))
    {
        return false;
    }

    sf::Keyboard::Key key = m_keyBindings.at(action);

    const bool curr = m_currentState.contains(key) ? m_currentState.at(key) : false;
    const bool prev = m_previousState.contains(key) ? m_previousState.at(key) : false;

    return !curr && prev;
}

// Returns the currently tracked position of the mouse.
sf::Vector2i InputManager::GetMousePosition() const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "GetMousePosition", sf::Vector2i(0, 0));

    return m_mousePosition;
}

// Internally updates the mouse position during event processing.
void InputManager::SetMousePosition(const sf::Vector2i &position)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "SetMousePosition");

    m_mousePosition = position;
}

// Updates internal for mouse button press
void InputManager::SetMouseButtonState(sf::Mouse::Button button, bool isPressed)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "SetMouseButtonState");

    m_mouseCurrent[button] = isPressed;
}

// Returns whether or not the Mouse button identified as 'button' is being pressed.
bool InputManager::IsMouseButtonPressed(sf::Mouse::Button button) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsMouseButtonPressed", false);

    return m_mouseCurrent.contains(button) ? m_mouseCurrent.at(button) : false;
}

// Returns whether or not the Mouse button identified as 'button' has just been pressed the first time.
bool InputManager::IsMouseButtonJustPressed(sf::Mouse::Button button) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsMouseButtonJustPressed", false);

    bool curr = m_mouseCurrent.contains(button) ? m_mouseCurrent.at(button) : false;
    bool prev = m_mousePrevious.contains(button) ? m_mousePrevious.at(button) : false;

    return curr && !prev;
}

// Returns whether or not the Mouse button identified as 'button' has just been released.
bool InputManager::IsMouseButtonJustReleased(sf::Mouse::Button button) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "IsMouseButtonJustReleased", false);

    bool curr = m_mouseCurrent.contains(button) ? m_mouseCurrent.at(button) : false;
    bool prev = m_mousePrevious.contains(button) ? m_mousePrevious.at(button) : false;

    return !curr && prev;
}

// Internally updates the state when SFML events are received
void InputManager::UpdateMouseButton(sf::Mouse::Button button, bool isDown)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "UpdateMouseButton");

    if (!m_mousePrevious.contains(button))
    {
        m_mousePrevious[button] = !isDown;
    }

    m_mousePrevious[button] = isDown;
}

// Stores an input action and the matching SFML Key to an internal unordered map.
void InputManager::BindKey(const std::string &action, sf::Keyboard::Key key)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "BindKey");

    m_keyBindings[action] = key;
}

// Removes an input action and the matching SFML Key from the internal unordered map.
void InputManager::UnbindKey(const std::string &action)
{
    CT_WARN_IF_UNINITIALIZED("InputManager", "UnbindKey");

    m_keyBindings.erase(action);
}

// Returns the matching SFML Key if the supplied action maps correctly to the internal unordered map.
sf::Keyboard::Key InputManager::GetBoundKey(const std::string &action) const
{
    CT_WARN_IF_UNINITIALIZED_RET("InputManager", "GetBoundKey", sf::Keyboard::Unknown);

    if (m_keyBindings.contains(action))
    {
        return m_keyBindings.at(action);
    }

    return sf::Keyboard::Unknown;
}

// Applies synchronization between the manager settings of SFML Key bindings and the Settings object.
void InputManager::LoadBindings()
{
    // TODO, provide additional key bindings for various scenes. This merely loads some easy defaults.
    // Initialize key bindings from settings or defaults
    m_keyBindings["MoveUp"] = sf::Keyboard::W;
    m_keyBindings["MoveDown"] = sf::Keyboard::S;
    m_keyBindings["MoveLeft"] = sf::Keyboard::A;
    m_keyBindings["MoveRight"] = sf::Keyboard::D;
}
