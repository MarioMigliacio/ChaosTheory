// ============================================================================
//  File        : BerserkerShip.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-19
//  Description : Fast kamikaze ship. Locks direction to player's position at
//                spawn and flies straight along that vector (no homing).
//
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseShip.h"
#include "BerserkerBehavior.h"
#include "ShipStatsScaling.h"

// ============================================================================
//  Class       : BerserkerShip
//  Purpose     : An aggressor spaceship that moves moderately fast towards
//                the players location at the time of spawning.
//
//  Responsibilities:
//      - Scale upon construction based on game difficulty and window size.
//      - Update position and aliveness for this spaceship.
//
// ============================================================================
class BerserkerShip : public BaseShip, public ShipStatsScaling, public BerserkerBehavior
{
  public:
    BerserkerShip(const sf::Vector2f &startPos, Allegiance allegiance);
    ~BerserkerShip() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    BerserkerShip(const BerserkerShip &) = delete;
    BerserkerShip &operator=(const BerserkerShip &) = delete;

    BerserkerShip(BerserkerShip &&) = delete;
    BerserkerShip &operator=(BerserkerShip &&) = delete;

  public:
    void Update(float dt) override;

  private:
    void InitializeGunStats() override;
};
