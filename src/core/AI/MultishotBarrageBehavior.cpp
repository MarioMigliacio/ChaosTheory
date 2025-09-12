// ============================================================================
//  File        : MultishotBarrageBehavior.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-07
//  Description : Drifts with a lateral wobble, aims at player, fires a multishot
//                volley, rotates slightly inbetween X volleys, then cools down.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "MultishotBarrageBehavior.h"
#include "BaseShip.h"
#include "ConfigurableGun.h"
#include "GunPattern.h"
#include "Macros.h"
#include "ProjectileFactory.h"
#include "ProjectileManager.h"
#include "ShipManager.h"
#include "WindowManager.h"
#include <algorithm>
#include <cmath>

/// @brief Constants that can be adjusted throughout the MultishotBarrageBehavior.
namespace
{
/// @brief Controls base downward drift.
constexpr float DRIFT_SPEED_Y = 95.f;

/// @brief Controls base x axis drift.
constexpr float DRIFT_SPEED_X_BASE = 60.f;

/// @brief Adds a little spice in the x direction at the start of a new drift.
constexpr float DRIFT_SPEED_X_JIT = 0.20f;

/// @brief Padding so the entity doesn't go off screen.
constexpr float SCREEN_MARGIN_X = 12.f;

/// @brief Adds appropriate delay before firing initial multi shot barrage.
constexpr float AIM_WINDUP_TIME = 0.20f;

/// @brief Controls the speed the entity rotates during lock on period.
constexpr float ROTATE_SPEED_DEG_PER_SEC = 240.f;

/// @brief Controls the number of times the entity fires a barrage after locking on.
constexpr int VOLLEYS_PER_BARRAGE = 2;

/// @brief Controls time between volley fire.
constexpr float BETWEEN_VOLLEYS_DELAY = 0.12f;

/// @brief Controls how many bullets are lobbed in a single volley.
constexpr int PELLETS_PER_VOLLEY = 5;

/// @brief Controls the angle between bullets in a single volley [-half, +half].
constexpr float SPREAD_HALF_ANGLE_DEG = 12.f;

/// @brief The degree amount turn inbetween volleys to add difficulty in dodging fire.
constexpr float TURN_STEP_ANGLE_DEG = 7.f;

/// @brief Controls the cooldown period before allowed to attempt another volley.
constexpr float COOLDOWN_TIME = 2.4f;
} // namespace

/// @brief Deterministic state management of movement for MultishotBarrageBehavior class.
/// @param ship Pointer to the Ship which inherits this interface.
/// @param dt Delta time since last update.
void MultishotBarrageBehavior::UpdateMovementLogic(BaseShip &ship, float dt)
{
    switch (m_phase)
    {
        case Phase::Drift:
        {
            if (m_driftDirX == 0.f || m_driftSpeedX <= 0.f)
            {
                BeginDrift();
            }

            DoDrift(ship, dt);

            m_phaseTimer -= dt;

            if (m_phaseTimer <= 0.f)
            {
                BeginAim(ship);
                m_phase = Phase::Aim;
                m_phaseTimer = AIM_WINDUP_TIME;
            }

            break;
        }

        case Phase::Aim:
        {
            DoAim(ship, dt);
            m_phaseTimer -= dt;

            if (m_phaseTimer <= 0.f)
            {
                BeginBarrage();
                m_phase = Phase::Barrage;
            }

            break;
        }

        case Phase::Barrage:
        {
            ship.Move({0.f, DRIFT_SPEED_Y * 0.25f * dt});
            DoBarrage(ship, dt);

            break;
        }

        case Phase::Cooldown:
        {
            DoDrift(ship, dt);
            m_phaseTimer -= dt;

            if (m_phaseTimer <= 0.f)
            {
                m_phase = Phase::Drift;
                BeginDrift();
                m_phaseTimer = CT_MATH::RandRange(0.10f, 0.35f);
            }

            break;
        }
    }
}

/// @brief Deterministic state management of gun action for MultishotBarrageBehavior class. Fires gun when available.
/// @param ship Pointer to the Ship which inherits this interface.
/// @param dt Delta time since last update.
void MultishotBarrageBehavior::UpdateGunLogic(BaseShip &ship, float dt)
{
    // Keep gun internals ticking so cooldowns are respected if we use it.
    if (auto *gun = ship.GetGun())
    {
        gun->SetOwnerPosition(ship.GetPosition());
        gun->Update(dt);
    }
}

/// @brief Helper method to prepping the ship for the Drifting behavior phase.
void MultishotBarrageBehavior::BeginDrift()
{
    // drift towards random direction in x.
    m_driftDirX = (CT_MATH::RandRange(0.f, 1.f) < 0.5f) ? -1.f : 1.f;

    // Speed with a small jitter
    const float jitter = 1.f + CT_MATH::RandRange(-DRIFT_SPEED_X_JIT, DRIFT_SPEED_X_JIT);
    m_driftSpeedX = DRIFT_SPEED_X_BASE * std::max(0.1f, jitter);
}

/// @brief Helper method to make the ship perform drifting during the movement phase.
/// @param ship The ship to enact upon.
/// @param dt Delta time since last update.
void MultishotBarrageBehavior::DoDrift(BaseShip &ship, float dt)
{
    // Initialize drift on first use (covers initial spawn case)
    if (m_driftDirX == 0.f || m_driftSpeedX <= 0.f)
    {
        BeginDrift();
    }

    // Move with constant X and steady Y
    ship.Move({m_driftDirX * m_driftSpeedX * dt, DRIFT_SPEED_Y * dt});

    // Soft keep-in-bounds in X: flip direction if near edges
    const auto &win = WindowManager::Instance().GetWindow();
    const auto sz = win.getSize();
    const float x = ship.GetPosition().x;

    if (x < SCREEN_MARGIN_X)
    {
        ship.Move({SCREEN_MARGIN_X - x, 0.f});
        m_driftDirX = +1.f; // right
    }

    else if (x > static_cast<float>(sz.x) - SCREEN_MARGIN_X)
    {
        ship.Move({(static_cast<float>(sz.x) - SCREEN_MARGIN_X) - x, 0.f});
        m_driftDirX = -1.f; // left
    }
}

/// @brief Helper method to target the player ship in preparation for the upcoming barrage phase.
/// @param ship Pointer to our ship.
void MultishotBarrageBehavior::BeginAim(BaseShip &ship)
{
    // Snapshot target angle toward player
    if (auto player = ShipManager::Instance().GetPlayer())
    {
        const sf::Vector2f d = player->GetPosition() - ship.GetPosition();
        m_targetAngleRad = std::atan2(d.y, d.x);
    }

    else
    {
        m_targetAngleRad = PI * 0.5f;
    }

    // turn angle inbetween volleys.
    m_turnDir = (CT_MATH::RandRange(0.f, 1.f) < 0.5f) ? -1.f : 1.f;
    m_aimAngleRad = m_targetAngleRad;
    SetSpriteRotation(ship, m_aimAngleRad);
}

/// @brief Helper method to update frame by frame action of aiming and rotating for smooth behavior.
/// @param ship Pointer to our ship
/// @param dt Delta time since last updpate.
void MultishotBarrageBehavior::DoAim(BaseShip &ship, float dt)
{
    // Smoothly rotate toward target even if player moved during windup
    const float maxStepRad = CT_MATH::DegToRad(ROTATE_SPEED_DEG_PER_SEC) * dt;

    if (auto player = ShipManager::Instance().GetPlayer())
    {
        const sf::Vector2f d = player->GetPosition() - ship.GetPosition();
        m_targetAngleRad = std::atan2(d.y, d.x);
    }

    m_aimAngleRad = ApproachAngle(m_aimAngleRad, m_targetAngleRad, maxStepRad);
    SetSpriteRotation(ship, m_aimAngleRad);
}

/// @brief Helper method to setup the internals for an upcoming barrage phase.
void MultishotBarrageBehavior::BeginBarrage()
{
    m_volleysFired = 0;
    m_betweenVolleyTimer = 0.f;
}

/// @brief Helper method to update frame by frame action of firing a multi shot barrage.
/// @param ship Pointer to the ship to enact upon.
/// @param dt Delta time since last update.
void MultishotBarrageBehavior::DoBarrage(BaseShip &ship, float dt)
{
    m_betweenVolleyTimer -= dt;

    if (m_volleysFired < VOLLEYS_PER_BARRAGE && m_betweenVolleyTimer <= 0.f)
    {
        // volley N
        FireVolley(ship, m_aimAngleRad);
        m_volleysFired++;

        // Perform a strategical rotation so that barrages become harder to avoid.
        if (m_volleysFired < VOLLEYS_PER_BARRAGE)
        {
            m_aimAngleRad += CT_MATH::DegToRad(TURN_STEP_ANGLE_DEG) * m_turnDir;
            m_betweenVolleyTimer = BETWEEN_VOLLEYS_DELAY;
            SetSpriteRotation(ship, m_aimAngleRad);
        }

        else
        {
            m_phase = Phase::Cooldown;
            m_phaseTimer = COOLDOWN_TIME;
        }
    }
}

/// @brief Helper method to acquire the ships gun, and request a multi shot fire.
/// @param ship Pointer to our ship.
/// @param baseAngleRad Aim direction for volley.
void MultishotBarrageBehavior::FireVolley(BaseShip &ship, float baseAngleRad)
{
    if (auto *cfg = dynamic_cast<ConfigurableGun *>(ship.GetGun()))
    {
        cfg->SetOwnerPosition(ship.GetPosition());

        const int n = std::max(1, PELLETS_PER_VOLLEY);
        std::vector<float> anglesDeg;
        anglesDeg.reserve(n);

        if (n == 1)
        {
            anglesDeg.push_back(0.f);
        }

        else
        {
            const float span = 2.f * SPREAD_HALF_ANGLE_DEG;
            for (int i = 0; i < n; ++i)
            {
                const float t = static_cast<float>(i) / static_cast<float>(n - 1);
                const float deg = -SPREAD_HALF_ANGLE_DEG + t * span;
                anglesDeg.push_back(deg);
            }
        }

        // Aim direction from current aim angle
        const sf::Vector2f baseDir{std::cos(baseAngleRad), std::sin(baseAngleRad)};
        const sf::Vector2f pos = ship.GetPosition();

        // Fire immediately; behavior handles cadence cooldown is behavior-level here.
        cfg->FireSpread(pos, baseDir, anglesDeg);

        return;
    }
}

/// @brief Helper method for the rotation of the ship during aiming and inbetween volleys.
/// @param ship Pointer to the ship to change.
/// @param angleRad Angle in radians for the rotation.
void MultishotBarrageBehavior::SetSpriteRotation(BaseShip &ship, float angleRad)
{
    const float deg = CT_MATH::RadToDeg(angleRad) + 90.f;
    ship.SetRotation(deg);
}

/// @brief Helper method that keeps any angle (in radians) inside a canonical range of [-PI, +PI]
/// @param a Angle for consideration.
/// @return The shortest angle towards target.
float MultishotBarrageBehavior::WrapAngleRad(float a)
{
    // NOTE: Rotates the short way toward the player and never does a weird 350° spin when crossing the wrap boundary.
    // Shamelessly stolen from smarter folks than myself.

    while (a < -PI)
    {
        a += 2.f * PI;
    }

    while (a > PI)
    {
        a -= 2.f * PI;
    }

    return a;
}

/// @brief Helper method to step toward target along the shortest arc
/// @param current Current angle (radians).
/// @param target Desired angle (radians).
/// @param maxStepRad Maximum change to apply this call (radians, ≥ 0).
/// @return New angle in [-PI, +PI].
float MultishotBarrageBehavior::ApproachAngle(float current, float target, float maxStepRad)
{
    // Now diff is guaranteed to be the shortest signed angular difference.
    current = WrapAngleRad(current);
    target = WrapAngleRad(target);
    float diff = WrapAngleRad(target - current);

    if (std::fabs(diff) <= maxStepRad)
    {
        return target;
    }

    return current + ((diff > 0.f) ? maxStepRad : -maxStepRad);
}
