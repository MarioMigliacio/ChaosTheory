// ============================================================================
//  File        : UpgradableGun.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-31
//  Description : Player-specific firearm that supports configurable
//                projectile behavior.
//
//  License     : N/A Open source
// ============================================================================

#pragma once

#include "ConfigurableGun.h"

// ============================================================================
//  Class       : UpgradableGun
//  Purpose     : Gun belonging to PlayerShip. Is modular and upgradable, with
//                Abstract separation of concern.
//
//  Responsibilities:
//      - Provide Upgrade interface and stats for Player Gun.
//
// ============================================================================
class UpgradableGun : public ConfigurableGun
{
  public:
    UpgradableGun(const ProjectileStats &initialStats);

    std::shared_ptr<BaseProjectile> TryFire() override;
    std::shared_ptr<BaseProjectile> TryFireTowards(const sf::Vector2f &targetPos) override;
};
