// ============================================================================
//  File        : ConfigLoader.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-19
//  Description : Json settings file to assist in default loading
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "ConfigLoader.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

// uses the Nlohmann library LoadFromJson function to populate a Settings object.
bool ConfigLoader::LoadFromJson(const std::string &filepath, Settings &settings)
{
    std::ifstream in(filepath);

    if (!in.is_open())
    {
        std::cerr << "Failed to open config file: " << filepath << '\n';
        return false;
    }

    try
    {
        json j;
        in >> j;

        settings.m_fontDirectory = j["paths"]["font_dir"];
        settings.m_audioDirectory = j["paths"]["audio_dir"];
        settings.m_spriteDirectory = j["paths"]["sprite_dir"];

        settings.m_masterVolume = j["audio"]["master_volume"];
        settings.m_musicVolume = j["audio"]["music_volume"];
        settings.m_sfxVolume = j["audio"]["sfx_volume"];
        settings.m_isMuted = j["audio"]["is_muted"];
    }
    catch (const json::exception &e)
    {
        std::cerr << "JSON parse error: " << e.what() << '\n';

        return false;
    }

    return true;
}
