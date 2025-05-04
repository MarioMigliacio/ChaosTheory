// ============================================================================
//  File        : SettingsManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-22
//  Description : Singleton class managing runtime and persistent settings.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Settings.h"
#include <memory>
#include <string>

// ============================================================================
//  Class       : SettingsManager
//  Purpose     : Singleton for accessing and managing global Settings.
//
//  Responsibilities:
//      - Load default config from file
//      - Provide runtime-safe access to mutable/shared Settings
//      - Encapsulate persistence logic if needed
//
// ============================================================================
class SettingsManager
{
  public:
    static SettingsManager &Instance();

    void Init(std::shared_ptr<Settings> settings);
    void Shutdown();

    bool IsInitialized() const;
    bool LoadFromFile(const std::string &filepath);
    bool SaveToFile(const std::string &path) const;

    std::shared_ptr<Settings> GetSettings();
    bool IsDifferentFrom(const Settings &other) const;

  private:
    SettingsManager() = default;
    ~SettingsManager() = default;

    SettingsManager(const SettingsManager &) = delete;
    SettingsManager &operator=(const SettingsManager &) = delete;

  private:
    std::shared_ptr<Settings> m_settings = nullptr;
    bool m_isInitialized = false;
};

// Returns a string from a ResolutionSettings enumeration
inline std::string ToString(ResolutionSetting setting)
{
    switch (setting)
    {
        case ResolutionSetting::Res480p:
            return "480p";
        case ResolutionSetting::Res720p:
        default: // fallback
            return "720p";
        case ResolutionSetting::Res1080p:
            return "1080p";
        case ResolutionSetting::Fullscreen:
            return "Fullscreen";
    }
}

// Returns a ResultionSetting enumeration from a string.
inline ResolutionSetting FromStringToResolution(const std::string &str)
{
    if (str == "480p")
    {
        return ResolutionSetting::Res480p;
    }

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