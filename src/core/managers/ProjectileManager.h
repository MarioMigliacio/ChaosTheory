// ============================================================================
//  File        : ProjectileManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : ProjectileManager is the CT collector of ingame projectiles.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseProjectile.h"
#include "CollisionManager.h"
#include "Settings.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// ============================================================================
//  Class       : ProjectileManager
//  Purpose     : Singleton class that manages the ingame projectiles.
//
//  Responsibilities:
//      - Update projectiles.
//      - Draw alive projectiles.
//      - Provide means to Add and Remove projectiles.
//      - Registers Projectiles for CollisionManagement.
//
// ============================================================================
class ProjectileManager
{
  public:
    static ProjectileManager &Instance();

    void Init();
    bool IsInitialized() const;
    void Shutdown();
    void Clear();

    void AddProjectile(const std::shared_ptr<BaseProjectile> &projectile);
    void RemoveProjectile(const std::shared_ptr<BaseProjectile> &projectile);

    std::vector<std::shared_ptr<BaseProjectile>> GetProjectiles() const;

    void Update(float dt);
    void Draw(sf::RenderTarget &target);

    void RegisterForCollision(CollisionManager &cm);

  private:
    ProjectileManager() = default;
    ~ProjectileManager() = default;

    ProjectileManager(const ProjectileManager &) = delete;
    ProjectileManager &operator=(const ProjectileManager &) = delete;

  private:
    std::vector<std::shared_ptr<BaseProjectile>> m_projectiles;

    bool m_isInitialized = false;
};
