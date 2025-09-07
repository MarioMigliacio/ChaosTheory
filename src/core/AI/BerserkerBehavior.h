// ============================================================================
//  File        : BerserkerBehavior.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-19
//  Description : Kamikaze behavior. On first tick, locks to the player's
//                position at spawn and flies straight along that fixed vector.
//                No shooting.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "IBehavior.h"
#include <SFML/Graphics.hpp>

// ============================================================================
//  Class       : BerserkerBehavior
//  Purpose     : Implements IBehavior interface for a Berserker behavior:
//                Moves in a vector direction aggressively towards player,
//                without shooting.
//
//  Responsibilities:
//      - Update logic for movement.
//      - Update logic for gun (not relevant in this behavior).
//
// ============================================================================
class BerserkerBehavior : public IBehavior
{
  public:
    BerserkerBehavior() = default;
    ~BerserkerBehavior() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    BerserkerBehavior(const BerserkerBehavior &) = delete;
    BerserkerBehavior &operator=(const BerserkerBehavior &) = delete;

    BerserkerBehavior(BerserkerBehavior &&) = delete;
    BerserkerBehavior &operator=(BerserkerBehavior &&) = delete;

  protected:
    // IBehavior hooks
    void UpdateMovementLogic(BaseShip &ship, float dt) override;
    void UpdateGunLogic(BaseShip &ship, float dt) override;

  public:
    void SetSpawnTargetSnapshot(const sf::Vector2f &playerPos);
    bool HasSnapshot() const;
    void ClearSnapshot();

  private:
    void EnsureDirectionInitialized(BaseShip &ship);

  private:
    sf::Vector2f m_spawnTargetSnapshot{};
    bool m_hasSnapshot = false;

    sf::Vector2f m_direction{0.f, 1.f};
    bool m_initialized = false;
};
