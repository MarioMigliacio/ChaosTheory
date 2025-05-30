// ============================================================================
//  File        : GameAssets.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-21
//  Description : Hosts the namespace for GameAssets
//                GameAssets are used in the GameScene.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <string>
#include <unordered_map>

/// @brief Exposes Textures, Fonts, and Audio assets to the GameAssets namespace.
namespace GameAssets
{
/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"PlainStarBackground", "assets/backgrounds/PlainStarBackground.png"},
    {"GasPattern1", "assets/backgrounds/GasPattern1.png"},
    {"GasPattern2", "assets/backgrounds/GasPattern2.png"},
};

/// @brief Fonts contain a Key and Value pair collection of font assets
static const std::unordered_map<std::string, std::string> Fonts = {
    {"Default", "assets/fonts/Default.ttf"},
    {"GameFont", "assets/fonts/Default.ttf"},
};
} // namespace GameAssets
