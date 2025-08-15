// ============================================================================
//  File        : ExpandingProjectile.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-09
//  Description : Projectile that grows in size the further it travels.
//                Damage and other stats come from ProjectileStats at spawn.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseProjectile.h"
#include "ProjectileStats.h"
#include <SFML/Graphics.hpp>

// ============================================================================
//  Class       : ExpandingProjectile
//  Purpose     : A projectile that expands in size and damage as it travels.
//
//  Responsibilities:
//      - Updates scale and size during update.
//      - Marks itself as not alive when traveling off screen.
//
// ============================================================================
class ExpandingProjectile final : public BaseProjectile
{
  public:
    ExpandingProjectile(const sf::Vector2f &startPos, const sf::Vector2f &velocity, float damage, Allegiance allegiance,
                        sf::Color tint);
    ~ExpandingProjectile() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    ExpandingProjectile(const ExpandingProjectile &) = delete;
    ExpandingProjectile &operator=(const ExpandingProjectile &) = delete;

    ExpandingProjectile(ExpandingProjectile &&) = delete;
    ExpandingProjectile &operator=(ExpandingProjectile &&) = delete;

  public:
    void Update(float dt) override;

  private:
    sf::Vector2f m_startPos{};

    float m_baseScaleX = 1.f;
    float m_baseScaleY = 1.f;
    float m_baseDamage = 0.f;
};
