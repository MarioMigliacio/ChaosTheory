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
    /// @brief Scales health accordingly based on GameDifficulty.
    /// @param value health to scale.
    /// @return new health after scaling applied.
    int ScaleHealthToDifficulty(const int value)
    {
        int health = 0;
        const auto difficulty = SettingsManager::Instance().GetSettings()->m_gameDifficulty;

        switch (difficulty)
        {
            case GameDifficultySetting::Easy:
                health = static_cast<int>(value * EASY_HEALTH_SCALE);
                break;

            case GameDifficultySetting::Normal:
                health = static_cast<int>(value * NORMAL_HEALTH_SCALE);
                break;

            case GameDifficultySetting::Hard:
                health = static_cast<int>(value * HARD_HEALTH_SCALE);
                break;

            default:
                return value;
        }

        return health;
    }

    /// @brief Scales speed accordingly based on WindowResolution, and GameDifficulty.
    /// @param value speed to scale.
    /// @return new speed after scaling applied.
    sf::Vector2f ScaleSpeedToDifficulty(const sf::Vector2f &value)
    {
        sf::Vector2f speed = {0.f, 0.f};
        const auto difficulty = SettingsManager::Instance().GetSettings()->m_gameDifficulty;

        const auto scaleX = ResolutionScaleManager::Instance().GetScaleX();
        const auto scaleY = ResolutionScaleManager::Instance().GetScaleY();

        switch (difficulty)
        {
            case GameDifficultySetting::Easy:
                speed = {value.x * EASY_SPEED_SCALE * scaleX, value.y * EASY_SPEED_SCALE * scaleY};
                break;

            case GameDifficultySetting::Normal:
                speed = {value.x * NORMAL_SPEED_SCALE * scaleX, value.y * NORMAL_SPEED_SCALE * scaleY};
                break;

            case GameDifficultySetting::Hard:
                speed = {value.x * HARD_SPEED_SCALE * scaleX, value.y * HARD_SPEED_SCALE * scaleY};
                break;

            default:
                return value;
        }

        return speed;
    }

    /// @brief Sets the appropriate tint to the Sprite based on GameDifficulty.
    /// @param sprite The sprite to alter the tint for.
    void SetSpriteColorFromDifficulty(sf::Sprite &sprite)
    {
        const auto difficulty = SettingsManager::Instance().GetSettings()->m_gameDifficulty;

        switch (difficulty)
        {
            case GameDifficultySetting::Easy:
                sprite.setColor(EASY_TINT);
                break;

            case GameDifficultySetting::Normal:
                sprite.setColor(NORMAL_TINT);
                break;

            case GameDifficultySetting::Hard:
                sprite.setColor(HARD_TINT);
                break;
        }
    }
};