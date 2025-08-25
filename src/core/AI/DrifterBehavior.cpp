// ============================================================================
//  File        : DrifterBehavior.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-18
//  Description : Drift behavior. Ship moves steadily along a vector, usually
//                downward, with RNG-based horizontal offset. No shooting.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "DrifterBehavior.h"
#include "BaseShip.h"
#include "Macros.h"
#include <cmath>

/// @brief Deterministic state management of movement for DriftBehavior class.
/// @param ship Pointer to the Ship which inherits this interface.
/// @param dt Delta time since last update.
void DrifterBehavior::UpdateMovementLogic(BaseShip &ship, float dt)
{
    // On first tick, decide direction
    if (!m_initialized)
    {
        // 25% chance to drift left, 25% right, 50% straight
        std::uniform_int_distribution<int> dist(0, 3);
        int roll = dist(m_rng);

        // Use default down with no x direction, remains unchanged if RNG roll is 2, or 3.
        m_direction = {0.f, 1.f}; // straight down

        if (roll == 0)
        {
            m_direction.x = -0.25f; // slight left
        }

        else if (roll == 1)
        {
            m_direction.x = 0.25f; // slight right
        }

        m_direction = CT_MATH::Normalize({m_direction.x, m_direction.y});
        m_initialized = true;
    }

    // Use a scalar speed  to scale BOTH components. If x direction > 0, we'll see significant drift.
    const sf::Vector2f speed = ship.GetSpeed();
    const float speedScalar = speed.y;

    ship.Move({m_direction.x * speedScalar * dt, m_direction.y * speedScalar * dt});
}

/// @brief Deterministic state management of gun action for DriftBehavior class. Not used.
/// @param ship Pointer to the Ship which inherits this interface.
/// @param dt Delta time since last update.
void DrifterBehavior::UpdateGunLogic(BaseShip &ship, float dt)
{
    // No op
}
