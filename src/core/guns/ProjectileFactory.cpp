// ============================================================================
//  File        : ProjectileFactory.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-20
//  Description : Manufacturer of projectile entities.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "ProjectileFactory.h"
#include "AssetManager.h"
#include "Assets.h"
#include "BaseGun.h"
#include "BasicProjectile.h"
#include "ResolutionScaleManager.h"
#include "SettingsManager.h"
#include <cmath>

/// @brief Get the current Instance for this ProjectileFactory singleton.
/// @return reference to existing ProjectileFactory interface.
ProjectileFactory &ProjectileFactory::Instance()
{
    static ProjectileFactory instance;

    return instance;
}

/// @brief Creates a basic projectile that will travel based on Allegiance.
/// @param pos Position to originate from.
/// @param dir Direction projectile will travel in.
/// @param stats ProjectileStats struct with this projectiles configuration stats.
/// @param allegiance Allegiance to instantiate with.
/// @return A safe pointer to a BasicProjectile object, inheriting from BaseProjectile base class.
std::shared_ptr<BaseProjectile> ProjectileFactory::CreateBasicProjectile(const sf::Vector2f &pos,
                                                                         const sf::Vector2f &dir,
                                                                         const ProjectileStats &stats,
                                                                         Allegiance allegiance)
{
    sf::Vector2f normalizedDir = dir;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length != 0.f)
    {
        normalizedDir /= length;
    }

    else
    {
        normalizedDir = {0.f, -1.f};
    }

    auto projectile =
        std::make_shared<BasicProjectile>(pos, normalizedDir * stats.speed, stats.damage, allegiance, stats.tint);

    auto *tex = AssetManager::Instance().GetTexture(SpriteAssets::ProjectileAssets::BasicBulletSpriteKey);

    if (tex)
    {
        float scaleX = ResolutionScaleManager::Instance().ScaleX(tex->getSize().x) / tex->getSize().x;
        float scaleY = ResolutionScaleManager::Instance().ScaleY(tex->getSize().y) / tex->getSize().y;
        projectile->SetScale(scaleX, scaleY);
    }

    return projectile;
}
