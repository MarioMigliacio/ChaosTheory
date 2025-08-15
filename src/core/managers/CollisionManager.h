// ============================================================================
//  File        : CollisionManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-26
//  Description : Manages collision detection and resolution between
//                collidable objects.
//
//  Note        : Special acknowledgement to Ericson's
// "Real-Time Collision Detection" Broad-phase collision detection algorithm.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

// ============================================================================
//  Collision System Notes - uniform grid broad-phase algorithm
// ============================================================================
//
//  The CollisionManager is responsible for detecting and resolving collisions
//  between all objects in the game world that implement the BaseCollidable
//  interface. It works together with a SpatialGrid to efficiently reduce the
//  number of collision checks performed each frame.
//  ---------------------------------------------------------------------------
//  - All objects that can participate in collisions (ships, projectiles,
//    icons, etc.) are registered via RegisterObject().
//  - During Update(), the CollisionManager clears the SpatialGrid and then
//    re-inserts all currently active collidables into grid cells based on their
//    world-space bounds.
//  - Instead of checking every object against every other (O(N^2) checks),
//    the SpatialGrid only returns potential neighbors that share or border
//    the same grid cells as the object being tested. This drastically reduces
//    the number of intersection tests to O(N) in sparse scenarios.
//  - Performance scales much better as the number of objects increases.
//  - Grid cell size is tunable (defaults are chosen to balance speed and
//    accuracy).
//  ---------------------------------------------------------------------------
//  - Once candidate pairs are retrieved from the grid, CollisionManager:
//      1. Skips invalid or dead objects.
//      2. Applies ShouldCollide() rules to filter out pairs that should not
//         interact (e.g., projectile vs projectile, icon vs icon).
//      3. Performs a simple bounding-box intersection check.
//      4. If intersecting, ResolveCollision() is called.
//
//  - ResolveCollision() delegates to several helper methods:
//      - HandleCollisionEnemyVsProjectile
//      - HandleCollisionPlayerVsProjectile
//      - HandleCollisionEnemyVsPlayer
//      - HandleCollisionPlayerVsIcon
//      - HandleCollisionShipVsEnvironment
//
//  - Each helper returns true if it handled the collision, preventing further
//    unnecessary checks.
// ============================================================================

#pragma once

#include "BaseCollidable.h"
#include "PlayerShip.h"
#include "SpatialGrid.h"
#include "UIIcon.h"
#include <memory>
#include <vector>

/// @brief Anonymous namespace to support constant for CollisionManager grid size.
namespace
{
/// @brief The size for the SpatialGrid cells for determining neighbors.
constexpr float GRID_SIZE = 128.f;
} // namespace

// ============================================================================
//  Class       : CollisionManager
//  Purpose     : Singleton class that manages the Collision detection events.
//
//  Responsibilities:
//      - CollisionManager + SpatialGrid = fast, scalable collision checks.
//      - only process meaningful object pairs.
//
// ============================================================================
class CollisionManager
{
  public:
    static CollisionManager &Instance();

    void Init(float cellSize);
    bool IsInitialized() const;
    void Shutdown();
    void Clear();

    void RegisterObject(std::shared_ptr<BaseCollidable> obj);
    void Update(float dt);

    bool HandleBombCollisionWithEnemyOrWorld(const BaseCollidable &projectile) const;
    void ApplyAreaDamage(const sf::Vector2f &center, float radius, float damage, Allegiance sourceAllegiance);
    std::shared_ptr<BaseCollidable> GetNearestTarget(const sf::Vector2f &from, Allegiance sourceAllegiance) const;

  private:
    CollisionManager() = default;
    ~CollisionManager() = default;

    CollisionManager(const CollisionManager &) = delete;
    CollisionManager &operator=(const CollisionManager &) = delete;

    void ResolveCollision(std::shared_ptr<BaseCollidable> a, std::shared_ptr<BaseCollidable> b);
    bool ShouldCollide(CollisionCategory catA, CollisionCategory catB) const;

    bool HandleCollisionEnemyVsProjectile(CollisionCategory catA, CollisionCategory catB,
                                          std::shared_ptr<BaseCollidable> a, std::shared_ptr<BaseCollidable> b) const;
    bool HandleCollisionPlayerVsProjectile(CollisionCategory catA, CollisionCategory catB,
                                           std::shared_ptr<BaseCollidable> a, std::shared_ptr<BaseCollidable> b) const;
    bool HandleCollisionEnemyVsPlayer(CollisionCategory catA, CollisionCategory catB, std::shared_ptr<BaseCollidable> a,
                                      std::shared_ptr<BaseCollidable> b) const;
    bool HandleCollisionPlayerVsIcon(CollisionCategory catA, CollisionCategory catB, std::shared_ptr<BaseCollidable> a,
                                     std::shared_ptr<BaseCollidable> b) const;
    bool HandleCollisionShipVsEnvironment(CollisionCategory catA, CollisionCategory catB,
                                          std::shared_ptr<BaseCollidable> a, std::shared_ptr<BaseCollidable> b) const;

  private:
    SpatialGrid m_grid{GRID_SIZE};
    std::vector<std::shared_ptr<BaseCollidable>> m_objects;

    bool m_initialized = false;
};
