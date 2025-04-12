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

#include <spdlog/common.h>
#include <string>

class Settings
{
  public:
    Settings() = default;
    ~Settings() = default;

    Settings(const Settings &) = default;
    Settings &operator=(const Settings &) = default;

    unsigned int screenWidth = 1280;
    unsigned int screenHeight = 720;
    bool fullscreen = false;
    unsigned int framerateLimit = 60;

    spdlog::level::level_enum logLevel = spdlog::level::debug;
    std::string defaultFontPath = "assets/fonts/default.ttf";
};
