// ============================================================================
//  File        : LazerBeamProjectile.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-13
//  Description : A tiny, very fast projectile used to build a continuous
//                traveling lazer stream. No AoE, no impact animation.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "LazerBeamProjectile.h"
#include "AssetManager.h"
#include "Assets.h"
#include "Macros.h"
#include "ResolutionScaleManager.h"
#include "WindowManager.h"

/// @brief Constructs a Singular segment for a LazerBeamProjectile.
/// @param startPos Position to spawn at.
/// @param velocity Speed or direction to travel at upon spawn.
/// @param baseDamageFromGun Base damage to spawn with.
/// @param allegiance Allegiance (player/enemy) to spawn with.
/// @param tint Color tint to apply if relevant.
/// @param perSegmentDamageFactor Dampening effect for lazer beam, since they collide at much higher frequency.
LazerBeamProjectile::LazerBeamProjectile(const sf::Vector2f &startPos, const sf::Vector2f &velocity,
                                         float baseDamageFromGun, Allegiance allegiance, sf::Color tint,
                                         float perSegmentDamageFactor)
{
    m_allegiance = allegiance;
    m_alive = true;

    SetPosition(startPos);
    SetVelocity(velocity);

    // Damage = gunDamage * factor (fractional health supported in your code now)
    const float segDmg = std::max(0.f, baseDamageFromGun) * perSegmentDamageFactor;
    SetDamage(segDmg);

    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::ProjectileAssets::LazerBeamProjectileSpriteKey);

    if (tex)
    {
        m_sprite.setTexture(*tex);
        m_sprite.setColor(tint);
        const auto size = tex->getSize();

        // Player: center horizontally, top anchored
        if (m_allegiance == Allegiance::Player || m_allegiance == Allegiance::Friendly)
        {
            m_sprite.setOrigin(size.x * 0.5f, static_cast<float>(size.y));
        }

        // Enemy: center horizontally, top anchored
        else if (m_allegiance == Allegiance::Enemy)
        {
            m_sprite.setOrigin(size.x * 0.5f, 0.f);
        }
    }

    else
    {
        CT_LOG_ERROR("LazerBeamProjectile: texture not found.");
    }
}

/// @brief Performs routine update during a frame.
/// @param dt delta time since last update.
void LazerBeamProjectile::Update(float dt)
{
    if (!m_alive)
    {
        return;
    }

    SetPosition(GetPosition() + GetVelocity() * dt);

    // Offscreen cull
    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    const auto bounds = m_sprite.getGlobalBounds();

    if (bounds.top > winSize.y || bounds.top + bounds.height < 0 || bounds.left + bounds.width < 0 ||
        bounds.left > winSize.x)
    {
        Kill();
    }
}
