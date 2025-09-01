// ============================================================================
//  File        : CrusaderBehavior.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-24
//  Description : Lines up to player's X, telegraphs (shake + red glow),
//                then fires a lethal laser beam stream forward.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "IBehavior.h"
#include <SFML/Graphics.hpp>

// ============================================================================
//  Class       : CrusaderBehavior
//  Purpose     : Implements IBehavior interface for a Crusader behavior:
//                Moves in x direction towards player, locks on, and changes
//                mode to fire a deadly laser beam stream. Then reverts to
//                cooldown and attempt to lock on again if able to.
//
//  Responsibilities:
//      - Update logic for movement.
//      - Update logic for gun.
//
// ============================================================================
class CrusaderBehavior : public IBehavior
{
  public:
    ~CrusaderBehavior() override = default;

  protected:
    // IBehavior hooks
    void UpdateMovementLogic(BaseShip &ship, float dt) override;
    void UpdateGunLogic(BaseShip &ship, float dt) override;

  private:
    enum class Phase
    {
        Align,
        Telegraph,
        Fire,
        Cooldown
    };

    void AlignToPlayerX(BaseShip &ship, float dt);
    void DoTelegraph(BaseShip &ship, float dt);
    void BeginFire(BaseShip &ship);
    void DoFire(BaseShip &ship, float dt);

  private:
    Phase m_phase = Phase::Align;

    float m_phaseTimer = 0.f;
    float m_shakeAnchorX = 0.f;

    bool m_firingPrimed = false;
    bool m_glowApplied = false;
    bool m_anchorCaptured = false;
};
