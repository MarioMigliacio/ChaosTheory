// ============================================================================
//  File        : GruntShip.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-05
//  Description : A small unit that hops locally and fires aimed shots.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "GruntShip.h"
#include "AssetManager.h"
#include "Assets.h"
#include "DodgeAndAimBehavior.h"
#include "EnemyGun.h"
#include "Macros.h"
#include "ShipStatsScaling.h"
#include "WindowManager.h"

/// @brief Constants that can be adjusted throughout the GruntShip.
namespace
{
/// @brief Base Health before scaling.
constexpr int GRUNT_BASE_HEALTH = 20;

/// @brief Base Speed before scaling.
const sf::Vector2f GRUNT_BASE_SPEED = {0.f, 110.f};

/// @brief Configurable constant for Gun FireRate.
constexpr float GRUNT_BASE_FIRERATE = 1.0f;

/// @brief Configurable constant for Gun Damage.
constexpr float GRUNT_BULLET_DAMAGE = 9.f;

/// @brief Configurable constant for Gun Speed.
constexpr float GRUNT_BULLET_SPEED = 350.f;

/// @brief Configurable constant for Gun Projectile color.
const sf::Color GRUNT_BULLET_COLOR = sf::Color::Red;
} // namespace

/// @brief Constructor for a GruntShip type of ship.
/// @param startPos Position to emplace at.
/// @param allegiance Allegiance to employ with.
GruntShip::GruntShip(const sf::Vector2f &startPos, Allegiance allegiance)
{
    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::EnemyAssets::GruntShipSpriteKey);

    if (!tex)
    {
        CT_LOG_ERROR("GruntShip texture not found.");

        return;
    }

    m_sprite.setTexture(*tex);
    m_sprite.setPosition(startPos);
    m_sprite.setOrigin(tex->getSize().x * 0.5f, tex->getSize().y * 0.5f);
    m_allegiance = allegiance;

    m_health = m_maxHealth = ShipStatsScaling::ScaleHealthToDifficulty(GRUNT_BASE_HEALTH);
    m_speed = ShipStatsScaling::ScaleSpeedToDifficulty(GRUNT_BASE_SPEED);
    ShipStatsScaling::SetSpriteColorFromDifficulty(m_sprite);

    InitializeGunStats();
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update frame.
void GruntShip::Update(float dt)
{
    if (!m_alive)
    {
        return;
    }

    UpdateBehavior(*this, dt);
    CullIfOffscreen();
}

/// @brief Initialize gun stats
void GruntShip::InitializeGunStats()
{
    m_gunStats.fireRate = GRUNT_BASE_FIRERATE;
    m_gunStats.damage = GRUNT_BULLET_DAMAGE;
    m_gunStats.speed = GRUNT_BULLET_SPEED;
    m_gunStats.tint = GRUNT_BULLET_COLOR;
    m_gunStats.homing = false;

    m_gun = std::make_unique<EnemyGun>(m_gunStats);

    const sf::Vector2f spriteSize(static_cast<float>(m_sprite.getTexture()->getSize().x),
                                  static_cast<float>(m_sprite.getTexture()->getSize().y));

    m_gun->SetAllegiance(Allegiance::Enemy, spriteSize);
}
