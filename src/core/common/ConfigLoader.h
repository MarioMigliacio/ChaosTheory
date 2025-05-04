// ============================================================================
//  File        : ConfigLoader.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-19
//  Description : Json settings file to assist in default loading
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "SettingsManager.h"
#include <string>

// ============================================================================
//  Class       : ConfigLoader
//  Purpose     : Assists in the main loading for the settings file.
//  Responsibilities:
//      - Singular LoadFromJson capability
//
// ============================================================================
class ConfigLoader
{
  public:
    static bool LoadFromJson(const std::string &filepath, Settings &settings);
    static bool SaveAsJson(const std::string &filepath, const Settings &settings);
};
