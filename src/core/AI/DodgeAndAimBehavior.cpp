// ============================================================================
//  File        : DodgeAndAimBehavior.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-05
//  Description : Short-hop dodge to a nearby X/Y, snapshot-aim at player's
//                current location, fire a single projectile, then drift & cool.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "DodgeAndAimBehavior.h"
#include "BaseShip.h"
#include "ConfigurableGun.h"
#include "Macros.h"
#include "ProjectileManager.h"
#include "ShipManager.h"
#include "WindowManager.h"
#include <algorithm>
#include <cmath>

/// @brief Constants that can be adjusted throughout the DodgeAndAimBehavior.
namespace
{
/// @brief Horizontal/vertical hop speed toward the short-distance dodge target.
static constexpr float HOP_SPEED = 260.f;

/// @brief Constant vertical drift while not explicitly moving to target.
static constexpr float DRIFT_SPEED_Y = 80.f;

/// @brief Distance tolerance to consider the target reached.
static constexpr float TARGET_TOLERANCE = 6.f;

/// @brief Target within this min x distance from current pos for a Dodge hop.
static constexpr float HOP_MIN_X = 40.f;

/// @brief Target within this max x distance from current pos for a Dodge hop.
static constexpr float HOP_MAX_X = 140.f;

/// @brief Target within this min y distance from current pos for a Dodge hop.
static constexpr float HOP_MIN_Y = 20.f;

/// @brief Target within this max y distance from current pos for a Dodge hop.
static constexpr float HOP_MAX_Y = 60.f;

/// @brief Time constraint before firing during the Aim and Fire phase.
static constexpr float AIM_WINDUP = 0.15f;

/// @brief Cooldown after firing before another cycle can begin.
static constexpr float COOLDOWN_TIME = 2.0f;

/// @brief Clamp margins so target stays on screen.
static constexpr float SCREEN_MARGIN = 16.f;

/// @brief Bullet default speed.
static constexpr float BULLET_SPEED = 420.f;

/// @brief Bullet default damage.
static constexpr float BULLET_DAMAGE = 8.f;
} // namespace

/// @brief Performs routine updates during a frame.
/// @param ship The ship to perform the updpate on.
/// @param dt Delta time since last update.
void DodgeAndAimBehavior::UpdateMovementLogic(BaseShip &ship, float dt)
{
    switch (m_phase)
    {
        case Phase::ChooseTarget:
        {
            PickNearbyTarget(ship);
            m_phase = Phase::MoveToTarget;

            break;
        }

        case Phase::MoveToTarget:
        {
            MoveTowardTarget(ship, dt);

            // Small constant downward drift so it still advances the screen.
            ship.Move({0.f, DRIFT_SPEED_Y * 0.35f * dt});

            const sf::Vector2f dx = m_target - ship.GetPosition();

            if (std::abs(dx.x) <= TARGET_TOLERANCE && std::abs(dx.y) <= TARGET_TOLERANCE)
            {
                // Snapshot player's current position at aim time.
                if (auto player = ShipManager::Instance().GetPlayer())
                {
                    m_snapshotPlayerPos = player->GetPosition();
                }

                else
                {
                    m_snapshotPlayerPos = ship.GetPosition() + sf::Vector2f{0.f, 32.f};
                }

                m_phaseTimer = AIM_WINDUP;
                m_fired = false;
                m_phase = Phase::AimAndFire;
            }

            break;
        }

        case Phase::AimAndFire:
        {
            // Hover slightly while aiming.
            ship.Move({0.f, DRIFT_SPEED_Y * 0.15f * dt});

            m_phaseTimer -= dt;

            if (m_phaseTimer <= 0.f && !m_fired)
            {
                FireAimedShot(ship);
                m_fired = true;
                m_phase = Phase::Cooldown;
                m_phaseTimer = COOLDOWN_TIME;
            }

            break;
        }

        case Phase::Cooldown:
        {
            // Drift downward during cooldown.
            ship.Move({0.f, DRIFT_SPEED_Y * dt});

            m_phaseTimer -= dt;

            if (m_phaseTimer <= 0.f)
            {
                m_phase = Phase::ChooseTarget;
                m_hasTarget = false;
            }

            break;
        }
    }
}

/// @brief Performs routine gun logic during an update frame.
/// @param ship Reference to Ship.
/// @param dt delta time since last update.
void DodgeAndAimBehavior::UpdateGunLogic(BaseShip &ship, float dt)
{
    if (auto *gun = ship.GetGun())
    {
        gun->SetOwnerPosition(ship.GetPosition());
        gun->Update(dt);
    }
}

/// @brief Helper function to locate a nearby X/Y coordinate to use as the Hop logic for Dodge and Aim.
/// @param ship Reference to the ship.
void DodgeAndAimBehavior::PickNearbyTarget(const BaseShip &ship)
{
    const auto &window = WindowManager::Instance().GetWindow();
    const auto winSize = window.getSize();

    const sf::Vector2f pos = ship.GetPosition();

    const float dx = CT_MATH::RandRange(HOP_MIN_X, HOP_MAX_X) * (CT_MATH::RandRange(0.f, 1.f) < 0.5f ? -1.f : 1.f);
    const float dy = CT_MATH::RandRange(HOP_MIN_Y, HOP_MAX_Y) * (CT_MATH::RandRange(0.f, 1.f) < 0.5f ? -1.f : 1.f);

    sf::Vector2f candidate = {pos.x + dx, pos.y + dy};

    // Clamp within the screen bounds with a small margin.
    candidate.x = std::clamp(candidate.x, SCREEN_MARGIN, static_cast<float>(winSize.x) - SCREEN_MARGIN);
    candidate.y = std::clamp(candidate.y, SCREEN_MARGIN, static_cast<float>(winSize.y) - SCREEN_MARGIN);

    m_target = candidate;
    m_hasTarget = true;
}

/// @brief Helpepr function to provide step movements for the ship during the Dodge portion of logic.
/// @param ship Reference to the ship.
/// @param dt delta time since last update.
void DodgeAndAimBehavior::MoveTowardTarget(BaseShip &ship, float dt)
{
    if (!m_hasTarget)
    {
        return;
    }

    const sf::Vector2f p = ship.GetPosition();
    const sf::Vector2f dp = m_target - p;

    // Stop if very close
    if (std::abs(dp.x) <= TARGET_TOLERANCE && std::abs(dp.y) <= TARGET_TOLERANCE)
    {
        return;
    }

    const sf::Vector2f dir = CT_MATH::Normalize(dp);
    const sf::Vector2f vel = {dir.x * HOP_SPEED, dir.y * HOP_SPEED};

    ship.Move({vel.x * dt, vel.y * dt});
}

/// @brief Fire a single projectile at the target location.
/// @param ship Reference to the ship that is firing.
void DodgeAndAimBehavior::FireAimedShot(BaseShip &ship)
{
    if (auto *cfg = dynamic_cast<ConfigurableGun *>(ship.GetGun()))
    {
        cfg->SetOwnerPosition(ship.GetPosition());
        cfg->SetPattern(GunPattern::DirectedShot);

        if (auto proj = cfg->TryFireTowards(m_snapshotPlayerPos))
        {
            ProjectileManager::Instance().AddProjectile(proj);

            return;
        }
    }
}
