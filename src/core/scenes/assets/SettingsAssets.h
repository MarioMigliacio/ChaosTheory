// ============================================================================
//  File        : SettingsAssets.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-09
//  Description : Hosts the namespace for SettingsAssets
//                SettingsAssets are used in the SettingsScene.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <string>
#include <unordered_map>

namespace SettingsAssets
{
/// @brief Key to the SettingsBackground Asset.
constexpr auto SettingsBackground = "SettingsBackground";

/// @brief Key to the SettingsSound Asset.
constexpr auto SettingsSound = "SettingsSound";

/// @brief Key to the SettingsFont Asset.
constexpr auto SettingsFont = "SettingsFont";

/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"SettingsBackground", "assets/backgrounds/DarkStarryBackground.png"},
};

/// @brief Sounds contain a Key and Value pair collection of audio sfx assets
static const std::unordered_map<std::string, std::string> Sounds = {
    {"SettingsSound", "assets/audio/PewPew.wav"},
};

/// @brief Fonts contain a Key and Value pair collection of font assets
static const std::unordered_map<std::string, std::string> Fonts = {
    {"Default.ttf", "assets/fonts/Default.ttf"},
    {"SettingsFont", "assets/fonts/Default.ttf"},
};
} // namespace SettingsAssets
