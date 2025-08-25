// ============================================================================
//  File        : LaserBeamProjectile.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-13
//  Description : A tiny, very fast projectile used to build a continuous
//                traveling laser stream. No AoE, no impact animation.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseProjectile.h"

/// @brief Provides a clean data structure that makes Configuration for LaserBeamProjectile fire rate customizable.
/// @note Example: fireRate=0.50s starting rate, intervalFactor=0.20 -> 0.1s interval = 100 ms fire beams.
///       Spawn cadence = fireRate * intervalFactor, clamped by minInterval
/// @param intervalFactor Multiplier to be used in conjunction with the Guns stats.fireRate, creates the 'cooldown'.
/// @param minInterval The absolute floor for how fast intervalFactor is allowed to get. Prevents absurd spawn rates.
struct LaserBeamStreamConfig
{
    float intervalFactor = 0.20f;
    float minInterval = 0.015f;
};

// ============================================================================
//  Class       : LaserBeamProjectile
//  Purpose     : A steady stream of laser beam projectiles that consistently
//                damage the impacted unit quickly.
//
//  Responsibilities:
//      - Updates the laserbeam projectile.
//      - Marks itself as not alive when traveling off screen.
//
// ============================================================================
class LaserBeamProjectile final : public BaseProjectile
{
  public:
    LaserBeamProjectile(const sf::Vector2f &startPos, const sf::Vector2f &velocity, float baseDamageFromGun,
                        Allegiance allegiance, sf::Color tint, float perSegmentDamageFactor = 0.5f);
    ~LaserBeamProjectile() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    LaserBeamProjectile(const LaserBeamProjectile &) = delete;
    LaserBeamProjectile &operator=(const LaserBeamProjectile &) = delete;

    LaserBeamProjectile(const LaserBeamProjectile &&) = delete;
    LaserBeamProjectile &operator=(const LaserBeamProjectile &&) = delete;

  public:
    void Update(float dt) override;
};
