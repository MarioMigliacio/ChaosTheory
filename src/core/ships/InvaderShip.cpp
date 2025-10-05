// ============================================================================
//  File        : InvaderShip.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-07
//  Description : Implementation for InvaderShip.
// ============================================================================

#include "InvaderShip.h"
#include "AssetManager.h"
#include "Assets.h"
#include "EnemyGun.h"
#include "Macros.h"
#include "MultishotBarrageBehavior.h"
#include "ShipStatsScaling.h"
#include "WindowManager.h"

/// @brief Constants that can be adjusted throughout the InvaderShip.
namespace
{
/// @brief Base Health before scaling.
constexpr int INVADER_BASE_HEALTH = 28;

/// @brief Base Speed before scaling.
const sf::Vector2f INVADER_BASE_SPEED = {0.f, 110.f};

/// @brief Configurable constant for Gun FireRate.
constexpr float INVADER_BASE_FIRERATE = 1.0f;

/// @brief Configurable constant for Gun Damage.
constexpr float INVADER_PELLET_DAMAGE = 10.f;

/// @brief Configurable constant for Gun projectile velocity.
constexpr float INVADER_PELLET_SPEED = 400.f;

/// @brief Configurable constant for projectile color.
const sf::Color INVADER_BULLET_COLOR = sf::Color(220, 245, 255);
} // namespace

/// @brief Constructor for an InvaderShip type of ship.
/// @param startPos Position to emplace at.
/// @param allegiance Allegiance to employ with.
InvaderShip::InvaderShip(const sf::Vector2f &startPos, Allegiance allegiance)
{
    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::EnemyAssets::InvaderShipSpriteKey);

    m_type = ShipType::Invader;
    m_scoreValue = INVADER_SCORE_VALUE;

    if (!tex)
    {
        CT_LOG_ERROR("InvaderShip texture not found.");

        return;
    }

    m_sprite.setTexture(*tex);
    m_sprite.setPosition(startPos);
    m_sprite.setOrigin(tex->getSize().x * 0.5f, tex->getSize().y * 0.5f);
    m_allegiance = allegiance;

    m_health = m_maxHealth = ShipStatsScaling::ScaleHealthToDifficulty(INVADER_BASE_HEALTH);
    m_speed = ShipStatsScaling::ScaleSpeedToDifficulty(INVADER_BASE_SPEED);
    ShipStatsScaling::SetSpriteColorFromDifficulty(m_sprite);

    InitializeGunStats();
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update frame.
void InvaderShip::Update(float dt)
{
    if (!m_alive)
    {
        return;
    }

    UpdateBehavior(*this, dt);
    CullIfOffscreen();
}

/// @brief Initialize gun stats
void InvaderShip::InitializeGunStats()
{
    m_gunStats.fireRate = INVADER_BASE_FIRERATE;
    m_gunStats.damage = INVADER_PELLET_DAMAGE;
    m_gunStats.speed = INVADER_PELLET_SPEED;
    m_gunStats.tint = INVADER_BULLET_COLOR;
    m_gunStats.homing = false;

    m_gun = std::make_unique<EnemyGun>(m_gunStats);

    const sf::Vector2f spriteSize(static_cast<float>(m_sprite.getTexture()->getSize().x),
                                  static_cast<float>(m_sprite.getTexture()->getSize().y));

    m_gun->SetAllegiance(Allegiance::Enemy, spriteSize);
}
