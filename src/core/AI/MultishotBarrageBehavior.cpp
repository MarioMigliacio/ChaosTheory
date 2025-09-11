// ============================================================================
//  File        : MultishotBarrageBehavior.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-07
//  Description : Implementation for Invader-style multishot barrage behavior.
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
#include <random>

/// @brief Constants that can be adjusted throughout the MultishotBarrageBehavior.
namespace
{
constexpr float DRIFT_SPEED_Y = 95.f;      // base downward drift
constexpr float DRIFT_SPEED_X_BASE = 60.f; // base lateral speed
constexpr float DRIFT_SPEED_X_JIT = 0.20f; // ±20% random jitter

constexpr float SCREEN_MARGIN_X = 12.f;

constexpr float AIM_WINDUP_TIME = 0.20f;          // brief "lock-on"
constexpr float ROTATE_SPEED_DEG_PER_SEC = 240.f; // sprite rotate speed
constexpr float MAX_AIM_DURATION = 0.35f;         // failsafe

constexpr int VOLLEYS_PER_BARRAGE = 2;         // exactly 2
constexpr float BETWEEN_VOLLEYS_DELAY = 0.12f; // seconds between 1st and 2nd
constexpr float TURN_STEP_ANGLE_DEG = 7.f;     // rotate a bit before 2nd volley

// Cooldown
constexpr float COOLDOWN_TIME = 2.4f; // drift & wait

// Math helpers
inline float DegToRad(float d)
{
    return d * (PI / 180.f);
}
inline float RadToDeg(float r)
{
    return r * (180.f / PI);
}

// Random
inline float RandRange(float lo, float hi)
{
    static thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(lo, hi);

    return dist(rng);
}
} // namespace

void MultishotBarrageBehavior::UpdateMovementLogic(BaseShip &ship, float dt)
{
    switch (m_phase)
    {
        case Phase::Drift:
        {
            if (m_driftDirX == 0.f || m_driftSpeedX <= 0.f)
            {
                BeginDrift(ship);
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

            // Either windup done or timeout safety reached.
            if (m_phaseTimer <= 0.f)
            {
                BeginBarrage();
                m_phase = Phase::Barrage;
            }

            break;
        }

        case Phase::Barrage:
        {
            // drift ever so slightly while firing
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
                // (Re)enter Drift: pick a fresh lateral direction/speed and a short pre-aim delay
                m_phase = Phase::Drift;
                BeginDrift(ship);
                m_phaseTimer = RandRange(0.10f, 0.35f);
            }

            break;
        }
    }
}

void MultishotBarrageBehavior::UpdateGunLogic(BaseShip &ship, float dt)
{
    // Keep gun internals ticking so cooldowns are respected if we use it.
    if (auto *gun = ship.GetGun())
    {
        gun->SetOwnerPosition(ship.GetPosition());
        gun->Update(dt);
    }
}

void MultishotBarrageBehavior::BeginDrift(BaseShip &ship)
{
    // Random left/right
    m_driftDirX = (RandRange(0.f, 1.f) < 0.5f) ? -1.f : 1.f;

    // Speed with a small jitter
    const float jitter = 1.f + RandRange(-DRIFT_SPEED_X_JIT, DRIFT_SPEED_X_JIT);
    m_driftSpeedX = DRIFT_SPEED_X_BASE * std::max(0.1f, jitter);
}

void MultishotBarrageBehavior::DoDrift(BaseShip &ship, float dt)
{
    // Initialize drift on first use (covers initial spawn case)
    if (m_driftDirX == 0.f || m_driftSpeedX <= 0.f)
    {
        BeginDrift(ship);
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

void MultishotBarrageBehavior::BeginAim(BaseShip &ship)
{
    // Snapshot target angle toward player
    if (auto player = ShipManager::Instance().GetPlayer())
    {
        const sf::Vector2f d = player->GetPosition() - ship.GetPosition();
        m_targetAngleRad = std::atan2(d.y, d.x); // y-down coords
    }

    else
    {
        m_targetAngleRad = PI * 0.5f; // downward
    }

    // Pick a small turn direction (+/-) for the second volley
    m_turnDir = (RandRange(0.f, 1.f) < 0.5f) ? -1.f : 1.f;

    // Start from current aim
    m_aimAngleRad = m_targetAngleRad;
    SetSpriteRotation(ship, m_aimAngleRad);
}

void MultishotBarrageBehavior::DoAim(BaseShip &ship, float dt)
{
    // Smoothly rotate toward target (in case player moved during windup)
    const float maxStepRad = DegToRad(ROTATE_SPEED_DEG_PER_SEC) * dt;

    // Recompute target each frame (feels snappier), then approach
    if (auto player = ShipManager::Instance().GetPlayer())
    {
        const sf::Vector2f d = player->GetPosition() - ship.GetPosition();
        m_targetAngleRad = std::atan2(d.y, d.x);
    }

    m_aimAngleRad = ApproachAngle(m_aimAngleRad, m_targetAngleRad, maxStepRad);
    SetSpriteRotation(ship, m_aimAngleRad);
}

void MultishotBarrageBehavior::BeginBarrage()
{
    m_volleysFired = 0;
    m_betweenVolleyTimer = 0.f; // fire immediately
}

void MultishotBarrageBehavior::DoBarrage(BaseShip &ship, float dt)
{
    m_betweenVolleyTimer -= dt;

    if (m_volleysFired < VOLLEYS_PER_BARRAGE && m_betweenVolleyTimer <= 0.f)
    {
        // volley N
        FireVolley(ship, m_aimAngleRad);
        m_volleysFired++;

        if (m_volleysFired < VOLLEYS_PER_BARRAGE)
        {
            // Prep slight turn for next volley
            m_aimAngleRad += DegToRad(TURN_STEP_ANGLE_DEG) * m_turnDir;
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

void MultishotBarrageBehavior::FireVolley(BaseShip &ship, float baseAngleRad)
{
    if (auto *cfg = dynamic_cast<ConfigurableGun *>(ship.GetGun()))
    {
        // Keep owner in sync for muzzle computations.
        cfg->SetOwnerPosition(ship.GetPosition());

        // Build evenly distributed degrees in [-half, +half]
        static constexpr int PELLETS_PER_VOLLEY = 5; // or use your tunable
        static constexpr float SPREAD_HALF_ANGLE_DEG = 12.f;

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
                const float t = static_cast<float>(i) / static_cast<float>(n - 1); // 0..1
                const float deg = -SPREAD_HALF_ANGLE_DEG + t * span;               // -half..+half
                anglesDeg.push_back(deg);
            }
        }

        // Aim direction from current aim angle
        const sf::Vector2f baseDir{std::cos(baseAngleRad), std::sin(baseAngleRad)};
        const sf::Vector2f pos = ship.GetPosition();

        // Fire immediately; behavior handles cadence (cooldown is behavior-level here)
        cfg->FireSpread(pos, baseDir, anglesDeg);

        return;
    }
}

void MultishotBarrageBehavior::SetSpriteRotation(BaseShip &ship, float angleRad)
{
    const float deg = RadToDeg(angleRad) + 90.f;
    ship.SetRotation(deg);
}

float MultishotBarrageBehavior::WrapAngleRad(float a)
{
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

float MultishotBarrageBehavior::ApproachAngle(float current, float target, float maxStepRad)
{
    current = WrapAngleRad(current);
    target = WrapAngleRad(target);
    float diff = WrapAngleRad(target - current);

    if (std::fabs(diff) <= maxStepRad)
    {
        return target;
    }

    return current + ((diff > 0.f) ? maxStepRad : -maxStepRad);
}
