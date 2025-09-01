// ============================================================================
//  File        : CrusaderShip.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-24
//  Description : A telegraphed laser attacker. Aligns to player's X, shakes,
//                glows red, then fires a lethal beam stream.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once
#include "BaseShip.h"
#include "CrusaderBehavior.h"
#include "ShipStatsScaling.h"

// ============================================================================
//  Class       : CrusaderShip
//  Purpose     : An aggressor spaceship that tracks the players x position
//                and upon lining up, begins to charge a laser beam stream.
//                This stream is extra fast, and fires at a deadly rate.
//
//  Responsibilities:
//      - Scale upon construction based on game difficulty and window size.
//      - Update position and aliveness for this spaceship.
//
// ============================================================================
class CrusaderShip : public BaseShip, public ShipStatsScaling, public CrusaderBehavior
{
  public:
    CrusaderShip(const sf::Vector2f &startPos, Allegiance allegiance);
    ~CrusaderShip() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    CrusaderShip(const CrusaderShip &) = delete;
    CrusaderShip &operator=(const CrusaderShip &) = delete;

    CrusaderShip(CrusaderShip &&) = delete;
    CrusaderShip &operator=(CrusaderShip &&) = delete;

  public:
    void Update(float dt) override;

  private:
    void InitializeGunStats() override;
};
