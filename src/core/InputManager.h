// ============================================================================
//  File        : InputManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Input Manager is the CT library dedicated manager of
//                user inputs from any source
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Settings.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <unordered_map>

// ============================================================================
//  Class       : InputManager
//  Purpose     : Singleton class that manages the input events.
//
//  Responsibilities:
//      - Initializes and shuts down
//      - Returns Key Press, Key Held, Key Release.
//      - TODO: Mouse inputs, and any further robust additions.
//
// ============================================================================
class InputManager
{
  public:
    static InputManager &Instance();

    void Init(std::shared_ptr<Settings> settings);
    void Shutdown();

    bool IsInitialized() const;

    void Update(const sf::Event &event);
    void PostUpdate();

    bool IsKeyPressed(const std::string &action) const;
    bool IsJustPressed(const std::string &action) const;
    bool IsJustReleased(const std::string &action) const;

    sf::Vector2i GetMousePosition() const;
    void SetMousePosition(const sf::Vector2i &position);
    void SetMouseButtonState(sf::Mouse::Button button, bool isPressed); // for testing

    bool IsMouseButtonPressed(sf::Mouse::Button button) const;
    bool IsMouseButtonJustPressed(sf::Mouse::Button button) const;
    bool IsMouseButtonJustReleased(sf::Mouse::Button button) const;
    void UpdateMouseButton(sf::Mouse::Button button, bool isDown);

    void BindKey(const std::string &action, sf::Keyboard::Key key);
    void UnbindKey(const std::string &action);
    sf::Keyboard::Key GetBoundKey(const std::string &action) const;

  private:
    InputManager() = default;
    ~InputManager() = default;

    InputManager(const InputManager &) = delete;
    InputManager &operator=(const InputManager &) = delete;

    void LoadBindings();

  private:
    std::unordered_map<std::string, sf::Keyboard::Key> m_keyBindings;
    std::unordered_map<sf::Keyboard::Key, bool> m_currentState;
    std::unordered_map<sf::Keyboard::Key, bool> m_previousState;

    sf::Vector2i m_mousePosition;
    std::unordered_map<sf::Mouse::Button, bool> m_mouseCurrent;
    std::unordered_map<sf::Mouse::Button, bool> m_mousePrevious;

    std::shared_ptr<Settings> m_settings;
    bool m_isInitialized = false;
};
