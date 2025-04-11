#include "InputManager.h"

InputManager& InputManager::Instance() {
    static InputManager instance;
    return instance;
}

void InputManager::Update() {
    currentKeys.clear();
    for (int k = sf::Keyboard::A; k <= sf::Keyboard::KeyCount; ++k) {
        sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(k);
        currentKeys[key] = sf::Keyboard::isKeyPressed(key);
    }
}

bool InputManager::IsKeyPressed(sf::Keyboard::Key key) const {
    auto it = currentKeys.find(key);
    if (it != currentKeys.end()) {
        return it->second;
    }
    return false;
}