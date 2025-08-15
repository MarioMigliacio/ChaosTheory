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
#include "ExpandingProjectile.h"
#include "Macros.h"
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
/// @return A safe pointer to a BaseProjectile object.
std::shared_ptr<BaseProjectile> ProjectileFactory::CreateBasicProjectile(const sf::Vector2f &pos,
                                                                         const sf::Vector2f &dir,
                                                                         const ProjectileStats &stats,
                                                                         Allegiance allegiance)
{
    sf::Vector2f normalizedDir = CT_MATH::Norm(dir);

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

/// @brief Creates an expanding projectile that will travel based on Allegiance.
/// @param pos Position to originate from.
/// @param dir Direction projectile will travel in.
/// @param stats ProjectileStats struct with this projectiles configuration stats.
/// @param allegiance Allegiance to instantiate with.
/// @return A safe pointer to a BaseProjectile object.
std::shared_ptr<BaseProjectile> ProjectileFactory::CreateExpandingProjectile(const sf::Vector2f &pos,
                                                                             const sf::Vector2f &dir,
                                                                             const ProjectileStats &stats,
                                                                             Allegiance allegiance)
{
    sf::Vector2f normalizedDir = CT_MATH::Norm(dir);

    auto projectile =
        std::make_shared<ExpandingProjectile>(pos, normalizedDir * stats.speed, stats.damage, allegiance, stats.tint);

    auto *tex = AssetManager::Instance().GetTexture(SpriteAssets::ProjectileAssets::BasicBulletSpriteKey);

    if (tex)
    {
        float scaleX = ResolutionScaleManager::Instance().ScaleX(tex->getSize().x) / tex->getSize().x;
        float scaleY = ResolutionScaleManager::Instance().ScaleY(tex->getSize().y) / tex->getSize().y;
        projectile->SetScale(scaleX, scaleY);
    }

    return projectile;
}

/// @brief Creates a Bomb Projectile that travels based on velocity and allegiance.
/// @param pos Initial spawn location.
/// @param velocity Vector direction to travel after spawning.
/// @param allegiance Allegiance to instantiate with.
/// @param cfg BombProjectileConfig to initiate custom.
/// @return A safe pointer to a BaseProjectile object.
std::shared_ptr<BaseProjectile> ProjectileFactory::CreateBombProjectile(const sf::Vector2f &pos,
                                                                        const sf::Vector2f &velocity,
                                                                        Allegiance allegiance,
                                                                        const BombProjectileConfig &cfg)
{
    // Bomb Projectile handles its own resolution scaling internally.
    return std::make_shared<BombProjectile>(allegiance, pos, velocity, cfg);
}

/// @brief Creates a HomingRocket Projectile that seeks the nearest target.
/// @param pos Initial spawn location.
/// @param dir Vector direction to travel after spawning.
/// @param stats ProjectileStats to initialize with.
/// @param allegiance Allegiance to instantiate with.
/// @param cfg HomingRocketConfig to intiate custom.
/// @return A safe pointer to a BaseProjectile object.
std::shared_ptr<BaseProjectile> ProjectileFactory::CreateHomingRocket(const sf::Vector2f &pos, const sf::Vector2f &dir,
                                                                      const ProjectileStats &stats,
                                                                      Allegiance allegiance,
                                                                      const HomingRocketConfig &cfg)
{
    sf::Vector2f normalizedDir = CT_MATH::Norm(dir);

    // Homing Rocket Projectile handles its own resolution scaling internally.
    auto proj = std::make_shared<HomingRocketProjectile>(pos, normalizedDir * stats.speed, stats.damage, allegiance,
                                                         stats.tint, cfg);

    return proj;
}

/// @brief Creates an individual LazerBeam Projectile segment that travels based on allegiance.
/// @param pos Position to spawn at.
/// @param dir Direction to travel at upon spawn.
/// @param stats ProjectileStats to configure with.
/// @param allegiance Allegiance to instantiate with.
/// @param perSegmentDamageFactor Factor to scale damage based on projectile segment.
/// @return A safe pointer to a BaseProjectile object.
std::shared_ptr<BaseProjectile> ProjectileFactory::CreateLazerBeamProjectile(const sf::Vector2f &pos,
                                                                             const sf::Vector2f &dir,
                                                                             const ProjectileStats &stats,
                                                                             Allegiance allegiance,
                                                                             float perSegmentDamageFactor)
{
    sf::Vector2f normalizedDir = CT_MATH::Norm(dir);

    auto seg = std::make_shared<LazerBeamProjectile>(pos, normalizedDir * stats.speed, stats.damage, allegiance,
                                                     stats.tint, perSegmentDamageFactor);

    auto *tex = AssetManager::Instance().GetTexture(SpriteAssets::ProjectileAssets::LazerBeamProjectileSpriteKey);

    if (tex)
    {
        float scaleX = ResolutionScaleManager::Instance().ScaleX(tex->getSize().x) / tex->getSize().x;
        float scaleY = ResolutionScaleManager::Instance().ScaleY(tex->getSize().y) / tex->getSize().y;
        seg->SetScale(scaleX, scaleY);
    }

    return seg;
}
