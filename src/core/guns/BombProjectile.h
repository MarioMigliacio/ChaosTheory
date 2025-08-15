// ============================================================================
//  File        : BombProjectile.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-07
//  Description : A lobbed projectile that explodes in a large AoE on impact.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseProjectile.h"
#include "SpriteSheetAnimator.h"

/// @brief Provides a clean data structure that makes Configuration for Bomb Projectile customizable.
/// @param fuseTime Seconds until detonation regardless of hit.
/// @param blastRadius Area of Effect Radius.
/// @param damage High damage potential, but not boss-killing.
/// @param frameDelay Animaltion frame time.
/// @param projFrameWidth Frame width of initial bomb projectile before sprite sheet.
/// @param projFrameHeight Frame height of initial bomb projectile before sprite sheet.
/// @param explodeFrameWidth One frame width (sheet is 512x512).
/// @param explodeFrameHeight One frame height (sheet is 512x512).
/// @param framesPerRow Frames per row in sprite sheet.
/// @param totalFrames Total frames in sprite sheet.
struct BombProjectileConfig
{
    float fuseTime = 1.5f;
    float blastRadius = 128.f;
    float damage = 80.f;
    float frameDelay = 0.05f;
    int projFrameWidth = 64;
    int projFrameHeight = 64;
    int explodeFrameWidth = 128;
    int explodeFrameHeight = 128;
    int framesPerRow = 4;
    int totalFrames = 16;
};

// ============================================================================
//  Class       : BombProjectile
//  Purpose     : Singleton class that manages the generation of Projectile
//                elements.
//
//  Responsibilities:
//      - Scale texture sizes and generate safe pointer Projectile objects.
//      - Scale damage and velocity baseed on game difficulty.
//
// ============================================================================
class BombProjectile final : public BaseProjectile
{
  public:
    BombProjectile(Allegiance allegiance, const sf::Vector2f &pos, const sf::Vector2f &velocity,
                   const BombProjectileConfig &config);
    ~BombProjectile() override = default;

    // Disallow copy and move semantics to avoid shallow copies or misuse
    BombProjectile(const BombProjectile &) = delete;
    BombProjectile &operator=(const BombProjectile &) = delete;

    BombProjectile(BombProjectile &&) = delete;
    BombProjectile &operator=(BombProjectile &&) = delete;

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
    BombProjectileConfig m_cfg;
    SpriteSheetAnimator m_anim;

    float m_lifetime = 0.f;

    bool m_exploding = false;
};
