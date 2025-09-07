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
#include "ShipStatsScaling.h"
#include "WindowManager.h"
#include <random>

/// @brief Constants that can be adjusted throughout the AlienShip.
namespace
{
/// @brief Base Health before scaling.
constexpr int ALIEN_BASE_HEALTH = 30;

/// @brief Base Speed before scaling.
const sf::Vector2f ALIEN_BASE_SPEED = {100.f, 100.f};

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

        m_health = m_maxHealth = ShipStatsScaling::ScaleHealthToDifficulty(ALIEN_BASE_HEALTH);
        m_speed = ShipStatsScaling::ScaleSpeedToDifficulty(ALIEN_BASE_SPEED);
        ShipStatsScaling::SetSpriteColorFromDifficulty(m_sprite);
        InitializeGunStats();
    }

    else
    {
        CT_LOG_ERROR("AlienShip texture not found.");
    }
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update frame.
void AlienShip::Update(float dt)
{
    if (!m_alive)
    {
        return;
    }

    UpdateBehavior(*this, dt);
    CullIfOffscreen();
}

/// @brief Initialize gun stats
void AlienShip::InitializeGunStats()
{
    m_gunStats.fireRate = ALIEN_PROJECTILE_FIRERATE;
    m_gunStats.damage = ALIEN_PROJECTILE_DAMAGE;
    m_gunStats.speed = ALIEN_PROJECTILE_SPEED;
    m_gunStats.tint = ALIEN_PROJECTILE_COLOR;
    m_gunStats.homing = false;

    m_gun = std::make_unique<EnemyGun>(m_gunStats);

    sf::Vector2f spriteSize(static_cast<float>(m_sprite.getTexture()->getSize().x),
                            static_cast<float>(m_sprite.getTexture()->getSize().y));

    m_gun->SetAllegiance(Allegiance::Enemy, spriteSize);
}
