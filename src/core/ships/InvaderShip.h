// ============================================================================
//  File        : InvaderShip.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-07
//  Description : Enemy that executes a two-volley multishot barrage with
//                slight angle change between volleys.
// ============================================================================

#pragma once

#include "Allegiance.h"
#include "BaseShip.h"
#include "MultishotBarrageBehavior.h"

// ============================================================================
//  Class       : InvaderShip
//  Purpose     : An aggressive ship that fires barrages of multi shots
//                directed towards the player, and then cools
//                down and repeats until dead or off screen.
//
//  Responsibilities:
//      - Scale upon construction based on game difficulty and window size.
//      - Update position and aliveness for this spaceship.
//
// ============================================================================
class InvaderShip : public BaseShip, public MultishotBarrageBehavior
{
  public:
    InvaderShip(const sf::Vector2f &startPos, Allegiance allegiance);
    ~InvaderShip() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    InvaderShip(const InvaderShip &) = delete;
    InvaderShip &operator=(const InvaderShip &) = delete;

  public:
    void Update(float dt) override;

  private:
    void InitializeGunStats();
};
