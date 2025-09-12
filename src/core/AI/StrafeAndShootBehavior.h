// ============================================================================
//  File        : StrafeAndShootBehavior.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-18
//  Description : Behavior for moving side by side, and shooting.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "IBehavior.h"
#include <SFML/Graphics.hpp>
#include <random>

// ============================================================================
//  Class       : StrafeAndShootBehavior
//  Purpose     : Implements IBehavior interface for a Combat ready,
//                Strafe and Shoot behavior:
//                Moves in a random x direction down stream, while shooting.
//
//  Responsibilities:
//      - Update logic for movement.
//      - Update logic for gun.
//
// ============================================================================
class StrafeAndShootBehavior : public IBehavior
{
  public:
    StrafeAndShootBehavior();
    ~StrafeAndShootBehavior() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    StrafeAndShootBehavior(const StrafeAndShootBehavior &) = delete;
    StrafeAndShootBehavior &operator=(const StrafeAndShootBehavior &) = delete;

    StrafeAndShootBehavior(StrafeAndShootBehavior &&) = delete;
    StrafeAndShootBehavior &operator=(StrafeAndShootBehavior &&) = delete;

  protected:
    // IBehavior hooks
    void UpdateMovementLogic(BaseShip &ship, float dt) override;
    void UpdateGunLogic(BaseShip &ship, float dt) override;

  private:
    std::mt19937 m_rng{std::random_device{}()};

    float m_directionChangeCooldown = 0.f;
    float m_currentDirX = 0.f;
    float m_initialHoldTime = 0.f;
};
