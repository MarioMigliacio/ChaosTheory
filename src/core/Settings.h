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

#include "SettingModes.h"
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <unordered_map>

/// @brief Settings structure to hold internal game configurable data points.
struct Settings
{
    std::string m_windowTitle = "ChaosTheory";
    ResolutionSetting m_resolution = ResolutionSetting::Res720p;
    unsigned int m_windowWidth = 1280;
    unsigned int m_windowHeight = 720;
    unsigned int m_targetFramerate = 60;
    bool m_verticleSyncEnabled = true;
    bool m_isFullscreen = false;

    float m_masterVolume = 100.0f;
    float m_musicVolume = 100.0f;
    float m_sfxVolume = 100.0f;
    bool m_isMuted = false;

    GameDifficultySetting m_gameDifficulty = GameDifficultySetting::Normal;

    std::string m_audioDirectory = "assets/audio/";
    std::string m_fontDirectory = "assets/fonts/";
    std::string m_spriteDirectory = "assets/sprites/";

    std::unordered_map<std::string, sf::Keyboard::Key> m_keyBindings = {{"MoveLeft", sf::Keyboard::A},
                                                                        {"MoveRight", sf::Keyboard::D},
                                                                        {"MoveUp", sf::Keyboard::W},
                                                                        {"MoveDown", sf::Keyboard::S}};
};
