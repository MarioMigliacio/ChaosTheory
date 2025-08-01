// ============================================================================
//  File        : ConfigurableGun.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-31
//  Description : Abstracted Base logic for guns which will be upgradable.
//
//  License     : N/A Open source
// ============================================================================

#pragma once

#include "BaseGun.h"
#include "ProjectileManager.h"
#include "ProjectileStats.h"

// ============================================================================
//  Class       : ConfigurableGun
//  Purpose     : An abstraction layer above BaseGun interface,
//                for shared common projectile behaviors for upgradable guns.
//
//  Responsibilities:
//      - Provides ProjectileStats for inheriting classes.
//      - Provides interface for Upgrade stats on the internal ProjectileStats.
//      - Provides common update method.
//      - Provides FireProjectiles shared logic so that inheriting classes
//        TryFire, and TryFireTowards is vastly simplified.
//
// ============================================================================
class ConfigurableGun : public BaseGun
{
  public:
    /// @brief Constructor which registers the ProjectileStats for this ConfigurableGun.
    /// @param stats ProjectileStats struct which holds gun attributes for projectiles.
    ConfigurableGun(const ProjectileStats &stats) : m_stats(stats)
    {
    }

    /// @brief Sets the potential projectile FireRate.
    void UpgradeFireRate(float factor)
    {
        m_stats.fireRate *= factor;
    }

    /// @brief Sets the potential projectile Damage.
    void UpgradeDamage(float factor)
    {
        m_stats.damage *= factor;
    }

    /// @brief Sets the potential projectile Velocity.
    void UpgradeVelocity(float factor)
    {
        m_stats.speed *= factor;
    }

    /// @brief Sets the potential amount of ProjectilesPerShot.
    void SetProjectilesPerShot(int count)
    {
        m_stats.projectilesPerShot = std::max(1, count);
    }

    /// @brief Perform routine update logic on cooldowns during a frame.
    void Update(float dt) override
    {
        if (m_cooldown > 0.f)
        {
            m_cooldown -= dt;
        }
    }

  protected:
    /// @brief Shared firing logic — can be reused by UpgradableGun and EnemyGun
    /// @param firePos Vector position from which projectile spawns from.
    /// @param baseDir Vector direction for which projectile will travel at.
    /// @return Safe pointer to a BaseProjectile.
    std::shared_ptr<BaseProjectile> FireProjectiles(const sf::Vector2f &firePos, const sf::Vector2f &baseDir)
    {
        std::shared_ptr<BaseProjectile> lastProjectile = nullptr;

        if (m_stats.projectilesPerShot <= 1)
        {
            lastProjectile =
                ProjectileFactory::Instance().CreateBasicProjectile(firePos, baseDir, m_stats, m_allegiance);
            ProjectileManager::Instance().AddProjectile(lastProjectile);

            return lastProjectile;
        }

        float spreadAngle = 8.f * (m_stats.projectilesPerShot - 1);
        float startAngle = -spreadAngle / 2.f;
        float angleStep = spreadAngle / (m_stats.projectilesPerShot - 1);
        float baseAngle = std::atan2(baseDir.y, baseDir.x);

        for (int i = 0; i < m_stats.projectilesPerShot; ++i)
        {
            float angle = baseAngle + (startAngle + i * angleStep) * (3.14159f / 180.f);
            sf::Vector2f spreadDir(std::cos(angle), std::sin(angle));

            lastProjectile =
                ProjectileFactory::Instance().CreateBasicProjectile(firePos, spreadDir, m_stats, m_allegiance);
            ProjectileManager::Instance().AddProjectile(lastProjectile);
        }

        return lastProjectile;
    }

  protected:
    ProjectileStats m_stats;
};
