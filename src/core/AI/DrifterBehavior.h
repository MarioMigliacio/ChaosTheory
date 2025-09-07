// ============================================================================
//  File        : DrifterBehavior.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-18
//  Description : Drift behavior. Ship moves steadily along a vector, usually
//                downward, with RNG-based horizontal offset. No shooting.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "IBehavior.h"
#include <SFML/Graphics.hpp>
#include <random>

// ============================================================================
//  Class       : DrifterBehavior
//  Purpose     : Implements IBehavior interface for a Drifter behavior:
//                Moves in a vector direction down stream, without shooting.
//
//  Responsibilities:
//      - Update logic for movement.
//      - Update logic for gun (not relevant in this behavior).
//
// ============================================================================
class DrifterBehavior : public IBehavior
{
  public:
    DrifterBehavior() = default;
    ~DrifterBehavior() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    DrifterBehavior(const DrifterBehavior &) = delete;
    DrifterBehavior &operator=(const DrifterBehavior &) = delete;

    DrifterBehavior(DrifterBehavior &&) = delete;
    DrifterBehavior &operator=(DrifterBehavior &&) = delete;

  protected:
    void UpdateMovementLogic(BaseShip &ship, float dt) override;
    void UpdateGunLogic(BaseShip &ship, float dt) override;

  private:
    std::mt19937 m_rng{std::random_device{}()};
    sf::Vector2f m_direction{0.f, 1.f};

    bool m_initialized = false;
};
