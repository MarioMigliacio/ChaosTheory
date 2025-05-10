// ============================================================================
//  File        : SplashAssets.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-09
//  Description : Hosts the namespace for SplashAssets
//                SplashAssets are used in the SplashScene.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <string>
#include <unordered_map>

namespace SplashAssets
{
/// @brief Key to the SplashBackground Asset.
constexpr auto SplashBackground = "SplashBackground";

/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"SplashBackground", "assets/backgrounds/ChaosTheorySplash.png"},
};
} // namespace SplashAssets
