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
#include "Settings.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// ============================================================================
//  Class       : ProjectileManager
//  Purpose     : Singleton class that manages the ingame projectiles.
//
//  Responsibilities:
//      - Initializes, Updates, Draws, and manages projectile collection.
//
// ============================================================================
class ProjectileManager
{
  public:
    static ProjectileManager &Instance();

    void Init(std::shared_ptr<const Settings> settings);
    void Shutdown();
    bool IsInitialized() const;

    void AddProjectile(const std::shared_ptr<BaseProjectile> &projectile);
    void RemoveProjectile(const std::shared_ptr<BaseProjectile> &projectile);
    void ClearAllProjectiles();

    std::vector<std::shared_ptr<BaseProjectile>> GetProjectiles() const;

    void Update(float dt);
    void Draw(sf::RenderTarget &target);

  private:
    ProjectileManager() = default;
    ~ProjectileManager() = default;

    ProjectileManager(const ProjectileManager &) = delete;
    ProjectileManager &operator=(const ProjectileManager &) = delete;

    std::vector<std::shared_ptr<BaseProjectile>> m_projectiles;
    std::shared_ptr<const Settings> m_settings;

    bool m_isInitialized = false;
};
