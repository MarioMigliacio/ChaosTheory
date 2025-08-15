// ============================================================================
//  File        : HomingRocketProjectile.cpp
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

#include "HomingRocketProjectile.h"
#include "AssetManager.h"
#include "Assets.h"
#include "CollisionManager.h"
#include "Macros.h"
#include "ResolutionScaleManager.h"
#include "WindowManager.h"

#include <algorithm>
#include <cmath>

// ============================================================================
// Shamelessly stolen from the internet maths:
//  Notes on steering math:
//  - We compute a forward unit vector F from current velocity V: F = V / |V|.
//  - For a target at position T, with rocket at P, the direction to target is
//      D = (T - P) / |T - P|.
//  - The cosine of the angle between F and D is dot(F, D). If dot(F, D) >= cos(θ),
//    the target lies within a forward cone of half-angle θ (θ = maxSeekAngleDeg).
//    This prevents the rocket from turning around (no U-turns).
//  - Steering is performed by blending the current velocity toward the desired
//    velocity (D * |V|) with a weight 'turn' derived from turnRateDegPerSec:
//
//      turn = radians(turnRateDegPerSec) * dt   in [0, ~]
//      V'   = normalize( (1 - turn) * V + turn * (D * |V|) ) * |V|
//
//    This "critically simple" blend produces stable, frame-rate-friendly steering
//    without trigonometric reorientation and respects the speed magnitude.
//
//  - Homing disengages after 'homingDuration' seconds or when dot(F, D) drops
//    below cos(θ), at which point the rocket travels straight.
// ============================================================================

/// @brief Constructs a HomingRocketProjectile.
/// @param startPos Position to spawn at.
/// @param velocity Speed or direction to travel on.
/// @param damage Base damage to spawn with.
/// @param allegiance Allegiance (player/enemy) to spawn with.
/// @param tint Color tint if applicable to apply.
/// @param cfg HomingRocketConfig structure to gather data from.
HomingRocketProjectile::HomingRocketProjectile(const sf::Vector2f &startPos, const sf::Vector2f &velocity, float damage,
                                               Allegiance allegiance, sf::Color tint, const HomingRocketConfig &cfg)
    : m_cfg(cfg), m_anim(m_sprite)
{
    m_allegiance = allegiance;
    m_alive = true;

    SetPosition(startPos);
    SetVelocity(velocity);
    SetDamage(static_cast<int>(std::round(damage)));
    m_baseDamage = damage;

    // HomingRocket has a sprite that changes based on time and events, scale it here instead of ProjectileManager.
    m_baseScaleX = ResolutionScaleManager::Instance().ScaleX(1.f);
    m_baseScaleY = ResolutionScaleManager::Instance().ScaleY(1.f);

    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::ProjectileAssets::RocketProjectileSpriteKey);

    if (tex)
    {
        m_sprite.setTexture(*tex);
        m_sprite.setTextureRect({0, 0, m_cfg.projFrameWidth, m_cfg.projFrameHeight});
        m_sprite.setOrigin(m_cfg.projFrameWidth / 2.f, m_cfg.projFrameHeight / 2.f);
        m_sprite.setColor(tint);
        m_sprite.setScale(m_baseScaleX, m_baseScaleY);

        const sf::Vector2f v = GetVelocity();
        float ang = std::atan2(v.y, v.x) * 180.f / PI + 90.f;
        m_sprite.setRotation(ang);
    }

    else
    {
        CT_LOG_ERROR("HomingRocketProjectile: flight texture not found.");
    }
}

/// @brief Performs routine update during a frame.
/// @param dt delta time since last update.
void HomingRocketProjectile::Update(float dt)
{
    if (!m_alive)
    {
        return;
    }

    m_age += dt;

    if (m_exploding)
    {
        m_anim.Update(dt);

        if (m_anim.IsFinished())
        {
            Kill();
        }

        return;
    }

    // Homing only while within time window AND target is inside forward cone.
    if (m_homingActive && m_age <= m_cfg.homingDuration)
    {
        auto tgt = CollisionManager::Instance().GetNearestTarget(GetPosition(), m_allegiance);

        if (tgt)
        {
            const sf::Vector2f fwd = CT_MATH::Norm(GetVelocity());
            auto b = tgt->GetBounds();
            const sf::Vector2f tCenter{b.left + b.width * 0.5f, b.top + b.height * 0.5f};
            const sf::Vector2f toTgt = CT_MATH::Norm(tCenter - GetPosition());

            const float cosMax = std::cos(CT_MATH::DegToRad(m_cfg.maxSeekAngleDeg));
            const float cosAng = CT_MATH::Dot(fwd, toTgt);

            // still in front cone
            if (cosAng >= cosMax)
            {
                const float turn = CT_MATH::DegToRad(m_cfg.turnRateDegPerSec) * dt;
                const float speed = CT_MATH::Length(GetVelocity());
                const sf::Vector2f desired = toTgt * speed;

                // simple, stable steer via blend
                const sf::Vector2f blended = CT_MATH::Norm((1.f - turn) * fwd * speed + turn * desired);
                SetVelocity(blended * speed);

                float ang = std::atan2(blended.y, blended.x) * 180.f / PI + 90.f;
                m_sprite.setRotation(ang);
            }

            // target left the cone: permanently disengage
            else
            {
                m_homingActive = false;
            }
        }

        // no target => fly straight
        else
        {
            m_homingActive = false;
        }
    }

    // window expired
    else
    {
        m_homingActive = false;
    }

    // Move
    SetPosition(GetPosition() + GetVelocity() * dt);

    // Impact / fuse
    if (m_age >= m_cfg.fuseTime || CollisionManager::Instance().HandleBombCollisionWithEnemyOrWorld(*this))
    {
        OnImpact();

        return;
    }

    // Offscreen cull
    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    const auto bounds = m_sprite.getGlobalBounds();

    if (bounds.top > winSize.y || bounds.top + bounds.height < 0 || bounds.left + bounds.width < 0 ||
        bounds.left > winSize.x)
    {
        Kill();
    }
}

/// @brief Called when the projectile impacts an enemy or world geometry.
/// @note Switches to explosion mode, plays animation, and applies AoE damage.
void HomingRocketProjectile::OnImpact()
{
    if (!m_alive || m_exploding)
    {
        return;
    }

    m_exploding = true;
    m_homingActive = false;
    SetVelocity({0.f, 0.f});

    auto sheet = AssetManager::Instance().GetTexture(SpriteAssets::ProjectileAssets::RocketBlastSpriteKey);

    if (sheet)
    {
        m_anim.SetSheet(*sheet, m_cfg.explodeFrameWidth, m_cfg.explodeFrameHeight, m_cfg.framesPerRow,
                        m_cfg.totalFrames);
        m_anim.SetFrameDuration(m_cfg.frameDelay);
        m_anim.SetLooping(false);
        m_anim.SetFrameRange(0, m_cfg.totalFrames - 1);

        m_sprite.setOrigin(m_cfg.explodeFrameWidth / 2.f, m_cfg.explodeFrameHeight / 2.f);
        m_anim.Reset();
        m_anim.Play();
    }

    // fail-safe so rocket doesn’t get stuck
    else
    {
        CT_LOG_ERROR("HomingRocketProjectile: explosion sheet not found.");
        Kill();

        return;
    }

    CollisionManager::Instance().ApplyAreaDamage(GetPosition(), m_cfg.blastRadius, m_baseDamage, m_allegiance);
}
