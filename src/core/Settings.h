#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

namespace Settings {
    extern sf::Vector2u resolution;
    extern bool fullscreen;

    void Load();
    void Save();
}