// ============================================================================
//  File        : GruntShip.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-05
//  Description : A small unit that hops locally and fires aimed shots.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Allegiance.h"
#include "BaseShip.h"
#include "DodgeAndAimBehavior.h"

// ============================================================================
//  Class       : GruntShip
//  Purpose     : A travelling spaceship that hops to a random local location,
//                shoots a directed shot towards the player, and then cools
//                down by travelling downwards until dead or off screen.
//
//  Responsibilities:
//      - Scale upon construction based on game difficulty and window size.
//      - Update position and aliveness for this spaceship.
//
// ============================================================================
class GruntShip : public BaseShip, public DodgeAndAimBehavior
{
  public:
    GruntShip(const sf::Vector2f &startPos, Allegiance allegiance);
    ~GruntShip() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    GruntShip(const GruntShip &) = delete;
    GruntShip &operator=(const GruntShip &) = delete;

  public:
    void Update(float dt) override;

  private:
    void InitializeGunStats();
};
