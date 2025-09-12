// ============================================================================
//  File        : CrusaderBehavior.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-24
//  Description : Lines up to player's X, telegraphs (shake + red glow),
//                then fires a lethal laser beam stream forward.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "CrusaderBehavior.h"
#include "BaseShip.h"
#include "ConfigurableGun.h"
#include "GunPattern.h"
#include "ProjectileManager.h"
#include "ShipManager.h"
#include "WindowManager.h"
#include <cmath>

/// @brief Constants that can be adjusted throughout the CrusaderBehavior.
namespace
{
/// @brief Horizontal speed for aligning to player X.
constexpr float ALIGN_SPEED_X = 220.f;

/// @brief Constant downward drift while in Align phase.
constexpr float DRIFT_SPEED_Y = 60.f;

/// @brief Pixel tolerance in X for considering the ship aligned.
constexpr float ALIGN_TOLERANCE_X = 8.f;

/// @brief Telegraph duration before firing.
constexpr float TELEGRAPH_TIME = 1.0f;

/// @brief Horizontal shake amplitude during telegraph.
constexpr float SHAKE_AMPLITUDE_X = 16.f;

/// @brief Shake frequency.
constexpr float SHAKE_FREQ = 25.f;

/// @brief Beam firing window duration.
constexpr float FIRE_TIME = 1.5f;

/// @brief Minimum vertical offset (pixels) the player must be below before Crusader will telegraph/fire.
constexpr float MIN_DIST_TO_FIRE = 24.f;

/// @brief Cooldown after firing before Crusader can re-arm.
constexpr float REARM_COOLDOWN = 5.0f;

/// @brief Color for telegraph of the angry ship arming its laser.
const sf::Color ARMING_LASER_COLOR = sf::Color(255, 80, 80);

/// @brief Color for reverting back to normal after arming laser.
const sf::Color NEUTRAL_COLOR = sf::Color::White;

/// @brief Return direction of positive or negative based on x input.
/// @param x Player coordinate location.
/// @return Negative direction if player is to our left, right otherwise, or aligned == 0.
inline float Sign(float x)
{
    return (x < 0.f) ? -1.f : (x > 0.f ? 1.f : 0.f);
}

/// @brief Returns the x-distance to the ship from us.
/// @param ship Reference to the ship to compare against.
/// @return float distance from the target ship.
inline float DxToPlayer(const BaseShip &ship)
{
    auto player = ShipManager::Instance().GetPlayer();

    if (!player)
    {
        return 0.f;
    }

    return player->GetPosition().x - ship.GetPosition().x;
}

/// @brief Returns the y-distance to the ship from us.
/// @param ship Reference to the ship to compare against.
/// @return float distance from the target ship.
inline float DyToPlayer(const BaseShip &ship)
{
    auto player = ShipManager::Instance().GetPlayer();

    if (!player)
    {
        return 0.f;
    }

    return player->GetPosition().y - ship.GetPosition().y;
}

} // namespace

/// @brief Performs routine updates during a frame.
/// @param ship The ship to perform the updpate on.
/// @param dt Delta time since last update.
void CrusaderBehavior::UpdateMovementLogic(BaseShip &ship, float dt)
{
    switch (m_phase)
    {
        case Phase::Align:
        {
            AlignToPlayerX(ship, dt);

            // only proceed if player is below min dist theshold.
            const float dx = std::fabs(DxToPlayer(ship));
            const float dy = DyToPlayer(ship);
            const bool alignedX = (dx <= ALIGN_TOLERANCE_X);
            const bool playerBelow = (dy >= MIN_DIST_TO_FIRE);

            if (alignedX && playerBelow)
            {
                m_phase = Phase::Telegraph;
                m_phaseTimer = TELEGRAPH_TIME;
                m_glowApplied = false;
            }
            break;
        }
        case Phase::Telegraph:
            DoTelegraph(ship, dt);
            break;
        case Phase::Fire:
            // slow ominous movement for effect.
            ship.Move({0.f, DRIFT_SPEED_Y * 0.25f * dt});
            break;
        case Phase::Cooldown:
            ship.Move({0.f, DRIFT_SPEED_Y * dt});
            break;
    }
}

/// @brief Performs routine gun logic during an update frame.
/// @param ship Reference to the Ship to perform update for gun.
/// @param dt Delta time since last update.
void CrusaderBehavior::UpdateGunLogic(BaseShip &ship, float dt)
{
    switch (m_phase)
    {
        case Phase::Align:
            break;

        case Phase::Telegraph:
            break;

        case Phase::Fire:
            DoFire(ship, dt);
            break;

        case Phase::Cooldown:
            m_phaseTimer -= dt;

            if (m_phaseTimer <= 0.f)
            {
                m_phase = Phase::Align;
                m_firingPrimed = false;
            }
            break;
    }
}

/// @brief Helper method to align Ship to player X coordinate location.
/// @param ship Reference to ship to compare.
/// @param dt Delta time since last update frame.
void CrusaderBehavior::AlignToPlayerX(BaseShip &ship, float dt)
{
    ship.Move({0.f, DRIFT_SPEED_Y * dt});

    auto player = ShipManager::Instance().GetPlayer();

    if (!player)
    {
        return;
    }

    const float dx = DxToPlayer(ship);

    if (std::fabs(dx) > ALIGN_TOLERANCE_X)
    {
        const float moveX = Sign(dx) * ALIGN_SPEED_X * dt;
        ship.Move({moveX, 0.f});
    }
}

/// @brief Helper method to perform animation and color choreograph before firing.
/// @param ship Reference to ship to perform action on.
/// @param dt Delta time since last update.
void CrusaderBehavior::DoTelegraph(BaseShip &ship, float dt)
{
    // one-time red tint
    if (!m_glowApplied)
    {
        ship.SetTint(ARMING_LASER_COLOR);
        m_glowApplied = true;
    }

    // Capture anchor X once at the start of the telegraph window
    if (!m_anchorCaptured)
    {
        m_shakeAnchorX = ship.GetPosition().x;
        m_anchorCaptured = true;
    }

    m_phaseTimer -= dt;

    const float elapsed = TELEGRAPH_TIME - std::max(0.f, m_phaseTimer);
    const float currentX = ship.GetPosition().x;

    const float shake = 2.f * PI * SHAKE_FREQ;
    const float targetX = m_shakeAnchorX + std::sin(elapsed * shake) * SHAKE_AMPLITUDE_X;
    const float dx = targetX - currentX;

    // move direction for random oscillations
    ship.Move({dx, 0.f});

    // tiny hover in y direction
    ship.Move({0.f, DRIFT_SPEED_Y * 0.25f * dt});

    if (m_phaseTimer <= 0.f)
    {
        m_anchorCaptured = false;
        m_phase = Phase::Fire;
        m_phaseTimer = FIRE_TIME;
        m_firingPrimed = false;
    }
}

/// @brief Helper method to prime the ships gun for the firing phase.
/// @param ship The ship which will perform the action.
void CrusaderBehavior::BeginFire(BaseShip &ship)
{
    auto *gun = dynamic_cast<ConfigurableGun *>(ship.GetGun());

    if (!gun)
    {
        return;
    }

    // Crank up the stats so the stream feels instant and fatal.
    gun->SetPattern(GunPattern::LaserBeam);
    gun->UpgradeFireRate(.05f);
    gun->UpgradeVelocity(1000.f);

    m_firingPrimed = true;
}

/// @brief Perform the actions for Firing a primed laser stream.
/// @param ship The ship which will perform the action.
/// @param dt Delta time since last update.
void CrusaderBehavior::DoFire(BaseShip &ship, float dt)
{
    if (!m_firingPrimed)
    {
        BeginFire(ship);
    }

    auto *gun = ship.GetGun();

    if (gun)
    {
        gun->SetOwnerPosition(ship.GetPosition());
        gun->Update(dt);

        if (auto proj = gun->TryFire())
        {
            ProjectileManager::Instance().AddProjectile(proj);
        }
    }

    m_phaseTimer -= dt;

    if (m_phaseTimer <= 0.f)
    {
        ship.SetTint(NEUTRAL_COLOR);
        m_phase = Phase::Cooldown;
        m_phaseTimer = REARM_COOLDOWN;
        m_firingPrimed = false;
        m_anchorCaptured = false;
    }
}
