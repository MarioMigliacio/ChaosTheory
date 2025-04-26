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
    bool LoadDefaultsFromConfig(const std::string &filepath);
    bool SaveToFile(const std::string &path) const;

    std::shared_ptr<Settings> GetSettings();

  private:
    SettingsManager() = default;
    ~SettingsManager() = default;

    SettingsManager(const SettingsManager &) = delete;
    SettingsManager &operator=(const SettingsManager &) = delete;

  private:
    std::shared_ptr<Settings> m_settings = nullptr;
    bool m_isInitialized = false;
};
