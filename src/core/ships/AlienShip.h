// ============================================================================
//  File        : AlienShip.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-18
//  Description : A fast-moving alien ship that zig-zags across the screen.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseShip.h"
#include <random>

class AlienShip : public BaseShip
{
  public:
    AlienShip(const sf::Vector2f &startPos, int allegiance);
    ~AlienShip() override = default;

    void Update(float dt) override;

  protected:
    void ApplyDifficultyScaling() override;

  private:
    std::default_random_engine m_rng;
    std::uniform_int_distribution<int> m_directionDist{0, 1};

    float m_directionChangeCooldown = 0.f;
    float m_currentDirection = 1.f;
};
