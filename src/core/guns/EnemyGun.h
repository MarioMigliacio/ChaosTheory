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
    EnemyGun(const ProjectileStats &baseStats);

    std::shared_ptr<BaseProjectile> TryFire() override;
    std::shared_ptr<BaseProjectile> TryFireTowards(const sf::Vector2f &targetPos) override;

    void ApplyDifficultyScaling();
};
