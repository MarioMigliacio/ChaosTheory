// ============================================================================
//  File        : EnemyGun.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-31
//  Description : Enemy-specific firearm that applies difficulty scaling and
//                supports configurable projectile behavior.
//
//  License     : N/A Open source
// ============================================================================

#pragma once

#include "ConfigurableGun.h"
#include "SettingsManager.h"

/// @brief Constants that can be adjusted throughout the EnemyGun.
namespace
{
/// @brief Damage scaling constant for Easy mode.
constexpr float EASY_DMG_SCALE = 1.0f;

/// @brief Speed scaling constant for Easy mode.
constexpr float EASY_SPD_SCALE = 1.0f;

/// @brief Damage scaling constant for Normal mode.
constexpr float NORMAL_DMG_SCALE = 1.1f;

/// @brief Speed scaling constant for Normal mode.
constexpr float NORMAL_SPD_SCALE = 1.1f;

/// @brief Damage scaling constant for Hard mode.
constexpr float HARD_DMG_SCALE = 1.2f;

/// @brief Speed scaling constant for Hard mode.
constexpr float HARD_SPD_SCALE = 1.2f;
} // namespace

// ============================================================================
//  Class       : EnemyGun
//  Purpose     : Gun belonging to EnemyShip. Is modular and upgradable, with
//                Abstract separation of concern.
//
//  Responsibilities:
//      - Provide Upgrade interface and stats for Enemy Gun.
//      - Applies scaling for harder difficulties.
//
// ============================================================================
class EnemyGun : public ConfigurableGun
{
  public:
    /// @brief Constructor for the EnemyGun, Constructed first from ConfigurableGun for the common base.
    /// @param baseStats Stats structure for the Gun to base with.
    EnemyGun(const ProjectileStats &baseStats) : ConfigurableGun(baseStats)
    {
        ApplyDifficultyScaling();
    }

    /// @brief Upon gun construction, adjust the bullet damage and velocity accordingly for harder game difficulties.
    void ApplyDifficultyScaling()
    {
        GameDifficultySetting diff = SettingsManager::Instance().GetSettings()->m_gameDifficulty;

        switch (diff)
        {
            case GameDifficultySetting::Easy:
                m_stats.speed *= EASY_SPD_SCALE;
                m_stats.damage *= EASY_DMG_SCALE;
                break;
            case GameDifficultySetting::Normal:
                m_stats.speed *= NORMAL_SPD_SCALE;
                m_stats.damage *= NORMAL_DMG_SCALE;
                break;
            case GameDifficultySetting::Hard:
                m_stats.speed *= HARD_SPD_SCALE;
                m_stats.damage *= HARD_DMG_SCALE;
                break;
        }
    }
};