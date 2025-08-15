// ============================================================================
//  File        : ExpandingProjectile.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-09
//  Description : Projectile that grows in size the further it travels.
//                Damage and other stats come from ProjectileStats at spawn.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "ExpandingProjectile.h"
#include "AssetManager.h"
#include "Assets.h"
#include "Macros.h"
#include "ResolutionScaleManager.h"
#include "WindowManager.h"

/// @brief Constants that can be adjusted throughout the ExpandingProjectile.
namespace
{
/// @brief Scales the size of the Expanding Projectile in conjunction with delta time.
constexpr float GROWTH_PER_PIXEL = 0.015f;

/// @brief Caps the growth scale for the Expanding Projectile.
constexpr float MAX_SCALE = 5.f;
} // namespace

/// @brief Constructor for the ExpandingProjectile.
/// @param startPos Position to spawn at.
/// @param velocity Speed and direction this projectile travels at.
/// @param damage Base damage for this projectile.
/// @param allegiance Allegiance, (player/enemy) for projectile.
/// @param tint Any color tint that might be applied to the projectile.
ExpandingProjectile::ExpandingProjectile(const sf::Vector2f &startPos, const sf::Vector2f &velocity, float damage,
                                         Allegiance allegiance, sf::Color tint)
{
    m_allegiance = allegiance;
    m_alive = true;

    m_startPos = startPos;
    SetPosition(startPos);
    SetVelocity(velocity);
    SetDamage(static_cast<int>(std::round(damage)));
    m_baseDamage = damage;

    // Base (resolution) scale captured so growth multiplies on top.
    m_baseScaleX = ResolutionScaleManager::Instance().ScaleX(1.f);
    m_baseScaleY = ResolutionScaleManager::Instance().ScaleY(1.f);

    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::ProjectileAssets::BasicBulletSpriteKey);

    if (tex)
    {
        tex->setSmooth(true); // very simple hack for exploding the resolution up, not so blocky.
        m_sprite.setTexture(*tex);

        const auto size = tex->getSize();
        m_sprite.setTextureRect({0, 0, static_cast<int>(size.x), static_cast<int>(size.y)});
        m_sprite.setOrigin(size.x / 2.f, size.y / 2.f);
        m_sprite.setColor(tint);

        // Start at base resolution scale; growth applied in Update().
        m_sprite.setScale(m_baseScaleX, m_baseScaleY);
    }

    else
    {
        CT_LOG_ERROR("ExpandingProjectile: Texture key not found.");
    }
}

/// @brief Performs routine update during a frame.
/// @param dt delta time since last update.
void ExpandingProjectile::Update(float dt)
{
    if (!m_alive)
    {
        return;
    }

    SetPosition(GetPosition() + GetVelocity() * dt);

    // Compute growth factor based on distance
    const sf::Vector2f delta = GetPosition() - m_startPos;
    const float dist = CT_MATH::Length(delta);
    const float targetScale = std::clamp(1.f + dist * GROWTH_PER_PIXEL, 1.f, MAX_SCALE);

    // Scale damage bonus based on scale. Pow keeps the scaling resonable.
    const float dmgMultiplier = std::pow(targetScale, 0.5f); //
    const float scaledDamage = m_baseDamage * dmgMultiplier;
    SetDamage(static_cast<int>(std::round(scaledDamage)));
    m_sprite.setScale(m_baseScaleX * targetScale, m_baseScaleY * targetScale);

    // Offscreen cull
    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    const auto bounds = m_sprite.getGlobalBounds();

    if (bounds.top > winSize.y || bounds.top + bounds.height < 0 || bounds.left + bounds.width < 0 ||
        bounds.left > winSize.x)
    {
        Kill();
    }
}
