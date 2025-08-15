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

#include "BaseGun.h"
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

    void ApplyIconEffect(const std::shared_ptr<UIIcon> &icon);
    void ProcessInput(const float dt);

    float GetGas() const;
    float GetMaxGas() const;
    void ReplenishGas(const float amount);
    void BoostMaxGas(const float amount);

    void ReplenishHealth(const float amount);
    void BoostMaxHealth(const float amount);

    void TakeDamage(const float amount) override;
    void Kill() override;

    void GainLifeCount();
    void LoseLife();
    int GetLifeCount() const;
    void Respawn();
    bool GetInvincibleStatus() const;

    bool IsGameOver() const;

    int GetBombCount() const;
    void GainBombCount();
    void TryFireBomb();

  private:
    void HandleGunUpdate(const float dt);
    void HandleRespawnUpdate(const float dt);
    void InitializeGunStats() override;
    void InitializeGenericStats();

  private:
    int m_lives = 0;
    int m_bombs = 1;

    float m_bombCooldown = 0.f;
    float m_gas = 0.f;
    float m_maxGas = 0.f;
    float m_accelerationMultiplier = 0.f;
    float m_gasDrainRate = 0.f;
    float m_baseSpeed = 0.f;
    float m_invincibilityTimer = 0.f;
    float m_blinkTimer = 0.f;

    bool m_gameOver = false;
    bool m_invincible = false;
    bool m_visible = true;
};
