// ============================================================================
//  File        : ProjectileFactory.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-20
//  Description : Manufacturer of projectile entities.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseProjectile.h"
#include <memory>

/// @brief Enumeration field for Projectile Categories
enum class ProjectileCategory
{
    /// @brief White is standard action projectile.
    White,

    /// @brief Red is a more fast, but less damaging projectile.
    Red,

    /// @brief Blue is a slower, higher damaging projectile.
    Blue,

    /// @brief Green is a balanced, higher output than white projectile.
    Green,

    /// @brief Yellow is a special projectile, slightly superior on average than other projectiles.
    Yellow,
};

/// @brief Structure which holds speed, damage and color tint.
struct ProjectileStats
{
    /// @brief Speed statistic for this ProjectileStats.
    float speed;

    /// @brief Damage statistic for this ProjectileStats.
    int damage;

    /// @brief Color Tint for this ProjectileStats.
    sf::Color tint;
};

// ============================================================================
//  Class       : ProjectileFactory
//  Purpose     : Singleton class that manages the generation of Projectile elements.
//
//  Responsibilities:
//      - Scale texture sizes and generate safe pointer Projectile objects.
//      - Scale damage and velocity baseed on game difficulty.
//
// ============================================================================
class ProjectileFactory
{
  public:
    static ProjectileFactory &Instance();

    std::shared_ptr<BaseProjectile> CreateBasicProjectile(const sf::Vector2f &pos, ProjectileCategory type,
                                                          Allegiance allegiance);
    std::shared_ptr<BaseProjectile> CreateBasicProjectile(const sf::Vector2f &pos, const sf::Vector2f &dir,
                                                          ProjectileCategory type, Allegiance allegiance);

    ProjectileStats GetStats(ProjectileCategory type) const;
    sf::Vector2f GetDefaultFireDirection(Allegiance allegiance) const;

  private:
    ProjectileStats ApplyDifficultyScaling(ProjectileStats stats, Allegiance allegiance);

  private:
    ProjectileFactory() = default;
};
