// ============================================================================
//  File        : IGun.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-19
//  Description : Interface for firearm behavior across all ships.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================
#pragma once

#include "BaseProjectile.h"
#include <SFML/Graphics.hpp>
#include <memory>

// ============================================================================
//  Class       : IGun
//  Purpose     : A pure abstraction for common gun behaviors.
//
//  Responsibilities:
//      - Provide Draw, Update, TryFire and TryFireTowards interface.
//
// ============================================================================
class IGun
{
  public:
    virtual ~IGun() = default;

    virtual void Update(float dt) = 0;
    virtual void SetOwnerPosition(const sf::Vector2f &position) = 0;

    virtual std::shared_ptr<BaseProjectile> TryFire() = 0;
    virtual std::shared_ptr<BaseProjectile> TryFireTowards(const sf::Vector2f &targetPos) = 0;
};
