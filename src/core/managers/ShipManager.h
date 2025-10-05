// ============================================================================
//  File        : ShipManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-26
//  Description : Manages all ship entities (player and enemies) in the game.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "CollisionManager.h"
#include "PlayerShip.h"
#include <memory>
#include <vector>

// ============================================================================
//  Class       : ShipManager
//  Purpose     : Singleton class that manages the Life cycle of Ships.
//
//  Responsibilities:
//      - Update Ships.
//      - Draw alive Ships.
//      - Provide means to Spawn Ships.
//      - Registers Ships for CollisionManagement.
//
// ============================================================================
class ShipManager
{
  public:
    static ShipManager &Instance();

    void Init();
    bool IsInitialized() const;
    void Shutdown();
    void Clear();

    void SpawnPlayer(const sf::Vector2f &pos);
    void SpawnBasicEnemy(const sf::Vector2f &pos);
    void SpawnAlienEnemy(const sf::Vector2f &pos);
    void SpawnBerserkerEnemy(const sf::Vector2f &pos);
    void SpawnCrusaderEnemy(const sf::Vector2f &pos);
    void SpawnGruntEnemy(const sf::Vector2f &pos);
    void SpawnInvaderEnemy(const sf::Vector2f &pos);

    std::shared_ptr<PlayerShip> GetPlayer() const;
    const std::vector<std::shared_ptr<BaseShip>> &GetEnemies() const;
    const int GetEnemyCount() const;

    void Update(float dt);
    void Draw(sf::RenderTarget &target);

    void RegisterForCollision(CollisionManager &cm);

  private:
    ShipManager() = default;
    ~ShipManager() = default;

    ShipManager(const ShipManager &) = delete;
    ShipManager &operator=(const ShipManager &) = delete;

  private:
    std::shared_ptr<PlayerShip> m_player;
    std::vector<std::shared_ptr<BaseShip>> m_enemies;

    bool m_initialized = false;
};
