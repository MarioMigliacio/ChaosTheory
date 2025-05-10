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

// Simple enumeration type for window Resolution settings.
enum class ResolutionSetting
{
    Res720p,
    Res1080p,
    Fullscreen
};

/// @brief Utility function to convert ResolutionSetting to string
/// @param id which ResolutionSetting enumeration.
/// @return readyonly string identifying the ResolutionSetting enum.
inline std::string ResolutionSettingToString(ResolutionSetting setting)
{
    switch (setting)
    {
        case ResolutionSetting::Res720p:
        default: // fallback
            return "720p";
        case ResolutionSetting::Res1080p:
            return "1080p";
        case ResolutionSetting::Fullscreen:
            return "Fullscreen";
    }
}

/// @brief Returns a ResultionSetting enumeration from a string.
/// @param str input ResolutionSetting as a string representation.
/// @return an Enumeration form of ResolutionSetting.
inline ResolutionSetting FromStringToResolution(const std::string &str)
{
    if (str == "720p")
    {
        return ResolutionSetting::Res720p;
    }

    if (str == "1080p")
    {
        return ResolutionSetting::Res1080p;
    }

    if (str == "Fullscreen")
    {
        return ResolutionSetting::Fullscreen;
    }

    return ResolutionSetting::Res720p; // default fallback
}

// Settings structure to hold internal game configurable data points.
struct Settings
{
    std::string m_windowTitle = "Chaos Theory";
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
