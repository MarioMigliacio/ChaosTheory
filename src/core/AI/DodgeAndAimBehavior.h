// ============================================================================
//  File        : DodgeAndAimBehavior.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-05
//  Description : Short hop dodge to a nearby X/Y, snapshot-aim at players
//                current location, fire a single projectile, then drift & cool.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "IBehavior.h"
#include <SFML/Graphics.hpp>

// ============================================================================
//  Class       : DodgeAndAimBehavior
//  Purpose     : Implements IBehavior interface for a DodgeAndAim behavior:
//                Dodges to a nearby X/Y, then fires a single aimed shot at
//                the player's current position, then cools down while
//                drifting downward.
//
//  Responsibilities:
//      - Update logic for movement.
//      - Update logic for gun.
//
// ============================================================================
class DodgeAndAimBehavior : public IBehavior
{
  public:
    DodgeAndAimBehavior() = default;
    ~DodgeAndAimBehavior() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    DodgeAndAimBehavior(const DodgeAndAimBehavior &) = delete;
    DodgeAndAimBehavior &operator=(const DodgeAndAimBehavior &) = delete;

    DodgeAndAimBehavior(DodgeAndAimBehavior &&) = delete;
    DodgeAndAimBehavior &operator=(DodgeAndAimBehavior &&) = delete;

  protected:
    // IBehavior hooks
    void UpdateMovementLogic(BaseShip &ship, float dt) override;
    void UpdateGunLogic(BaseShip &ship, float dt) override;

  private:
    enum class Phase
    {
        ChooseTarget,
        MoveToTarget,
        AimAndFire,
        Cooldown
    };

    void PickNearbyTarget(const BaseShip &ship);
    void MoveTowardTarget(BaseShip &ship, float dt);
    void FireAimedShot(BaseShip &ship);

  private:
    Phase m_phase = Phase::ChooseTarget;
    sf::Vector2f m_target{};
    sf::Vector2f m_snapshotPlayerPos{};

    float m_phaseTimer = 0.f;

    bool m_hasTarget = false;
    bool m_fired = false;
};
