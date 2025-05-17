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

/// @brief Get the current Instance for this SettingsManager singleton.
/// @return reference to existing SettingsManager interface.
SettingsManager &SettingsManager::Instance()
{
    static SettingsManager instance;
    return instance;
}

/// @brief Initializes this SettingsManager, owning the Settings configurations.
/// @param settings Shared pointer to settings object.
void SettingsManager::Init(std::shared_ptr<Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = std::move(settings);
    m_isInitialized = true;

    CT_LOG_INFO("SettingsManager initialized.");
}

/// @brief Resets the internal states for the SettingsManager.
void SettingsManager::Shutdown()
{
    m_settings.reset();
    m_isInitialized = false;
}

/// @brief Returns whether or not the SettingsManager has been initialized.
/// @return m_isInitialized.
bool SettingsManager::IsInitialized() const
{
    return m_isInitialized;
}

/// @brief Loads in the essential default settings config file into this Settings object.
/// @param filepath file path to config file.
/// @return success = true / false.
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

/// @brief Saves the settings object to file for future use.
/// @param path file path to config file.
/// @return success = true / false.
bool SettingsManager::SaveToFile(const std::string &path) const
{
    CT_WARN_IF_UNINITIALIZED_RET("SettingsManager", "SaveToFile", false);

    if (!m_settings)
    {
        return false;
    }

    bool result = ConfigLoader::SaveAsJson(path, *m_settings);

    CT_LOG_INFO("SettingsManager saved to file: {}", path);

    return result;
}

/// @brief Returns the reference for the Settings object.
/// @return m_settings.
std::shared_ptr<Settings> SettingsManager::GetSettings()
{
    return m_settings;
}

/// @brief Similar to == overload, but function form: Is parameter settings different then m_settings?
/// @param other compare settings against.
/// @return true = is different, false = is same.
bool SettingsManager::IsDifferentFrom(const Settings &other) const
{
    return m_settings->m_windowTitle != other.m_windowTitle || m_settings->m_windowWidth != other.m_windowWidth ||
           m_settings->m_windowHeight != other.m_windowHeight || m_settings->m_resolution != other.m_resolution ||
           m_settings->m_targetFramerate != other.m_targetFramerate ||
           m_settings->m_verticleSyncEnabled != other.m_verticleSyncEnabled ||
           m_settings->m_isFullscreen != other.m_isFullscreen || m_settings->m_masterVolume != other.m_masterVolume ||
           m_settings->m_musicVolume != other.m_musicVolume || m_settings->m_sfxVolume != other.m_sfxVolume ||
           m_settings->m_isMuted != other.m_isMuted || m_settings->m_gameDifficulty != other.m_gameDifficulty ||
           m_settings->m_audioDirectory != other.m_audioDirectory ||
           m_settings->m_fontDirectory != other.m_fontDirectory ||
           m_settings->m_spriteDirectory != other.m_spriteDirectory || m_settings->m_keyBindings != other.m_keyBindings;
}
