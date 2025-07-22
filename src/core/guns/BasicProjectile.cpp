// ============================================================================
//  File        : BasicProjectile.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-20
//  Description : A default straight-traveling bullet projectile.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "BasicProjectile.h"
#include "AssetManager.h"
#include "Assets.h"
#include "Macros.h"
#include "WindowManager.h"

BasicProjectile::BasicProjectile(const sf::Vector2f &startPos, const sf::Vector2f &velocity, int damage,
                                 Allegiance allegiance, sf::Color tint)
{
    auto *tex = AssetManager::Instance().GetTexture(SpriteAssets::ProjectileAssets::BulletWhiteSpriteKey);

    if (tex)
    {
        m_sprite.setTexture(*tex);
        m_sprite.setColor(tint);
        m_sprite.setOrigin(tex->getSize().x / 2.f, tex->getSize().y / 2.f);
        m_sprite.setPosition(startPos);
    }

    else
    {
        CT_LOG_ERROR("BasicProjectile texture not found.");
    }

    m_velocity = velocity;
    m_damage = damage;
    m_allegiance = allegiance;
}

void BasicProjectile::Update(float dt)
{
    if (!m_alive)
    {
        return;
    }

    m_sprite.move(m_velocity * dt);

    const auto &bounds = m_sprite.getGlobalBounds();
    const auto &winSize = WindowManager::Instance().GetWindow().getSize();

    if (bounds.top + bounds.height < 0 || bounds.top > winSize.y || bounds.left + bounds.width < 0 ||
        bounds.left > winSize.x)
    {
        m_alive = false;
    }
}
