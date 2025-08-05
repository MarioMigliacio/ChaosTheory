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
    /// @brief Constructor for the UpgradableGun, Constructed first from ConfigurableGun for the common base.
    /// @param baseStats Stats structure for the Gun to base with.
    UpgradableGun(const ProjectileStats &initialStats) : ConfigurableGun(initialStats)
    {
    }
};
