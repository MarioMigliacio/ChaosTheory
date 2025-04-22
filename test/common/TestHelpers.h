// ============================================================================
//  File        : TestHelpers.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : A Settings object used for internal testing
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Settings.h"
#include <memory>

inline std::shared_ptr<Settings> CreateTestSettings()
{
    auto settings = std::make_shared<Settings>();
    settings->m_windowWidth = 800;
    settings->m_windowHeight = 600;
    settings->m_windowTitle = "Test Window";
    settings->m_audioDirectory = "assets/audio/";
    settings->m_masterVolume = 50.0f;
    settings->m_musicVolume = 50.0f;
    settings->m_sfxVolume = 50.0f;
    settings->m_isMuted = false;
    return settings;
}
