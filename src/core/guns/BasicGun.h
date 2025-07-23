// ============================================================================
//  File        : BasicGun.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-19
//  Description : BasicGun implementation for firearm behavior.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Allegiance.h"
#include "BaseGun.h"
#include <SFML/Graphics.hpp>

// ============================================================================
//  Class       : BasicGun
//  Purpose     : An abstraction layer above BaseGun interface,
//                for shared common gun behaviors base implementation.
//
//  Responsibilities:
//      - Provide Update, TryFire and TryFireTowards implementations for BasicGun.
//      - Manages cooldown and projectile spawning.
//
// ============================================================================
class BasicGun : public BaseGun
{
  public:
    BasicGun(float cooldownSeconds, Allegiance allegiance);
    ~BasicGun() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    BasicGun(const BasicGun &) = delete;
    BasicGun &operator=(const BasicGun &) = delete;

    BasicGun(BasicGun &&) = delete;
    BasicGun &operator=(BasicGun &&) = delete;

  public:
    void Update(float dt) override;

    std::shared_ptr<BaseProjectile> TryFire() override;
    std::shared_ptr<BaseProjectile> TryFireTowards(const sf::Vector2f &targetPos) override;
};
