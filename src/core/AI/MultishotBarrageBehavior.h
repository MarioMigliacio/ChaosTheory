// ============================================================================
//  File        : MultishotBarrageBehavior.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-07
//  Description : Drifts with a lateral wobble, aims at player, fires a multishot
//                volley, rotates slightly inbetween X volleys, then cools down.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "IBehavior.h"
#include <SFML/Graphics.hpp>

// ============================================================================
//  Class       : MultishotBarrageBehavior
//  Purpose     : Implements IBehavior interface for a Combat ready,
//                Drift and Multi Shoot barrage behavior:
//                Moves in a random x direction down stream,
//                then locks onto player and fires a multi shot barrage
//                multiple times. Cools down by drifting again.
//
//  Responsibilities:
//      - Update logic for movement.
//      - Update logic for gun.
//
// ============================================================================
class MultishotBarrageBehavior : public IBehavior
{
  public:
    MultishotBarrageBehavior() = default;
    ~MultishotBarrageBehavior() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    MultishotBarrageBehavior(const MultishotBarrageBehavior &) = delete;
    MultishotBarrageBehavior &operator=(const MultishotBarrageBehavior &) = delete;

    MultishotBarrageBehavior(MultishotBarrageBehavior &&) = delete;
    MultishotBarrageBehavior &operator=(MultishotBarrageBehavior &&) = delete;

  protected:
    // IBehavior hooks
    void UpdateMovementLogic(BaseShip &ship, float dt) override;
    void UpdateGunLogic(BaseShip &ship, float dt) override;

  private:
    enum class Phase
    {
        Drift,
        Aim,
        Barrage,
        Cooldown
    };

  private:
    void BeginDrift();
    void DoDrift(BaseShip &ship, float dt);
    void BeginAim(BaseShip &ship);
    void DoAim(BaseShip &ship, float dt);
    void BeginBarrage();
    void DoBarrage(BaseShip &ship, float dt);
    void FireVolley(BaseShip &ship, float baseAngleRad);
    void SetSpriteRotation(BaseShip &ship, float angleRad);

    static float WrapAngleRad(float a);
    static float ApproachAngle(float current, float target, float maxStepRad);

  private:
    Phase m_phase{Phase::Drift};

    float m_driftDirX = 0.f;
    float m_driftSpeedX = 0.f;
    float m_phaseTimer = 0.f;
    float m_aimAngleRad = 0.f;
    float m_targetAngleRad = 0.f;
    float m_betweenVolleyTimer = 0.f;
    float m_turnDir = 1.f;

    int m_volleysFired = 0;
};
