// ============================================================================
//  File        : BasicGun.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-19
//  Description : Base implementation for firearm behavior across all ships.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Allegiance.h"
#include "IGun.h"
#include <SFML/Graphics.hpp>

// ============================================================================
//  Class       : BasicGun
//  Purpose     : An abstraction layer above IGun interface,
//                for shared common gun behaviors' base implementation.
//
//  Responsibilities:
//      - Provide Draw, Update, TryFire and TryFireTowards interface.
//      - Manages cooldown and projectile spawning.
//
// ============================================================================
class BasicGun : public IGun
{
  public:
    BasicGun(float cooldownSeconds, Allegiance allegiance);
    ~BasicGun() override = default;

    void Update(float dt) override;

    void SetOwnerPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetBarrelOffset() const;
    void SetAllegiance(Allegiance allegiance);

    std::shared_ptr<BaseProjectile> TryFire() override;
    std::shared_ptr<BaseProjectile> TryFireTowards(const sf::Vector2f &targetPos) override;

  private:
    Allegiance m_allegiance = Allegiance::Neutral;

    sf::Vector2f m_ownerPosition;
    sf::Vector2f m_barrelOffset;

    float m_cooldown = 0.f;
    float m_fireRate = 0.3f;
};
