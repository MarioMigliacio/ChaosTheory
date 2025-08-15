// ============================================================================
//  File        : BombProjectile.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-07
//  Description : A lobbed projectile that explodes in a large AoE on impact.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "BombProjectile.h"
#include "AssetManager.h"
#include "Assets.h"
#include "CollisionManager.h"
#include "Macros.h"
#include "ResolutionScaleManager.h"

/// @brief Constructs a bomb projectile instance.
/// @param allegiance Which allegiance this projectile spawns with.
/// @param pos Initial world position.
/// @param velocity Initial velocity vector.
/// @param config Configuration for fuse, frames, and explosion properties.
BombProjectile::BombProjectile(Allegiance allegiance, const sf::Vector2f &pos, const sf::Vector2f &velocity,
                               const BombProjectileConfig &config)
    : m_cfg(config), m_anim(m_sprite)
{
    m_allegiance = allegiance;
    m_alive = true;

    SetPosition(pos);
    m_velocity = velocity;

    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::ProjectileAssets::BombProjectileSpriteKey);

    if (tex)
    {
        m_sprite.setTexture(*tex);
        m_sprite.setTextureRect({0, 0, m_cfg.projFrameWidth, m_cfg.projFrameHeight});
        m_sprite.setOrigin(m_cfg.projFrameWidth / 2.f, m_cfg.projFrameHeight / 2.f);
        m_sprite.setScale(ResolutionScaleManager::Instance().ScaleX(1.f),
                          ResolutionScaleManager::Instance().ScaleY(1.f));
    }

    else
    {
        CT_LOG_ERROR("BombProjectile: Projectile Texture not found.");
    }
}

/// @brief Updates the projectile's state.
/// @param dt Elapsed time in seconds since the last update.
void BombProjectile::Update(float dt)
{
    if (!m_exploding)
    {
        SetPosition(GetPosition() + m_velocity * dt);

        m_lifetime += dt;

        if (m_lifetime >= m_cfg.fuseTime || CollisionManager::Instance().HandleBombCollisionWithEnemyOrWorld(*this))
        {
            OnImpact();
            return;
        }
    }

    else
    {
        m_anim.Update(dt);

        if (m_anim.IsFinished())
        {
            Kill();
        }
    }
}

/// @brief Called when the projectile impacts an enemy or world geometry.
/// @note Switches to explosion mode, plays animation, and applies AoE damage.
void BombProjectile::OnImpact()
{
    if (m_exploding)
    {
        return;
    }

    m_exploding = true;
    m_velocity = {0.f, 0.f};

    auto blastSheet = AssetManager::Instance().GetTexture(SpriteAssets::ProjectileAssets::BombBlastSpriteKey);

    if (blastSheet)
    {
        m_anim.SetSheet(*blastSheet, m_cfg.explodeFrameWidth, m_cfg.explodeFrameHeight, m_cfg.framesPerRow,
                        m_cfg.totalFrames);
        m_anim.SetFrameDuration(m_cfg.frameDelay);
        m_anim.SetLooping(false);
        m_anim.SetFrameRange(0, m_cfg.totalFrames - 1);

        m_sprite.setOrigin(m_cfg.explodeFrameWidth / 2.f, m_cfg.explodeFrameHeight / 2.f);
    }

    else
    {
        CT_LOG_ERROR("BombProjectile: Blast Texture not found.");
    }

    m_anim.Reset();
    m_anim.Play();

    CollisionManager::Instance().ApplyAreaDamage(GetPosition(), m_cfg.blastRadius, m_cfg.damage, m_allegiance);

    // TODO: play sound, shake camera
    CT_LOG_DEBUG("BombProjectile: Explosion triggered at position ({}, {}).", GetPosition().x, GetPosition().y);
}
