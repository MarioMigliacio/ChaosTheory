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
    settings->m_audioMuted = false;
    return settings;
}
