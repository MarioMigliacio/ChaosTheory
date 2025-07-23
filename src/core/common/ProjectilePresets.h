// ============================================================================
//  File        : ProjectilePresets.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-23
//  Description : Contains useful constants for Projectile management
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// ============================================================================
// Damage and Speed values:
// ============================================================================

/// @brief Default Speed value for white projectile.
constexpr float WHITE_PROJECTILE_DEFAULT_SPD = 150.f;

/// @brief Default Damage value for white projectile.
constexpr int WHITE_PROJECTILE_DEFAULT_DMG = 10;

/// @brief Default Speed value for red projectile.
constexpr float RED_PROJECTILE_DEFAULT_SPD = 250.f;

/// @brief Default Damage value for red projectile.
constexpr int RED_PROJECTILE_DEFAULT_DMG = 8;

/// @brief Default Speed value for blue projectile.
constexpr float BLUE_PROJECTILE_DEFAULT_SPD = 100.f;

/// @brief Default Damage value for blue projectile.
constexpr int BLUE_PROJECTILE_DEFAULT_DMG = 15;

/// @brief Default Speed value for green projectile.
constexpr float GREEN_PROJECTILE_DEFAULT_SPD = 200.f;

/// @brief Default Damage value for green projectile.
constexpr int GREEN_PROJECTILE_DEFAULT_DMG = 12;

/// @brief Default Speed value for yellow projectile.
constexpr float YELLOW_PROJECTILE_DEFAULT_SPD = 250.f;

/// @brief Default Damage value for yellow projectile.
constexpr int YELLOW_PROJECTILE_DEFAULT_DMG = 12;

/// @brief Damage scaling factor for Easy game difficulty.
constexpr float EASY_PROJECTILE_DAMAGE_SCALE = 1.f;

/// @brief Speed scaling factor for Easy game difficulty.
constexpr float EASY_PROJECTILE_SPEED_SCALE = 1.f;

/// @brief Damage scaling factor for Normal game difficulty.
constexpr float NORMAL_PROJECTILE_DAMAGE_SCALE = 1.1f;

/// @brief Speed scaling factor for Normal game difficulty.
constexpr float NORMAL_PROJECTILE_SPEED_SCALE = 1.1f;

/// @brief Damage scaling factor for Hard game difficulty.
constexpr float HARD_PROJECTILE_DAMAGE_SCALE = 1.2f;

/// @brief Speed scaling factor for Hard game difficulty.
constexpr float HARD_PROJECTILE_SPEED_SCALE = 1.2f;
