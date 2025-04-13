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

#include <string>

struct Settings
{
    std::string m_windowTitle = "Chaos Theory";
    unsigned int m_windowWidth = 1280;
    unsigned int m_windowHeight = 720;
    unsigned int m_framerateLimit = 60;
    bool m_fullscreen = false;

    float m_masterVolume = 100.0f;
    bool m_audioMuted = false;

    std::string m_audioDirectory = "assets/audio/";
    std::string m_fontDirectory = "assets/fonts/";
    std::string m_spriteDirectory = "assets/sprites/";
};
