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

/// @brief Uses the Nlohmann library to Load Json to populate a Settings object. (Deserialize)
/// @param filepath readonly filepath name
/// @param settings readonly Settings object
/// @return pass / fail
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

        // Asset paths
        settings.m_fontDirectory = j["paths"]["font_dir"];
        settings.m_audioDirectory = j["paths"]["audio_dir"];
        settings.m_spriteDirectory = j["paths"]["sprite_dir"];

        // Volume configs
        settings.m_masterVolume = j["audio"]["master_volume"];
        settings.m_musicVolume = j["audio"]["music_volume"];
        settings.m_sfxVolume = j["audio"]["sfx_volume"];
        settings.m_isMuted = j["audio"]["is_muted"];

        // Video Resolution
        settings.m_resolution = FromStringToResolution(j["video"]["resolution"]);

        // Game Difficulty
        settings.m_gameDifficulty = FromStringToGameDifficulty(j["difficulty"]["mode"]);
    }

    catch (const json::exception &e)
    {
        std::cerr << "JSON parse error: " << e.what() << std::endl;

        return false;
    }

    return true;
}

/// @brief Uses the Nlohmann library Save Json to the configuration settings file. (Serialize)
/// @param filepath readonly filepath name
/// @param settings readonly Settings object
/// @return pass / fail
bool ConfigLoader::SaveAsJson(const std::string &filepath, const Settings &settings)
{
    using json = nlohmann::json;
    json j;

    j["paths"]["font_dir"] = settings.m_fontDirectory;
    j["paths"]["audio_dir"] = settings.m_audioDirectory;
    j["paths"]["sprite_dir"] = settings.m_spriteDirectory;

    j["audio"]["master_volume"] = settings.m_masterVolume;
    j["audio"]["music_volume"] = settings.m_musicVolume;
    j["audio"]["sfx_volume"] = settings.m_sfxVolume;
    j["audio"]["is_muted"] = settings.m_isMuted;

    j["video"]["resolution"] = ResolutionSettingToString(settings.m_resolution);

    j["difficulty"]["mode"] = GameDifficultySettingToString(settings.m_gameDifficulty);

    std::ofstream out(filepath);

    if (!out.is_open())
    {
        std::cerr << "Failed to open config file for writing: " << filepath << '\n';
        return false;
    }

    out << j.dump(4); // Indented output
    return true;
}
