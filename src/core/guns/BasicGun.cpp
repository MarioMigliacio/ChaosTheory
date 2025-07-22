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
#include "WindowManager.h"

BasicGun::BasicGun(float cooldownSeconds, Allegiance allegiance)
    : m_fireRate(cooldownSeconds), m_cooldown(0.f), m_allegiance(allegiance)
{
    m_barrelOffset = ProjectileFactory::Instance().GetDefaultBarrelOffset(allegiance);
}

void BasicGun::Update(float dt)
{
    m_cooldown -= dt;

    if (m_cooldown < 0.f)
    {
        m_cooldown = 0.f;
    }
}

void BasicGun::SetOwnerPosition(const sf::Vector2f &position)
{
    m_ownerPosition = position;
}

sf::Vector2f BasicGun::GetBarrelOffset() const
{
    return m_barrelOffset;
}

void BasicGun::SetAllegiance(Allegiance allegiance)
{
    m_allegiance = allegiance;
    m_barrelOffset = ProjectileFactory::Instance().GetDefaultBarrelOffset(allegiance);
}

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
