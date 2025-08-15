// ============================================================================
//  File        : BasicProjectile.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-20
//  Description : A default straight-traveling bullet projectile.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseProjectile.h"

// ============================================================================
//  Class       : BasicProjectile
//  Purpose     : A basic in game projectile with configurable stats.
//
//  Responsibilities:
//      - Provide Update for BasicProjectile.
//      - Marks itself as not alive when traveling off screen.
//
// ============================================================================
class BasicProjectile : public BaseProjectile
{
  public:
    BasicProjectile(const sf::Vector2f &startPos, const sf::Vector2f &velocity, float damage, Allegiance allegiance,
                    sf::Color tint);
    ~BasicProjectile() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    BasicProjectile(const BasicProjectile &) = delete;
    BasicProjectile &operator=(const BasicProjectile &) = delete;

    BasicProjectile(BasicProjectile &&) = delete;
    BasicProjectile &operator=(BasicProjectile &&) = delete;

  public:
    void Update(float dt) override;
};
