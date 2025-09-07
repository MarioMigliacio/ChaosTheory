// ============================================================================
//  File        : BerserkerShip.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-19
//  Description : Fast kamikaze ship. Locks direction to player's position at
//                spawn and flies straight along that vector (no homing).
// ============================================================================

#include "BerserkerShip.h"
#include "AssetManager.h"
#include "Assets.h"
#include "Macros.h"
#include "ShipManager.h"
#include "ShipStatsScaling.h"
#include "WindowManager.h"

/// @brief Constants that can be adjusted throughout the BerserkerShip.
namespace
{
/// @brief Adjustable constant for the health of the basic BerserkerShip.
constexpr int BERSERKER_BASE_HEALTH = 20;

/// @brief Adjustable constant for the speed of the basic BerserkerShip.
const sf::Vector2f BERSERKER_BASE_SPEED = {0.f, 200.f};
} // namespace

/// @brief Constructor for a BerserkerShip type of ship.
/// @param startPos Position to emplace at.
/// @param allegiance Allegiance to employ with.
BerserkerShip::BerserkerShip(const sf::Vector2f &startPos, Allegiance allegiance)
{
    auto player = ShipManager::Instance().GetPlayer();

    if (player)
    {
        SetSpawnTargetSnapshot(player->GetPosition());

        CT_LOG_DEBUG("BerserkerShip locked on to player position ({}, {})", player->GetPosition().x,
                     player->GetPosition().y);
    }

    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::EnemyAssets::BerserkerShipSpriteKey);

    if (!tex)
    {
        CT_LOG_ERROR("BerserkerShip texture not found.");

        return;
    }

    m_sprite.setTexture(*tex);
    m_sprite.setPosition(startPos);
    m_sprite.setOrigin(tex->getSize().x / 2.f, tex->getSize().y / 2.f);
    m_allegiance = allegiance;

    m_health = m_maxHealth = ShipStatsScaling::ScaleHealthToDifficulty(BERSERKER_BASE_HEALTH);
    m_speed = ShipStatsScaling::ScaleSpeedToDifficulty(BERSERKER_BASE_SPEED);
    ShipStatsScaling::SetSpriteColorFromDifficulty(m_sprite);

    InitializeGunStats();
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update frame.
void BerserkerShip::Update(float dt)
{
    if (!m_alive)
    {
        return;
    }

    UpdateBehavior(*this, dt);
    CullIfOffscreen();
}

/// @brief Initialize gun stats. Not used for a BerserkerShip.
void BerserkerShip::InitializeGunStats()
{
    m_gunStats.damage = 0.f;
    m_gunStats.fireRate = 0.f;
    m_gunStats.homing = false;
    m_gunStats.speed = 0.f;
    m_gunStats.tint = sf::Color::White;
}
