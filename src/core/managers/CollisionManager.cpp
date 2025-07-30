// ============================================================================
//  File        : CollisionManager.cpp
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

#include "CollisionManager.h"
#include "BaseCollidable.h"
#include "BaseProjectile.h"
#include "BaseShip.h"
#include "Macros.h"
#include "UIIcon.h"
#include <algorithm>

/// @brief Get the current Instance for this CollisionManager singleton.
/// @return reference to existing CollisionManager interface.
CollisionManager &CollisionManager::Instance()
{
    static CollisionManager instance;
    return instance;
}

/// @brief Initialize the collision manager with grid settings.
/// @param cellSize Size of individual cells
void CollisionManager::Init(float cellSize)
{
    m_grid = SpatialGrid(cellSize);
    m_grid.Clear();
    m_objects.clear();
    m_initialized = true;

    CT_LOG_INFO("CollisionManager initialized.");
}

/// @brief Returns whether or not this Manager is Initialized.
/// @return true / false.
bool CollisionManager::IsInitialized() const
{
    return m_initialized;
}

/// @brief Shutdown and free resources.
void CollisionManager::Shutdown()
{
    Clear();
    m_initialized = false;

    CT_LOG_INFO("CollisionManager shutdown.");
}

/// @brief Clear all stored objects and grid data.
void CollisionManager::Clear()
{
    m_objects.clear();
    m_grid.Clear();
}

/// @brief Add an object to the collection of observable objects.
/// @param obj Object to include in observations for collision.
void CollisionManager::RegisterObject(std::shared_ptr<BaseCollidable> obj)
{
    if (obj && obj->IsAlive())
    {
        m_objects.push_back(obj);
    }
}

/// @brief Perform collision detection & resolution.
void CollisionManager::Update(float dt)
{
    if (!m_initialized)
    {
        return;
    }

    // 1. Rebuild grid
    m_grid.Clear();

    for (auto &obj : m_objects)
    {
        if (obj && obj->IsAlive())
        {
            m_grid.Insert(obj);
        }
    }

    // 2. Check collisions
    for (auto &obj : m_objects)
    {
        if (!obj || !obj->IsAlive())
        {
            continue;
        }

        auto candidates = m_grid.QueryNearby(obj->GetBounds());

        for (auto &other : candidates)
        {
            if (obj == other || !other->IsAlive())
            {
                continue;
            }

            if (!ShouldCollide(obj->GetCollisionCategory(), other->GetCollisionCategory()))
            {
                continue;
            }

            if (obj->GetBounds().intersects(other->GetBounds()))
            {
                ResolveCollision(obj, other);
            }
        }
    }
}

/// @brief Return whether or not the candidate CollisionCategory objects can even interact for a collision.
/// @param a CollisionCategory enumeration field.
/// @param b CollisionCategory enumeration field.
/// @return true / false.
bool CollisionManager::ShouldCollide(CollisionCategory a, CollisionCategory b) const
{
    if (a == CollisionCategory::Projectile && b == CollisionCategory::Projectile)
    {
        return false;
    }

    if (a == CollisionCategory::Icon && b == CollisionCategory::Icon)
    {
        return false;
    }

    if (a == CollisionCategory::None || b == CollisionCategory::None)
    {
        return false;
    }

    return true;
}

/// @brief If an object is found to be in bounds of a neighboring object, call the handler delegates for any supported
/// type of collision.
/// @param a Collidable supported interface object to compare against other.
/// @param b Collidable supported interface object to compare against other.
void CollisionManager::ResolveCollision(std::shared_ptr<BaseCollidable> a, std::shared_ptr<BaseCollidable> b)
{
    const auto catA = a->GetCollisionCategory();
    const auto catB = b->GetCollisionCategory();

    // -- Enemy ship vs Player Projectile -- //
    if (HandleCollisionEnemyVsProjectile(catA, catB, a, b))
    {
        // handled the collision.
        return;
    }

    // -- Player ship vs Enemy Projectile -- //
    if (HandleCollisionPlayerVsProjectile(catA, catB, a, b))
    {
        // handled the collision.
        return;
    }

    // -- Player ship vs Enemy ship -- //
    if (HandleCollisionEnemyVsPlayer(catA, catB, a, b))
    {
        // handled the collision.
        return;
    }

    // -- Player ship vs Collectable Icon -- //
    if (HandleCollisionPlayerVsIcon(catA, catB, a, b))
    {
        // handled the collision.
        return;
    }

    // -- Ship vs Environment -- //
    if (HandleCollisionShipVsEnvironment(catA, catB, a, b))
    {
        // handled the collision.
        return;
    }
}

/// @brief Returns true if Enemy Ship collision with Player Projectile.
/// @param catA either an Enemy ship or Projectile.
/// @param catB either an Enemy ship or Projectile.
/// @param a A safe pointer to either an Enemy ship or Projectile.
/// @param b A safe pointer to either an Enemy ship or Projectile.
/// @return true / false.
bool CollisionManager::HandleCollisionEnemyVsProjectile(CollisionCategory catA, CollisionCategory catB,
                                                        std::shared_ptr<BaseCollidable> a,
                                                        std::shared_ptr<BaseCollidable> b) const
{
    bool check = false;
    std::shared_ptr<BaseProjectile> projectile;
    std::shared_ptr<BaseShip> enemy;

    if (catA == CollisionCategory::Projectile && catB == CollisionCategory::Enemy)
    {
        check = true;
        projectile = std::dynamic_pointer_cast<BaseProjectile>(a);
        enemy = std::dynamic_pointer_cast<BaseShip>(b);
    }

    else if (catA == CollisionCategory::Enemy && catB == CollisionCategory::Projectile)
    {
        check = true;
        projectile = std::dynamic_pointer_cast<BaseProjectile>(b);
        enemy = std::dynamic_pointer_cast<BaseShip>(a);
    }

    if (check)
    {
        if (projectile && enemy && projectile->GetAllegiance() != enemy->GetAllegiance())
        {
            CT_LOG_DEBUG("CollisionManager: Enemy hit by player projectile; received {} damage.",
                         projectile->GetDamage());

            enemy->TakeDamage(projectile->GetDamage());
            projectile->Kill();
        }
    }

    return check;
}

/// @brief Returns true if Player Ship collision with Enemy Projectile.
/// @param catA either a Player ship or Enemy Projectile.
/// @param catB either a Player ship or Enemy Projectile.
/// @param a A safe pointer to either a Player ship or Enemy Projectile.
/// @param b A safe pointer to either a Player ship or Enemy Projectile.
/// @return true / false.
bool CollisionManager::HandleCollisionPlayerVsProjectile(CollisionCategory catA, CollisionCategory catB,
                                                         std::shared_ptr<BaseCollidable> a,
                                                         std::shared_ptr<BaseCollidable> b) const
{
    bool check = false;
    std::shared_ptr<BaseProjectile> projectile;
    std::shared_ptr<BaseShip> player;

    if (catA == CollisionCategory::Projectile && catB == CollisionCategory::Player)
    {
        check = true;
        projectile = std::dynamic_pointer_cast<BaseProjectile>(a);
        player = std::dynamic_pointer_cast<BaseShip>(b);
    }

    else if (catA == CollisionCategory::Player && catB == CollisionCategory::Projectile)
    {
        check = true;
        projectile = std::dynamic_pointer_cast<BaseProjectile>(b);
        player = std::dynamic_pointer_cast<BaseShip>(a);
    }

    if (check)
    {
        if (projectile && player && projectile->GetAllegiance() != player->GetAllegiance())
        {
            CT_LOG_DEBUG("CollisionManager: Player hit by enemy projectile; received {} damage.",
                         projectile->GetDamage());

            player->TakeDamage(projectile->GetDamage());
            projectile->Kill();
        }
    }

    return check;
}

/// @brief Returns true if Player Ship collision with Enemy Ship.
/// @param catA either a Player ship or Enemy Ship.
/// @param catB either a Player ship or Enemy Ship.
/// @param a A safe pointer to either a Player ship or Enemy Ship.
/// @param b A safe pointer to either a Player ship or Enemy Ship.
/// @return true / false
bool CollisionManager::HandleCollisionEnemyVsPlayer(CollisionCategory catA, CollisionCategory catB,
                                                    std::shared_ptr<BaseCollidable> a,
                                                    std::shared_ptr<BaseCollidable> b) const
{
    bool check = false;
    std::shared_ptr<BaseShip> enemy;
    std::shared_ptr<BaseShip> player;

    // For now, player will die when collision takes place.
    // Enemy dealt a large amount of damage.
    if (catA == CollisionCategory::Player && catB == CollisionCategory::Enemy)
    {
        check = true;
        std::dynamic_pointer_cast<BaseShip>(a)->Kill();
        std::dynamic_pointer_cast<BaseShip>(b)->TakeDamage(200);

        CT_LOG_DEBUG("CollisionManager: Player crashed into enemy ship; received fatal damage.");
    }

    else if (catA == CollisionCategory::Enemy && catB == CollisionCategory::Player)
    {
        check = true;
        std::dynamic_pointer_cast<BaseShip>(b)->Kill();
        std::dynamic_pointer_cast<BaseShip>(a)->TakeDamage(200);

        CT_LOG_DEBUG("CollisionManager: Player crashed into enemy ship; received fatal damage.");
    }

    return check;
}

/// @brief Returns true if Player Ship collision with Collectable Icon.
/// @param catA either a Player ship or Collectable Icon.
/// @param catB either a Player ship or Collectable Icon.
/// @param a A safe pointer to either a Player ship or Collectable Icon.
/// @param b A safe pointer to either a Player ship or Collectable Icon.
/// @return true / false
bool CollisionManager::HandleCollisionPlayerVsIcon(CollisionCategory catA, CollisionCategory catB,
                                                   std::shared_ptr<BaseCollidable> a,
                                                   std::shared_ptr<BaseCollidable> b) const
{
    bool check = false;
    std::shared_ptr<UIIcon> icon;
    std::shared_ptr<BaseShip> player;

    if (catA == CollisionCategory::Player && catB == CollisionCategory::Icon)
    {
        check = true;
        player = std::dynamic_pointer_cast<BaseShip>(a);
        icon = std::dynamic_pointer_cast<UIIcon>(b);
    }

    else if (catA == CollisionCategory::Icon && catB == CollisionCategory::Player)
    {
        check = true;
        player = std::dynamic_pointer_cast<BaseShip>(b);
        icon = std::dynamic_pointer_cast<UIIcon>(a);
    }

    if (check)
    {
        if (player && icon)
        {
            CT_LOG_DEBUG("CollisionManager: Player interaction with {}.", ToString(icon->GetIconType()));

            // TODO: Trigger icon effect (to be implemented in icon logic)
            icon->Expire();
        }
    }

    return check;
}

/// @brief Returns true if Player Ship, or Enemy Ship collision with Environment.
/// @param catA either a Player Ship, Enemy Ship or Environment.
/// @param catB either a Player Ship, Enemy Ship or Environment.
/// @param a A safe pointer to either a Player Ship, Enemy Ship or Environment.
/// @param b A safe pointer to either a Player Ship, Enemy Ship or Environment.
/// @return true / false
bool CollisionManager::HandleCollisionShipVsEnvironment(CollisionCategory catA, CollisionCategory catB,
                                                        std::shared_ptr<BaseCollidable> a,
                                                        std::shared_ptr<BaseCollidable> b) const
{
    bool pCheck = false;
    bool eCheck = false;
    std::shared_ptr<UIIcon> icon;
    std::shared_ptr<BaseShip> player;

    if (catA == CollisionCategory::Player && catB == CollisionCategory::Environment)
    {
        pCheck = true;
        std::dynamic_pointer_cast<BaseShip>(a)->Kill();
    }

    else if (catA == CollisionCategory::Environment && catB == CollisionCategory::Player)
    {
        pCheck = true;
        std::dynamic_pointer_cast<BaseShip>(b)->Kill();
    }

    else if (catA == CollisionCategory::Enemy && catB == CollisionCategory::Environment)
    {
        eCheck = true;
        std::dynamic_pointer_cast<BaseShip>(a)->Kill();
    }

    else if (catA == CollisionCategory::Environment && catB == CollisionCategory::Enemy)
    {
        eCheck = true;
        std::dynamic_pointer_cast<BaseShip>(b)->Kill();
    }

    if (pCheck)
    {
        CT_LOG_DEBUG("CollisionManager: Player ship crashed into meteor-like Environment object.");

        return true;
    }

    else if (eCheck)
    {
        CT_LOG_DEBUG("CollisionManager: Enemy ship crashed into meteor-like Environment object.");

        return true;
    }

    return false;
}
