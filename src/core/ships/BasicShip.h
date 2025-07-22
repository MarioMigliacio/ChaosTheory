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

// ============================================================================
//  Class       : BasicShip
//  Purpose     : An unintelligent spaceship that moves moderately fast down Y axis.
//
//  Responsibilities:
//      - Scale upon construction based on game difficulty and window size.
//      - Update position and aliveness for this spaceship.
//
// ============================================================================
class BasicShip : public BaseShip
{
  public:
    BasicShip(const sf::Vector2f &startPos, Allegiance allegiance);
    ~BasicShip() override = default;

    void Update(float dt) override;

  protected:
    void ApplyDifficultyScaling() override;
};