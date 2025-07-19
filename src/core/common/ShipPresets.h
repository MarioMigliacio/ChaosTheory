// ============================================================================
//  File        : ShipPresets.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-18
//  Description : Contains useful constants for Ship management
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// ============================================================================
// Color tint styles:
// ============================================================================

/// @brief Slight color tint to reflect Easy difficulty visually (light blue)
const sf::Color EASY_TINT = sf::Color(200, 200, 255);

/// @brief Slight color tint to reflect Normal difficulty visually (no tint)
const sf::Color NORMAL_TINT = sf::Color::White;

/// @brief Slight color tint to reflect Hard difficulty visually (Reddish tint)
const sf::Color HARD_TINT = sf::Color(255, 150, 150);

// ============================================================================
// Health and Speed values:
// ============================================================================

/// @brief Health scaling factor for Easy game difficulty.
constexpr float EASY_HEALTH_SCALE = 1.f;

/// @brief Speed scaling factor for Easy game difficulty.
constexpr float EASY_SPEED_SCALE = 1.f;

/// @brief Health scaling factor for Normal game difficulty.
constexpr float NORMAL_HEALTH_SCALE = 1.25f;

/// @brief Speed scaling factor for Normal game difficulty.
constexpr float NORMAL_SPEED_SCALE = 1.25f;

/// @brief Health scaling factor for Hard game difficulty.
constexpr float HARD_HEALTH_SCALE = 1.5f;

/// @brief Speed scaling factor for Hard game difficulty.
constexpr float HARD_SPEED_SCALE = 1.5f;
