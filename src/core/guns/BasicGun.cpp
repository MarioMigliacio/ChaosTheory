// ============================================================================
//  File        : BasicGun.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-19
//  Description : Base implementation for firearm behavior across all ships.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "BasicGun.h"
#include "ProjectileFactory.h"
#include "ProjectileManager.h"

/// @brief Constructor for a BasicGun
/// @param cooldownSeconds Amount of delta time to pass before able to successfully TryFire variants.
/// @param allegiance Allegiance for the unit this Gun is attached to.
BasicGun::BasicGun(float cooldownSeconds, Allegiance allegiance)
{
    m_barrelOffset = GetDefaultBarrelOffset(allegiance);

    m_fireRate = cooldownSeconds;
    m_cooldown = 0.f;
    m_allegiance = allegiance;
}

/// @brief Updates cooldown logic during a frame.
/// @param dt delta time since last update frame.
void BasicGun::Update(float dt)
{
    m_cooldown -= dt;

    if (m_cooldown < 0.f)
    {
        m_cooldown = 0.f;
    }
}

/// @brief Attempts to Try firing a projectile from this gun.
/// @return a safe pointer to a base class projectile, or nullptr if not ready to fire.
std::shared_ptr<BaseProjectile> BasicGun::TryFire()
{
    if (m_cooldown > 0.f)
    {
        return nullptr;
    }

    m_cooldown = m_fireRate;

    sf::Vector2f firePos = m_ownerPosition + m_barrelOffset;

    // TODO: parameterize the projectileCategory, we might not always default white.
    auto projectile =
        ProjectileFactory::Instance().CreateBasicProjectile(firePos, ProjectileCategory::White, m_allegiance);
    ProjectileManager::Instance().AddProjectile(projectile);

    return projectile;
}

/// @brief Attempts to Try firing a projectile towards a target position from this gun.
/// @param targetPos The Vector position this projectile will try to fire towards.
/// @return a safe pointer to a base class projectile, or nullptr if not ready to fire.
std::shared_ptr<BaseProjectile> BasicGun::TryFireTowards(const sf::Vector2f &targetPos)
{
    if (m_cooldown > 0.f)
    {
        return nullptr;
    }

    m_cooldown = m_fireRate;

    sf::Vector2f firePos = m_ownerPosition + m_barrelOffset;
    sf::Vector2f direction = targetPos - firePos;

    // TODO: parameterize the projectileCategory, we might not always default red.
    auto projectile =
        ProjectileFactory::Instance().CreateBasicProjectile(firePos, direction, ProjectileCategory::Red, m_allegiance);
    ProjectileManager::Instance().AddProjectile(projectile);

    return projectile;
}
