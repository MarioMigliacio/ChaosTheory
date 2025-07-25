// ============================================================================
//  File        : PlayerShip.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-25
//  Description : Represents the ChaosTheory Playable space ship.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseShip.h"

// ============================================================================
//  Class       : PlayerShip
//  Purpose     : A player controlled ship with an upgradable gun, and added
//                acceleration maneuverability with bombing capability.
//
//  Responsibilities:
//      - Accept input to move in 8 direction.
//      - Accelerate when appropriate key is down.
//      - Fire munitions with attached gun when appropriate key is down.
//      - Launch bombs when appropriate key is down.
//      - Update internal health/gas etc. states.
//
// ============================================================================
class PlayerShip : public BaseShip
{
  public:
    PlayerShip();
    ~PlayerShip() override = default;

    void Update(float dt) override;
    void Move(const sf::Vector2f &offset) override;
    std::shared_ptr<BaseProjectile> TryFire();

    void ProcessInput(const float dt);

    float GetGas() const;
    void ReplenishGas(float amount);

    void ApplyDifficultyScaling() override;

  private:
    void HandleGunUpdate(float dt);

  private:
    float m_gas;
    float m_maxGas;
    float m_maxHealth;
    float m_accelerationMultiplier;
    float m_gasDrainRate;
    float m_baseSpeed;
};
