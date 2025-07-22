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

class BasicProjectile : public BaseProjectile
{
  public:
    BasicProjectile(const sf::Vector2f &startPos, const sf::Vector2f &velocity, int damage, Allegiance allegiance,
                    sf::Color tint);
    ~BasicProjectile() override = default;

    void Update(float dt) override;
};
