// ============================================================================
//  File        : SettingsManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-22
//  Description : Singleton class managing runtime and persistent settings.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SettingsManager.h"
#include "ConfigLoader.h"
#include "Macros.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

SettingsManager &SettingsManager::Instance()
{
    static SettingsManager instance;
    return instance;
}

// Initializes this SettingsManager, owning the Settings configurations.
void SettingsManager::Init(std::shared_ptr<Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = std::move(settings);
    m_isInitialized = true;

    CT_LOG_INFO("SettingsManager initialized.");
}

// Resets the internal states for the Settings Manager.
void SettingsManager::Shutdown()
{
    m_settings.reset();
    m_isInitialized = false;
}

// Returns whether or not the SettingsManager has been initialized.
bool SettingsManager::IsInitialized() const
{
    return m_isInitialized;
}

// Loads in the essential default settings config file into this Settings object.
bool SettingsManager::LoadFromFile(const std::string &filepath)
{
    if (!m_settings)
    {
        m_settings = std::make_shared<Settings>();
    }

    bool result = ConfigLoader::LoadFromJson(filepath, *m_settings);
    m_isInitialized = result;

    CT_LOG_INFO("SettingsManager LoadDefaultsFromConfig: {}.", m_isInitialized ? "Success" : "Fail");

    return result;
}

bool SettingsManager::SaveToFile(const std::string &path) const
{
    CT_WARN_IF_UNINITIALIZED_RET("SettingsManager", "SaveToFile", false);

    if (!m_settings)
    {
        return false;
    }

    using json = nlohmann::json;

    json j;

    j["paths"]["font_dir"] = m_settings->m_fontDirectory;
    j["paths"]["audio_dir"] = m_settings->m_audioDirectory;
    j["paths"]["sprite_dir"] = m_settings->m_spriteDirectory;

    j["audio"]["master_volume"] = m_settings->m_masterVolume;
    j["audio"]["music_volume"] = m_settings->m_musicVolume;
    j["audio"]["sfx_volume"] = m_settings->m_sfxVolume;
    j["audio"]["is_muted"] = m_settings->m_isMuted;

    std::ofstream out(path);

    if (!out.is_open())
    {
        std::cerr << "Failed to open file for saving: " << path << "\\n";

        return false;
    }

    out << j.dump(4); // Pretty print with indent

    CT_LOG_INFO("SettingsManager saved to file: {}", path);

    return true;
}

// Get the reference for the Settings object.
std::shared_ptr<Settings> SettingsManager::GetSettings()
{
    return m_settings;
}

bool SettingsManager::IsDifferentFrom(const Settings &other) const
{
    return m_settings->m_windowTitle != other.m_windowTitle || m_settings->m_windowWidth != other.m_windowWidth ||
           m_settings->m_windowHeight != other.m_windowHeight ||
           m_settings->m_targetFramerate != other.m_targetFramerate ||
           m_settings->m_verticleSyncEnabled != other.m_verticleSyncEnabled ||
           m_settings->m_isFullscreen != other.m_isFullscreen || m_settings->m_masterVolume != other.m_masterVolume ||
           m_settings->m_musicVolume != other.m_musicVolume || m_settings->m_sfxVolume != other.m_sfxVolume ||
           m_settings->m_isMuted != other.m_isMuted || m_settings->m_audioDirectory != other.m_audioDirectory ||
           m_settings->m_fontDirectory != other.m_fontDirectory ||
           m_settings->m_spriteDirectory != other.m_spriteDirectory || m_settings->m_keyBindings != other.m_keyBindings;
}
