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
#include "BombProjectile.h"
#include "ProjectileStats.h"
#include <memory>

// ============================================================================
//  Class       : ProjectileFactory
//  Purpose     : Singleton class that manages the generation of Projectile
//                elements.
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

    std::shared_ptr<BaseProjectile> CreateBasicProjectile(const sf::Vector2f &pos, const sf::Vector2f &dir,
                                                          const ProjectileStats &stats, Allegiance allegiance);
    std::shared_ptr<BaseProjectile> CreateBombProjectile(const sf::Vector2f &pos, const sf::Vector2f &velocity,
                                                         Allegiance allegiance,
                                                         const BombProjectileConfig &config = BombProjectileConfig{});

  private:
    ProjectileFactory() = default;
};
