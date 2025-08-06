// ============================================================================
//  File        : CollectableIconManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-27
//  Description : Manages all collectible UIIcon objects that exist in the game world.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "CollisionManager.h"
#include "UIIcon.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// ============================================================================
//  Class       : CollectableIconManager
//  Purpose     : Manager for handling the life cycle of UIIcons in game.
//
//  Responsibilities:
//      - Update Icons.
//      - Draw alive Icons.
//      - Provide means to SpawnIcons.
//      - Registers Icons for CollisionManagement.
//
// ============================================================================
class CollectableIconManager
{
  public:
    static CollectableIconManager &Instance();

    void Init();
    bool IsInitialized() const;
    void Shutdown();
    void Clear();

    void SpawnIcon(const IconConfig &config);

    const std::vector<std::shared_ptr<UIIcon>> &GetIcons() const;

    void Update(float dt);
    void Draw(sf::RenderTarget &target);

    void RegisterForCollision(CollisionManager &cm);

  private:
    CollectableIconManager() = default;
    ~CollectableIconManager() = default;

    CollectableIconManager(const CollectableIconManager &) = delete;
    CollectableIconManager &operator=(const CollectableIconManager &) = delete;

  private:
    std::vector<std::shared_ptr<UIIcon>> m_icons;
    bool m_initialized = false;
};
