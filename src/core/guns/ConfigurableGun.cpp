// ============================================================================
//  File        : ConfigurableGun.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-01
//  Description : Implementation of ConfigurableGun with pattern upgrades,
//                stat upgrades, and shared firing logic.
//
//  License     : N/A Open source
// ============================================================================

#include "ConfigurableGun.h"
#include "ProjectileFactory.h"
#include "ProjectileManager.h"
#include <cmath>

/// @brief Constructor for a ConfigurableGun.
/// @param stats ProjectileStats to grant this ConfigurableGun.
ConfigurableGun::ConfigurableGun(const ProjectileStats &stats) : m_stats(stats)
{
}

/// @brief Manages the firing of projectile using the common base from ConfigurableGun.
/// @return Safe pointer to a BaseProjectile if successful, otherwise nullptr.
std::shared_ptr<BaseProjectile> ConfigurableGun::TryFire()
{
    if (m_cooldown > 0.f)
    {
        return nullptr;
    }

    m_cooldown = m_stats.fireRate;

    sf::Vector2f firePos = m_ownerPosition + m_barrelOffset;

    switch (m_stats.pattern)
    {
        case GunPattern::SingleShot:
            return FireBasic(firePos, GetDefaultDirection());

        case GunPattern::DoubleShot:
            FireSpread(firePos, GetDefaultDirection(), {-5.f, 5.f});

            return nullptr;

        case GunPattern::GrowingBullet:
            return FireGrowingBullet(firePos);

        case GunPattern::TripleShot:
            FireSpread(firePos, GetDefaultDirection(), {-8.f, 0.f, 8.f});

            return nullptr;

        case GunPattern::HomingRocket:
            return FireHomingRocket(firePos);

        case GunPattern::QuadShot:
            FireSpread(firePos, GetDefaultDirection(), {-10.f, -3.f, 3.f, 10.f});

            return nullptr;

        case GunPattern::LazerBeam:
            return FireLazerBeam(firePos);

        case GunPattern::LazerBeamDoubleShot:
            FireLazerBeam(firePos);
            FireSpread(firePos, GetDefaultDirection(), {-5.f, 5.f});

            return nullptr;

        case GunPattern::LazerBeamHomingRocket:
            FireLazerBeam(firePos);
            FireHomingRocket(firePos);

            return nullptr;

        case GunPattern::LazerBeamHybrid:
            FireLazerBeam(firePos);
            FireHomingRocket(firePos);
            FireSpread(firePos, GetDefaultDirection(), {-5.f, 5.f});

            return nullptr;

        case GunPattern::UltimateArcBeam:
            FireUltimateArc(firePos);

            return nullptr;
    }

    return nullptr;
}

/// @brief Manages the firing of a projectile that travels along a target vector direction.
/// @param targetPos The vector to travel along for the projectile.
/// @return Safe pointer to a BaseProjectile if successful, otherwise nullptr.
std::shared_ptr<BaseProjectile> ConfigurableGun::TryFireTowards(const sf::Vector2f &targetPos)
{
    if (m_cooldown > 0.f)
    {
        return nullptr;
    }

    m_cooldown = m_stats.fireRate;

    sf::Vector2f firePos = m_ownerPosition + m_barrelOffset;
    sf::Vector2f dir = targetPos - firePos;

    return FireBasic(firePos, dir);
}

/// @brief Performs a percentage upgrade on this guns current configured fireRate stat.
/// @note FireRate is a percentage, and lower means faster firing. So factor should be below 1.f in order to see
/// improved FireRate.
/// @param factor Percentage factor to multiply current fireRate by.
void ConfigurableGun::UpgradeFireRate(float factor)
{
    m_stats.fireRate *= factor;
}

/// @brief Performs a raw numerical upgrade on this guns current configured speed stat.
/// @param value Raw value to add to speed.
void ConfigurableGun::UpgradeVelocity(float value)
{
    m_stats.speed += value;
}

/// @brief Performs a percentage based upgrade on this guns current configured damage stat.
/// @param factor Percentage factor to multiply current damage by.
void ConfigurableGun::UpgradeDamageRate(float factor)
{
    m_stats.damage *= factor;
}

/// @brief Performs a raw numerical upgrade on this guns current configured damage stat.
/// @param value Raw value to add to damage.
void ConfigurableGun::UpgradeDamageByFlat(float amount)
{
    m_stats.damage += amount;
}

/// @brief Upgrades this guns current configured GunPattern mode.
void ConfigurableGun::UpgradePattern()
{
    m_stats.pattern = GetNextPattern(m_stats.pattern);
}

/// @brief Returns the currently configured GunPattern mode for this gun.
/// @return The GunPattern type which is set for this gun.
GunPattern ConfigurableGun::GetPattern() const
{
    return m_stats.pattern;
}

/// @brief Sets the currently configured GunPattern mode for this gun.
/// @param pattern GunPattern to be set.
void ConfigurableGun::SetPattern(const GunPattern pattern)
{
    m_stats.pattern = pattern;
}

/// @brief Helper method which easily returns the primary Up/Down mode of direction a bullet is designed to travel.
/// @return Up / Down vector direction, based on this guns allegiance (enemy or other).
sf::Vector2f ConfigurableGun::GetDefaultDirection() const
{
    return (m_allegiance == Allegiance::Enemy) ? sf::Vector2f(0.f, 1.f) : sf::Vector2f(0.f, -1.f);
}

/// @brief Performs routine update on the cooldown during a frame.
/// @param dt delta time since last update.
void ConfigurableGun::Update(float dt)
{
    if (m_cooldown > 0.f)
    {
        m_cooldown -= dt;

        if (m_cooldown < 0.f)
        {
            m_cooldown = 0.f;
        }
    }
}

/// @brief Fires the most basic of projectile types, and adds the projectile to the ProjectileManagers collection.
/// @param pos Position to spawn from.
/// @param dir Vector direction projectile will update with.
/// @return A safe pointer to the projectile, useful to indicate success to the caller.
std::shared_ptr<BaseProjectile> ConfigurableGun::FireBasic(const sf::Vector2f &pos, const sf::Vector2f &dir)
{
    auto proj = ProjectileFactory::Instance().CreateBasicProjectile(pos, dir, m_stats, m_allegiance);
    ProjectileManager::Instance().AddProjectile(proj);

    return proj;
}

/// @brief Fires a multitude of projectiles, and adds the projectile to the ProjectileManagers collection.
/// @note Will not return any projectiles, but will be added to the ProjectileManager.
/// @param pos Position to spawn from.
/// @param baseDir Vector direction projectile will traverse (up/down).
/// @param angles collection of angles for which the projectiles will spread out at during update.
void ConfigurableGun::FireSpread(const sf::Vector2f &pos, const sf::Vector2f &baseDir, const std::vector<float> &angles)
{
    for (float angleDeg : angles)
    {
        float baseAngle = std::atan2(baseDir.y, baseDir.x);
        float rad = angleDeg * 3.14159f / 180.f;
        sf::Vector2f dir(std::cos(baseAngle + rad), std::sin(baseAngle + rad));

        auto proj = ProjectileFactory::Instance().CreateBasicProjectile(pos, dir, m_stats, m_allegiance);
        ProjectileManager::Instance().AddProjectile(proj);
    }
}

/// @brief Fires a special projectile that will seek out its nearest target and explode.
/// @note Implementation is still TODO.
/// @param pos Position to spawn from.
/// @return A safe pointer to the projectile, useful to indicate success to the caller.
std::shared_ptr<BaseProjectile> ConfigurableGun::FireHomingRocket(const sf::Vector2f &pos)
{
    // TODO: Create a HomingProjectile subclass in future
    return FireBasic(pos, {0.f, -1.f});
}

/// @brief Fires a special projectile that will expand in size as it traverses space.
/// @note Implementation is still TODO.
/// @param pos Position to spawn from.
/// @return A safe pointer to the projectile, useful to indicate success to the caller.
std::shared_ptr<BaseProjectile> ConfigurableGun::FireGrowingBullet(const sf::Vector2f &pos)
{
    // TODO: Create GrowingProjectile subclass with scaling update
    return FireBasic(pos, {0.f, -1.f});
}

/// @brief Fires a special projectile that resembles a continuous stream of beaming light.
/// @note Implementation is still TODO.
/// @param pos Position to spawn from.
/// @return A safe pointer to the projectile, useful to indicate success to the caller.
std::shared_ptr<BaseProjectile> ConfigurableGun::FireLazerBeam(const sf::Vector2f &pos)
{
    // TODO: Implement continuous beam entity
    return FireBasic(pos, {0.f, -1.f});
}

/// @brief Fires a penultimate projectile that will do devastation in its wake. Behold it to believe it.
/// @note Implementation is still TODO.
/// @param pos Position to spawn from.
void ConfigurableGun::FireUltimateArc(const sf::Vector2f &pos)
{
    // TODO: Fire a sweeping arc pattern + side shots
    FireSpread(pos, {0.f, -1.f}, {-30.f, -15.f, 0.f, 15.f, 30.f});
}
