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
#include "ShipStatsScaling.h"
#include <random>

// ============================================================================
//  Class       : AlienShip
//  Purpose     : An unintelligent spaceship that moves randomly across X axis,
//                and slowly down Y axis.
//
//  Responsibilities:
//      - Scale upon construction based on game difficulty and window size.
//      - Update position and aliveness for this spaceship.
//
// ============================================================================
class AlienShip : public BaseShip, public ShipStatsScaling
{
  public:
    AlienShip(const sf::Vector2f &startPos, Allegiance allegiance);
    ~AlienShip() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    AlienShip(const AlienShip &) = delete;
    AlienShip &operator=(const AlienShip &) = delete;

    AlienShip(AlienShip &&) = delete;
    AlienShip &operator=(AlienShip &&) = delete;

  public:
    void Update(float dt) override;

  private:
    void UpdateMovementLogic(const float dt);
    void UpdateGunLogic(const float dt);
    void InitializeGunStats() override;

  private:
    std::default_random_engine m_rng;
    std::uniform_int_distribution<int> m_directionDist{0, 1};
    std::uniform_real_distribution<float> m_fireDelayDist{1.0f, 1.5f};

    float m_directionChangeCooldown = 0.f;
    float m_currentDirection = 1.f;
    float m_initialHoldTime = 1.f;
};
