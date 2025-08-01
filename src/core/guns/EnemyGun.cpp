// ============================================================================
//  File        : EnemyGun.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-31
//  Description : Enemy-specific firearm that applies difficulty scaling and
//                supports configurable projectile behavior.
//
//  License     : N/A Open source
// ============================================================================

#include "EnemyGun.h"
#include "ProjectileFactory.h"
#include "SettingsManager.h"
#include <cmath>

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

/// @brief Constructor for the EnemyGun, Constructed first from ConfigurableGun for the common base.
/// @param baseStats Stats structure for the Gun to base with.
EnemyGun::EnemyGun(const ProjectileStats &baseStats) : ConfigurableGun(baseStats)
{
    ApplyDifficultyScaling();
}

/// @brief Manages the firing of projectile using the common base from ConfigurableGun.
/// @note FireProjectiles method already registers the projectile to the ProjectileManager, DO NOT register here again.
/// @return Safe pointer to a BaseProjectile if successful, otherwise nullptr.
std::shared_ptr<BaseProjectile> EnemyGun::TryFire()
{
    if (m_cooldown > 0.f)
    {
        return nullptr;
    }

    m_cooldown = m_stats.fireRate;

    sf::Vector2f firePos = m_ownerPosition + m_barrelOffset;
    sf::Vector2f dir(0.f, 1.f); // Enemies fire downward by default

    return FireProjectiles(firePos, dir);
}

/// @brief Manages the firing of projectile towards a target using the common base from ConfigurableGun.
/// @note FireProjectiles method already registers the projectile to the ProjectileManager, DO NOT register here again.
/// @return Safe pointer to a BaseProjectile if successful, otherwise nullptr.
std::shared_ptr<BaseProjectile> EnemyGun::TryFireTowards(const sf::Vector2f &targetPos)
{
    if (m_cooldown > 0.f)
    {
        return nullptr;
    }

    m_cooldown = m_stats.fireRate;

    sf::Vector2f firePos = m_ownerPosition + m_barrelOffset;
    sf::Vector2f dir = targetPos - firePos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len > 0.f)
    {
        dir /= len;
    }

    else
    {
        dir = {0.f, 1.f};
    }

    return FireProjectiles(firePos, dir);
}

/// @brief Upon gun construction, adjust the bullet damage and velocity accordingly for harder game difficulties.
void EnemyGun::ApplyDifficultyScaling()
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
