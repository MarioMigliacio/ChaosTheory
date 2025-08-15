// ============================================================================
//  File        : HomingRocketProjectile.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-11
//  Description : Small rocket that briefly homes forward on the nearest enemy.
//                If it "misses" (target moves behind/out of cone), it disengages
//                and sails straight off-screen. Explodes with a small AoE.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseProjectile.h"
#include "SpriteSheetAnimator.h"

/// @brief Provides a clean data structure that makes Configuration for HomingRocketProjectile customizable.
/// @param turnRateDegPerSec Max turn rate in deg/per sec while homing. Large val are fast, small val feel heavy.
/// @param homingDuration Number of seconds after launch during which homing is allowed.
/// @param maxSeekAngleDeg Forward cone half-angle in degrees; targets outside this cone
/// are considered "behind" and will not be pursued (prevents U-turns).
/// @param fuseTime Seconds until self-detonation if nothing is hit.
/// @param blastRadius AoE radius applied at impact or fuse detonation.
/// @param frameDelay seconds per frame.
/// @param projFrameWidth Frame width of initial Homing Rocket Projectile before sprite sheet.
/// @param projFrameHeight Frame height of initial Homing Rocket Projectile before sprite sheet.
/// @param explodeFrameWidth one frame width (sheet is 256x256).
/// @param explodeFrameHeight one frame height (sheet is 256x256).
/// @param framesPerRow Frames per row in sprite sheet.
/// @param totalFrames Total frames in sprite sheet.
struct HomingRocketConfig
{
    float turnRateDegPerSec = 360.f;
    float homingDuration = 0.75f;
    float maxSeekAngleDeg = 75.f;
    float fuseTime = 1.5f;
    float blastRadius = 64.f;
    float frameDelay = 0.05f;
    int projFrameWidth = 16;
    int projFrameHeight = 16;
    int explodeFrameWidth = 64;
    int explodeFrameHeight = 64;
    int framesPerRow = 4;
    int totalFrames = 16;
};

// ============================================================================
//  Class       : HomingRocketProjectile
//  Purpose     : A forward-limited homing rocket with a small AoE explosion.
//                Seeks out nearest target in a cone radius from origin.
//
//  Responsibilities:
//      - Updates trajectory while still in fuseTime and homingDuration.
//      - Switches sprite and animates for explosion.
//      - Marks itself as not alive when traveling off screen.
//
// ============================================================================
class HomingRocketProjectile final : public BaseProjectile
{
  public:
    HomingRocketProjectile(const sf::Vector2f &startPos, const sf::Vector2f &velocity, float damage,
                           Allegiance allegiance, sf::Color tint, const HomingRocketConfig &cfg = {});
    ~HomingRocketProjectile() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    HomingRocketProjectile(const HomingRocketProjectile &) = delete;
    HomingRocketProjectile &operator=(const HomingRocketProjectile &) = delete;

    HomingRocketProjectile(const HomingRocketProjectile &&) = delete;
    HomingRocketProjectile &operator=(const HomingRocketProjectile &&) = delete;

  public:
    void Update(float dt) override;
    void OnImpact();

  public:
    bool SupportsImpactAnimation() const override
    {
        return true;
    }
    void TriggerImpact() override
    {
        OnImpact();
    }

  private:
    SpriteSheetAnimator m_anim;
    HomingRocketConfig m_cfg;

    float m_age = 0.f;
    float m_baseScaleX = 1.f;
    float m_baseScaleY = 1.f;
    float m_baseDamage = 0.f;

    bool m_homingActive = true;
    bool m_exploding = false;
};
