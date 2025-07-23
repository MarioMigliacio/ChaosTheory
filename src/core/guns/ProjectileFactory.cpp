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
/// @param type ProjectileCategory type to apply for this projectile.
/// @param allegiance Allegiance to instantiate with.
/// @return A safe pointer to a BasicProjectile object, inheriting from BaseProjectile base class.
std::shared_ptr<BaseProjectile> ProjectileFactory::CreateBasicProjectile(const sf::Vector2f &pos,
                                                                         ProjectileCategory type, Allegiance allegiance)
{
    auto stats = ApplyDifficultyScaling(GetStats(type), allegiance);
    sf::Vector2f dir = GetDefaultFireDirection(allegiance);

    auto projectile = std::make_shared<BasicProjectile>(pos, dir * stats.speed, stats.damage, allegiance, stats.tint);

    auto *tex = AssetManager::Instance().GetTexture(SpriteAssets::ProjectileAssets::BulletWhiteSpriteKey);

    if (tex)
    {
        float baseSizeX = static_cast<float>(tex->getSize().x);
        float baseSizeY = static_cast<float>(tex->getSize().y);
        float scaleX = ResolutionScaleManager::Instance().ScaleX(baseSizeX) / baseSizeX;
        float scaleY = ResolutionScaleManager::Instance().ScaleY(baseSizeY) / baseSizeY;
        projectile->SetScale(scaleX, scaleY);
    }

    return projectile;
}

/// @brief Creates a basic projectile that will be fired in a vector direction based on a target, typically AI driven.
/// @param pos Position to originate from.
/// @param dir Directional vector to target and update towards.
/// @param type ProjectileCategory type to apply for this projectile.
/// @param allegiance Allegiance to instantiate with.
/// @return A safe pointer to a BasicProjectile object, inheriting from BaseProjectile base class.
std::shared_ptr<BaseProjectile> ProjectileFactory::CreateBasicProjectile(const sf::Vector2f &pos,
                                                                         const sf::Vector2f &dir,
                                                                         ProjectileCategory type, Allegiance allegiance)
{
    auto stats = ApplyDifficultyScaling(GetStats(type), allegiance);

    // Normalize direction
    sf::Vector2f normalizedDir = dir;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length != 0.f)
    {
        normalizedDir /= length;
    }

    else
    {
        normalizedDir = {0.f, -1.f}; // Default upward fallback
    }

    auto projectile =
        std::make_shared<BasicProjectile>(pos, normalizedDir * stats.speed, stats.damage, allegiance, stats.tint);

    auto *tex = AssetManager::Instance().GetTexture(SpriteAssets::ProjectileAssets::BulletWhiteSpriteKey);

    if (tex)
    {
        float baseSizeX = static_cast<float>(tex->getSize().x);
        float baseSizeY = static_cast<float>(tex->getSize().y);
        float scaleX = ResolutionScaleManager::Instance().ScaleX(baseSizeX) / baseSizeX;
        float scaleY = ResolutionScaleManager::Instance().ScaleY(baseSizeY) / baseSizeY;
        projectile->SetScale(scaleX, scaleY);
    }

    return projectile;
}

/// @brief Returns configurable default stats for Projectiles based on category.
/// @param type ProjectileCategory to calculate based off of.
/// @return Structure of ProjectileStats based on type.
ProjectileStats ProjectileFactory::GetStats(ProjectileCategory type) const
{
    switch (type)
    {
        case ProjectileCategory::White:
        default:
            return {150.f, 10, sf::Color::White};
        case ProjectileCategory::Red:
            return {250.f, 8, sf::Color(255, 60, 60)};
        case ProjectileCategory::Blue:
            return {100.f, 15, sf::Color(100, 100, 255)};
        case ProjectileCategory::Green:
            return {200.f, 12, sf::Color(60, 255, 60)};
        case ProjectileCategory::Yellow:
            return {250.f, 12, sf::Color(255, 255, 100)};
    }
}

/// @brief Get default firing direction based on Allegiance.
/// @param allegiance Allegiance to base direction off of.
/// @return Vector direction that projectile should target.
sf::Vector2f ProjectileFactory::GetDefaultFireDirection(Allegiance allegiance) const
{
    switch (allegiance)
    {
        case Allegiance::Player:
        case Allegiance::Friendly:
            return {0.f, -1.f}; // Upward
        case Allegiance::Enemy:
            return {0.f, 1.f}; // Downward
        default:
            return {0.f, 0.f}; // No direction
    }
}

/// @brief Helper method to scale stats based on game difficulty.
/// @param stats Stats to update.
/// @param allegiance Only applicable to enemy units for difficulty scaling.
/// @return Inline scales ProjectileStats structure.
ProjectileStats ProjectileFactory::ApplyDifficultyScaling(ProjectileStats stats, Allegiance allegiance)
{
    if (allegiance != Allegiance::Enemy)
    {
        return stats;
    }

    switch (SettingsManager::Instance().GetSettings()->m_gameDifficulty)
    {
        case GameDifficultySetting::Easy:
            stats.speed *= 0.9f;
            stats.damage = static_cast<int>(stats.damage * 0.75f);
            break;

        case GameDifficultySetting::Normal:

            break;

        case GameDifficultySetting::Hard:
            stats.speed *= 1.1f;
            stats.damage = static_cast<int>(stats.damage * 1.25f);
            break;
    }

    return stats;
}