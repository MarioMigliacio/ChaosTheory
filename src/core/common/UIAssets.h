// ============================================================================
//  File        : UIAssets.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-09
//  Description : Hosts the namespace for UIAssets.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <string>
#include <unordered_map>

/// @brief Exposes Textures, Fonts, or other relevent assets to the UIAssets namespace.
namespace UIAssets
{
/// @brief Key to the UIArrow Texture Asset.
static auto UIArrowTextureKey = "UIArrow";

/// @brief Key to the UISkinnableButton: Blue-Idle Texture Asset.
static auto UISkinButtonBlueIdleKey = "BlueButtonIdle";

/// @brief Key to the UISkinnableButton: Blue-Hover Texture Asset.
static auto UISkinButtonBlueHoverKey = "BlueButtonHover";

/// @brief Key to the UISkinnableButton: Green-Idle Texture Asset.
static auto UISkinButtonGreenIdleKey = "GreenButtonIdle";

/// @brief Key to the UISkinnableButton: Green-Hover Texture Asset.
static auto UISkinButtonGreenHoverKey = "GreenButtonHover";

/// @brief Key to the UISkinnableButton: Red-Idle Texture Asset.
static auto UISkinButtonRedIdleKey = "RedButtonIdle";

/// @brief Key to the UISkinnableButton: Red-Hover Texture Asset.
static auto UISkinButtonRedHoverKey = "RedButtonHover";

/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"UIArrow", "assets/ui/arrow/UIArrow.png"},
    {"BlueButtonIdle", "assets/ui/button/blue/BlueButtonIdle.png"},
    {"BlueButtonHover", "assets/ui/button/blue/BlueButtonHover.png"},
    {"GreenButtonIdle", "assets/ui/button/green/GreenButtonIdle.png"},
    {"GreenButtonHover", "assets/ui/button/green/GreenButtonHover.png"},
    {"RedButtonIdle", "assets/ui/button/red/RedButtonIdle.png"},
    {"RedButtonHover", "assets/ui/button/red/RedButtonHover.png"},
};
} // namespace UIAssets