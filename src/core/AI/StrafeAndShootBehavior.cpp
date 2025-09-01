// ============================================================================
//  File        : StrafeAndShootBehavior.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-18
//  Description : Behavior for moving side by side, and shooting.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "StrafeAndShootBehavior.h"
#include "BaseShip.h"
#include "ProjectileManager.h"
#include "WindowManager.h"
#include <cmath>

/// @brief Constants that can be adjusted throughout the StrafeAndShootBehavior.
namespace
{
/// @brief Configurable time between random horizontal direction changes.
constexpr float DIRECTION_CHANGE_INTERVAL = 1.0f;

/// @brief Configurable multiplier to reduce horizontal strafe speed.
constexpr float INTERNAL_X_DAMPENER = 0.5f;

/// @brief Configurable multiplier to reduce vertical speed.
constexpr float INTERNAL_Y_DAMPENER = .25f;

/// @brief Configurable seconds to wait before the first shot.
constexpr float INITIAL_HOLD_TIME = 1.0f;
} // namespace

/// @brief Simple constructor for establishing configurables used during update behaviors.
StrafeAndShootBehavior::StrafeAndShootBehavior()
{
    m_directionChangeCooldown = DIRECTION_CHANGE_INTERVAL;
    m_initialHoldTime = INITIAL_HOLD_TIME;

    std::uniform_int_distribution<int> dist(0, 1);
    m_currentDirX = (dist(m_rng) == 0) ? -1.f : 1.f;
}

/// @brief Deterministic state management of movement for StrafeAndShootBehavior class.
/// @param ship Pointer to the Ship which inherits this interface.
/// @param dt Delta time since last update.
void StrafeAndShootBehavior::UpdateMovementLogic(BaseShip &ship, float dt)
{
    // 1) Random horizontal direction flip on interval
    m_directionChangeCooldown -= dt;

    if (m_directionChangeCooldown <= 0.f)
    {
        // RNG 0: negative x direction (left)
        // RNG 1: positive x direction
        std::uniform_int_distribution<int> dist(0, 1);
        m_currentDirX = (dist(m_rng) == 0) ? -1.f : 1.f;
        m_directionChangeCooldown = DIRECTION_CHANGE_INTERVAL;
    }

    // 2) Strafe X + slow descent Y
    const sf::Vector2f speed = ship.GetSpeed();
    sf::Vector2f delta{m_currentDirX * speed.x * INTERNAL_X_DAMPENER * dt, (speed.y * INTERNAL_Y_DAMPENER) * dt};
    ship.Move(delta);

    // 3) Clamp horizontally to screen bounds
    const auto win = WindowManager::Instance().GetWindow().getSize();
    const sf::FloatRect b = ship.GetBounds();
    const float left = b.left;
    const float right = b.left + b.width;

    if (left < 0.f)
    {
        ship.Move({-left, 0.f});
    }

    else if (right > static_cast<float>(win.x))
    {
        ship.Move({static_cast<float>(win.x) - right, 0.f});
    }
}

/// @brief Deterministic state management of gun action for StrafeAndShootBehavior class. Fires gun when available.
/// @param ship Pointer to the Ship which inherits this interface.
/// @param dt Delta time since last update.
void StrafeAndShootBehavior::UpdateGunLogic(BaseShip &ship, float dt)
{
    auto *gun = ship.GetGun();

    if (!gun)
    {
        return;
    }

    gun->SetOwnerPosition(ship.GetPosition());
    gun->Update(dt);

    if (m_initialHoldTime > 0.f)
    {
        m_initialHoldTime -= dt;

        return;
    }

    auto proj = gun->TryFire();

    if (proj)
    {
        ProjectileManager::Instance().AddProjectile(proj);
    }
}
