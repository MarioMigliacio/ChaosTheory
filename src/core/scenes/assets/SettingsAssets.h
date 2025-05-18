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

/// @brief Exposes Textures, Fonts, and Audio assets to the SettingsAssets namespace.
namespace SettingsAssets
{
/// @brief Key to the SettingsSound Asset.
constexpr auto SettingsSound = "SettingsSound";

/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"PlainStarBackground", "assets/backgrounds/PlainStarBackground.png"},
    {"GasPattern3", "assets/backgrounds/GasPattern3.png"},
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
