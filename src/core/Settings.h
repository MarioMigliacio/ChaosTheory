// ============================================================================
//  File        : Settings.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Settings object is designed for global optional
//                configurations set and passed through the application
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <unordered_map>

struct Settings
{
    std::string m_windowTitle = "Chaos Theory";
    unsigned int m_windowWidth = 1280;
    unsigned int m_windowHeight = 720;
    unsigned int m_targetFramerate = 60;
    bool m_isFullscreen = false;

    float m_masterVolume = 100.0f;
    bool m_audioMuted = false;

    std::string m_audioDirectory = "assets/audio/";
    std::string m_fontDirectory = "assets/fonts/";
    std::string m_spriteDirectory = "assets/sprites/";

    std::unordered_map<std::string, sf::Keyboard::Key> m_keyBindings = {{"MoveLeft", sf::Keyboard::A},
                                                                        {"MoveRight", sf::Keyboard::D},
                                                                        {"MoveUp", sf::Keyboard::W},
                                                                        {"MoveDown", sf::Keyboard::S},
                                                                        {"MenuSelectNext", sf::Keyboard::Enter},
                                                                        {"MenuSelectBack", sf::Keyboard::Space}};
};
