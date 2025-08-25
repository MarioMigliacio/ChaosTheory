// ============================================================================
//  File        : BerserkerBehavior.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-19
//  Description : Kamikaze behavior. On first tick, locks to the player's
//                position at spawn and flies straight along that fixed vector.
//                No shooting.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "BerserkerBehavior.h"
#include "BaseShip.h"
#include <cmath>

/// @brief Provide the player's position snapshot BEFORE the first Update().
/// @param playerPos The location to lock on (players position).
void BerserkerBehavior::SetSpawnTargetSnapshot(const sf::Vector2f &playerPos)
{
    m_spawnTargetSnapshot = playerPos;
    m_hasSnapshot = true;
    m_initialized = false;
}

/// @brief Ensures the ship has a direction to travel even if not locked on to player.
/// @param ship Pointer to ship for stats.
void BerserkerBehavior::EnsureDirectionInitialized(BaseShip &ship)
{
    if (m_initialized)
    {
        return;
    }

    if (!m_hasSnapshot)
    {
        // No snapshot provided; default to downward motion
        m_direction = {0.f, 1.f};
        m_initialized = true;

        return;
    }

    const sf::Vector2f from = ship.GetPosition();
    const sf::Vector2f target = m_spawnTargetSnapshot;

    m_direction = CT_MATH::Normalize({target.x - from.x, target.y - from.y});
    m_initialized = true;
}

/// @brief Optional helper to return whether or not this BerserkerBehavior is locked on to a target.
/// @return m_hasSnapshot, true / false.
bool BerserkerBehavior::HasSnapshot() const
{
    return m_hasSnapshot;
}

/// @brief Optional helper to remove any snapshot and initialized state.
void BerserkerBehavior::ClearSnapshot()
{
    m_hasSnapshot = false;
    m_initialized = false;
}

/// @brief Deterministic state management of movement for BerserkerBehavior class.
/// @param ship Pointer to the Ship which inherits this interface.
/// @param dt Delta time since last update.
void BerserkerBehavior::UpdateMovementLogic(BaseShip &ship, float dt)
{
    EnsureDirectionInitialized(ship);

    // Scale the heading by a scalar speed so X movement works even if speed.x == 0
    const sf::Vector2f s = ship.GetSpeed();
    const float speedScalar = s.y;

    ship.Move({m_direction.x * speedScalar * dt, m_direction.y * speedScalar * dt});
}

/// @brief Deterministic state management of gun action for BerserkerBehavior class. Not used.
/// @param ship Pointer to the Ship which inherits this interface.
/// @param dt Delta time since last update.
void BerserkerBehavior::UpdateGunLogic(BaseShip &ship, float dt)
{
    // No op
}
