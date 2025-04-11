#pragma once

#include <SFML/Window.hpp>
#include <unordered_map>

class InputManager {
public:
    static InputManager& Instance();

    void Update();
    bool IsKeyPressed(sf::Keyboard::Key key) const;

    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(InputManager&&) = delete;

private:
    InputManager() = default;
    ~InputManager() = default;

    std::unordered_map<sf::Keyboard::Key, bool> currentKeys;
};