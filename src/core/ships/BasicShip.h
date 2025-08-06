// ============================================================================
//  File        : BasicShip.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-17
//  Description : Basic ship type object logic.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseShip.h"
#include "ShipStatsScaling.h"

// ============================================================================
//  Class       : BasicShip
//  Purpose     : An unintelligent spaceship that moves moderately fast down Y axis.
//
//  Responsibilities:
//      - Scale upon construction based on game difficulty and window size.
//      - Update position and aliveness for this spaceship.
//
// ============================================================================
class BasicShip : public BaseShip, public ShipStatsScaling
{
  public:
    BasicShip(const sf::Vector2f &startPos, Allegiance allegiance);
    ~BasicShip() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    BasicShip(const BasicShip &) = delete;
    BasicShip &operator=(const BasicShip &) = delete;

    BasicShip(BasicShip &&) = delete;
    BasicShip &operator=(BasicShip &&) = delete;

  public:
    void Update(float dt) override;

  private:
    void InitializeGunStats() override;
};