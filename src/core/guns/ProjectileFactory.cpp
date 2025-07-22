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
#include "BasicProjectile.h"
#include "ResolutionScaleManager.h"
#include "SettingsManager.h"
#include <cmath>

ProjectileFactory &ProjectileFactory::Instance()
{
    static ProjectileFactory instance;

    return instance;
}

sf::Vector2f ProjectileFactory::GetDefaultFireDirection(Allegiance allegiance)
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

sf::Vector2f ProjectileFactory::GetDefaultBarrelOffset(Allegiance allegiance)
{
    constexpr float offsetY = 8.f; // TODO: Know that some ships barrel may be 32.f

    switch (allegiance)
    {
        case Allegiance::Player:
            return {0.f, -offsetY};
        case Allegiance::Enemy:
            return {0.f, offsetY};
        default:
            return {0.f, 0.f}; // Neutral/Friendly/Environment don't shoot
    }
}

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

ProjectileStats ProjectileFactory::GetStats(ProjectileCategory type)
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