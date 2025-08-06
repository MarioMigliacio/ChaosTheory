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
#include "GunPattern.h"
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
//      - Provides Projectile firing shared logic so that inheriting classes
//        TryFire, and TryFireTowards is vastly simplified.
//
// ============================================================================
class ConfigurableGun : public BaseGun
{
  public:
    ConfigurableGun(const ProjectileStats &stats);
    virtual ~ConfigurableGun() = default;

    virtual std::shared_ptr<BaseProjectile> TryFire() override;
    virtual std::shared_ptr<BaseProjectile> TryFireTowards(const sf::Vector2f &targetPos) override;

    void UpgradeFireRate(float factor);
    void UpgradeVelocity(float value);
    void UpgradeDamageRate(float factor);
    void UpgradeDamageByFlat(float amount);
    void UpgradePattern();

    GunPattern GetPattern() const;
    void SetPattern(const GunPattern pattern);

    sf::Vector2f GetDefaultDirection() const;

    void Update(float dt) override;

  protected:
    std::shared_ptr<BaseProjectile> FireBasic(const sf::Vector2f &pos, const sf::Vector2f &dir);
    void FireSpread(const sf::Vector2f &pos, const sf::Vector2f &baseDir, const std::vector<float> &angles);
    std::shared_ptr<BaseProjectile> FireHomingRocket(const sf::Vector2f &pos);
    std::shared_ptr<BaseProjectile> FireGrowingBullet(const sf::Vector2f &pos);
    std::shared_ptr<BaseProjectile> FireLazerBeam(const sf::Vector2f &pos);
    void FireUltimateArc(const sf::Vector2f &pos);

  protected:
    ProjectileStats m_stats;
};
