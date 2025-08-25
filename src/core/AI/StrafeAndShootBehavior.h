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
//      - Update logic for gun (not relevant in this behavior).
//
// ============================================================================
class StrafeAndShootBehavior : public IBehavior
{
  public:
    ~StrafeAndShootBehavior() override = default;

  protected:
    // IBehavior hooks
    void UpdateMovementLogic(BaseShip &ship, float dt) override;
    void UpdateGunLogic(BaseShip &ship, float dt) override;

  protected:
    // Per-behavior constants (tweak here if needed; or make setters later)
    static constexpr float sDirectionChangeInterval = 1.0f; // seconds
    static constexpr float sInternalXDampener = 0.5f;       // slows horizontal strafe
    static constexpr float sInternalYDampener = 4.0f;       // slows descent
    static constexpr float sInitialHoldTime = 1.0f;         // delay before first shot

    // Behavior-owned state (ships inherit this, so each instance gets its own copy)
    float m_directionChangeCooldown = sDirectionChangeInterval;
    float m_currentDirX = 1.f;
    float m_initialHoldTime = sInitialHoldTime;

    std::mt19937 m_rng{std::random_device{}()};
};
