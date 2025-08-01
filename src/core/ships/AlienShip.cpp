// ============================================================================
//  File        : AlienShip.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-18
//  Description : A fast-moving alien ship that zig-zags across the screen.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "AlienShip.h"
#include "AssetManager.h"
#include "Assets.h"
#include "EnemyGun.h"
#include "Macros.h"
#include "ProjectileManager.h"
#include "ResolutionScaleManager.h"
#include "SettingsManager.h"
#include "WindowManager.h"
#include <random>

/// @brief Constants that can be adjusted throughout the AlienShip.
namespace
{
/// @brief Base Health before scaling.
constexpr int ALIEN_BASE_HEALTH = 30;

/// @brief Base Speed before scaling.
constexpr float ALIEN_BASE_SPEED = 100.f;

/// @brief Configurable time for ship to randomly try changing x-direction.
constexpr float DIRECTION_CHANGE_INTERVAL = 1.0f;

/// @brief Divide the y-direction speed of this spaceship to slow its descent.
constexpr float INTERNAL_Y_DIRECTION_DAMPENER = 4.f;

/// @brief Divide the x-direction speed of this spaceship to slow its strafing.
constexpr float INTERNAL_X_DIRECTION_DAMPENER = .5f;

/// @brief Configurable constant for Gun FireRate.
constexpr float ALIEN_PROJECTILE_FIRERATE = 1.5f;

/// @brief Configurable constant for Gun Speed.
constexpr float ALIEN_PROJECTILE_SPEED = 150.f;

/// @brief Configurable constant for Gun Damage.
constexpr float ALIEN_PROJECTILE_DAMAGE = 10.f;

/// @brief Configurable constant for Gun Projectile color.
const sf::Color ALIEN_PROJECTILE_COLOR = sf::Color::Red;

/// @brief Configurable constant for Gun Shots per fire.
constexpr int ALIEN_PROJECTILE_SHOTS_PER_FIRE = 1;
} // namespace

/// @brief Constructor for an AlienShip type of ship.
/// @param startPos Position to emplace at.
/// @param allegiance Allegiance to employ with.
AlienShip::AlienShip(const sf::Vector2f &startPos, Allegiance allegiance) : m_rng(std::random_device{}())
{
    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::EnemyAssets::AlienShipSpriteKey);

    if (tex)
    {
        m_sprite.setTexture(*tex);
        m_sprite.setPosition(startPos);
        m_sprite.setOrigin(tex->getSize().x / 2.f, tex->getSize().y / 2.f);
        m_allegiance = allegiance;

        m_currentDirection = m_directionDist(m_rng) == 0 ? -1.f : 1.f;

        m_health = ALIEN_BASE_HEALTH;
        m_speed = {ALIEN_BASE_SPEED, ALIEN_BASE_SPEED};

        ApplyDifficultyScaling(m_health, m_speed, m_sprite);
    }

    else
    {
        CT_LOG_ERROR("AlienShip texture not found.");
    }

    InitializeGunStats();
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update frame.
void AlienShip::Update(float dt)
{
    if (!m_alive)
    {
        return;
    }

    UpdateMovementLogic(dt);
    UpdateGunLogic(dt);
}

/// @brief Helper method to move this AlienShip based on update time.
/// @param dt delta time since last update frame.
void AlienShip::UpdateMovementLogic(const float dt)
{
    auto pos = m_sprite.getPosition();

    // Handle random horizontal movement direction switch
    m_directionChangeCooldown -= dt;

    if (m_directionChangeCooldown <= 0.f)
    {
        m_directionChangeCooldown = DIRECTION_CHANGE_INTERVAL;
        m_currentDirection = m_directionDist(m_rng) == 0 ? -1.f : 1.f;
    }

    // Move horizontally and vertically
    pos.x += m_currentDirection * m_speed.x * INTERNAL_X_DIRECTION_DAMPENER * dt;
    pos.y += (m_speed.y / INTERNAL_Y_DIRECTION_DAMPENER) * dt; // prefer ship to travel slowly in y-direction.
    m_sprite.setPosition(pos);

    // Keep within screen bounds horizontally
    const auto windowSize = WindowManager::Instance().GetWindow().getSize();
    const float leftEdge = m_sprite.getGlobalBounds().left;
    const float rightEdge = leftEdge + m_sprite.getGlobalBounds().width;

    if (leftEdge < 0.f)
    {
        m_sprite.move(-leftEdge, 0.f);
    }

    if (rightEdge > windowSize.x)
    {
        m_sprite.move(windowSize.x - rightEdge, 0.f);
    }

    // Destroy when off bottom
    if (pos.y - m_sprite.getGlobalBounds().height / 2.f > static_cast<float>(windowSize.y))
    {
        m_alive = false;
        CT_LOG_DEBUG("AlienShip: Destroyed after exiting screen.");
    }
}

/// @brief Helper method to update this AlienShip based on update time.
/// @param dt delta time since last update frame.
void AlienShip::UpdateGunLogic(const float dt)
{
    if (!m_gun)
    {
        return;
    }

    m_gun->SetOwnerPosition(GetPosition());
    m_gun->Update(dt);

    if (m_initialHoldTime > 0.f)
    {
        m_initialHoldTime -= dt;

        return;
    }

    // Simply call TryFire each frame, EnemyGun controls its fire rate
    auto proj = m_gun->TryFire();

    // Optional: boss phase logic or random fire rate adjustment
    if (proj)
    {
        // Example: during phase, speed up fire rate
        ProjectileManager::Instance().AddProjectile(proj);
    }
}

/// @brief Initialize gun stats
void AlienShip::InitializeGunStats()
{
    m_gunStats.fireRate = ALIEN_PROJECTILE_FIRERATE;
    m_gunStats.damage = ALIEN_PROJECTILE_DAMAGE;
    m_gunStats.speed = ALIEN_PROJECTILE_SPEED;
    m_gunStats.projectilesPerShot = ALIEN_PROJECTILE_SHOTS_PER_FIRE;
    m_gunStats.tint = ALIEN_PROJECTILE_COLOR;
    m_gunStats.homing = false;
    m_gunStats.piercing = false;

    m_gun = std::make_unique<EnemyGun>(m_gunStats);
    m_gun->SetAllegiance(Allegiance::Enemy);
}
