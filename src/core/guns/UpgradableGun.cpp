// ============================================================================
//  File        : UpgradableGun.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-31
//  Description : Player-specific firearm that supports configurable
//                projectile behavior.
//
//  License     : N/A Open source
// ============================================================================

#include "UpgradableGun.h"
#include "ProjectileFactory.h"
#include "ProjectileManager.h"
#include <cmath>

/// @brief Constructor for the UpgradableGun, Constructed first from ConfigurableGun for the common base.
/// @param baseStats Stats structure for the Gun to base with.
UpgradableGun::UpgradableGun(const ProjectileStats &initialStats) : ConfigurableGun(initialStats)
{
}

/// @brief Manages the firing of projectile using the common base from ConfigurableGun.
/// @note FireProjectiles method already registers the projectile to the ProjectileManager, DO NOT register here again.
/// @return Safe pointer to a BaseProjectile if successful, otherwise nullptr.
std::shared_ptr<BaseProjectile> UpgradableGun::TryFire()
{
    if (m_cooldown > 0.f)
    {
        return nullptr;
    }

    m_cooldown = m_stats.fireRate;

    sf::Vector2f firePos = m_ownerPosition + m_barrelOffset;
    sf::Vector2f dir(0.f, -1.f); // Player always fires upward

    return FireProjectiles(firePos, dir);
}

/// @brief Manages the firing of projectile towards a target using the common base from ConfigurableGun.
/// @note FireProjectiles method already registers the projectile to the ProjectileManager, DO NOT register here again.
/// @return Safe pointer to a BaseProjectile if successful, otherwise nullptr.
std::shared_ptr<BaseProjectile> UpgradableGun::TryFireTowards(const sf::Vector2f &targetPos)
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
        dir = {0.f, -1.f};
    }

    return FireProjectiles(firePos, dir);
}
