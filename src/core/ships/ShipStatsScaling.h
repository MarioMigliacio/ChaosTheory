// ============================================================================
//  File        : ShipStatsScaling.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-31
//  Description : Provides an easy interface for Enemy ships to inherit for scaling.

//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "ResolutionScaleManager.h"
#include "SettingsManager.h"

/// @brief Constants that can be adjusted throughout the AlienShip.
namespace
{
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

/// @brief Slight color tint to reflect Easy difficulty visually (light blue)
const sf::Color EASY_TINT = sf::Color(200, 200, 255);

/// @brief Slight color tint to reflect Normal difficulty visually (no tint)
const sf::Color NORMAL_TINT = sf::Color::White;

/// @brief Slight color tint to reflect Hard difficulty visually (Reddish tint)
const sf::Color HARD_TINT = sf::Color(255, 150, 150);
} // namespace

// ============================================================================
//  Class       : ShipStatsScaling
//  Purpose     : Scale stats based on game difficulty and window size.
//
//  Responsibilities:
//      - provides an easy interface for Enemy ships to inherit for scaling.
//
// ============================================================================
class ShipStatsScaling
{
  public:
    /// @brief Scales stats accordingly based on WindowResolution, and GameDifficulty.
    /// @param health health to scale.
    /// @param speed speed to scale.
    /// @param sprite sprite to adjust based on difficulty scale.
    virtual void ApplyDifficultyScaling(int &health, sf::Vector2f &speed, sf::Sprite &sprite)
    {
        const auto difficulty = SettingsManager::Instance().GetSettings()->m_gameDifficulty;
        const auto scaleX = ResolutionScaleManager::Instance().GetScaleX();
        const auto scaleY = ResolutionScaleManager::Instance().GetScaleY();

        switch (difficulty)
        {
            case GameDifficultySetting::Easy:
                health = static_cast<int>(health * EASY_HEALTH_SCALE);
                speed = {speed.x * EASY_SPEED_SCALE * scaleX, speed.y * EASY_SPEED_SCALE * scaleY};
                sprite.setColor(EASY_TINT);
                break;

            case GameDifficultySetting::Normal:
                health = static_cast<int>(health * NORMAL_HEALTH_SCALE);
                speed = {speed.x * NORMAL_SPEED_SCALE * scaleX, speed.y * NORMAL_SPEED_SCALE * scaleY};
                sprite.setColor(NORMAL_TINT);
                break;

            case GameDifficultySetting::Hard:
                health = static_cast<int>(health * HARD_HEALTH_SCALE);
                speed = {speed.x * HARD_SPEED_SCALE * scaleX, speed.y * HARD_SPEED_SCALE * scaleY};
                sprite.setColor(HARD_TINT);
                break;
        }
    }
};