// ============================================================================
//  File        : MainMenuAssets.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-09
//  Description : Hosts the namespace for MainMenuAssets
//                MainMenuAssets are used in the MainMenuScene.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <string>
#include <unordered_map>

namespace MainMenuAssets
{
/// @brief Path for the MenuSong.
constexpr auto MenuSong = "assets/audio/RootMenu.wav";

/// @brief Key to the MenuFont Asset.
constexpr auto MenuFont = "MenuFont";

/// @brief Key to the MenuBackground Asset.
constexpr auto MenuBackground = "MenuBackground";

/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"MenuBackground", "assets/backgrounds/NebulaBackground.png"},
};

/// @brief Fonts contain a Key and Value pair collection of font assets
static const std::unordered_map<std::string, std::string> Fonts = {
    {"Default.ttf", "assets/fonts/Default.ttf"},
    {"MenuFont", "assets/fonts/Default.ttf"},
};
} // namespace MainMenuAssets
