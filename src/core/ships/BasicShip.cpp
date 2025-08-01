// ============================================================================
//  File        : BasicShip.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-17
//  Description : Basic ship type object logic.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "BasicShip.h"
#include "AssetManager.h"
#include "Assets.h"
#include "Macros.h"
#include "ResolutionScaleManager.h"
#include "SettingsManager.h"
#include "ShipPresets.h"
#include "WindowManager.h"

/// @brief Constants that can be adjusted throughout the BasicShip.
namespace
{
/// @brief Base Health before scaling.
constexpr int BASE_HEALTH = 50;

/// @brief Base Speed before scaling.
constexpr float BASE_SPEED = 75.f;
} // namespace

/// @brief Constructor for a BasicShip type of ship.
/// @param startPos Position to emplace at.
/// @param allegiance Allegiance to employ with.
BasicShip::BasicShip(const sf::Vector2f &startPos, Allegiance allegiance)
{
    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::EnemyAssets::BasicShipSpriteKey);

    if (tex)
    {
        m_sprite.setTexture(*tex);
        m_sprite.setPosition(startPos);
        m_sprite.setOrigin(tex->getSize().x / 2.f, tex->getSize().y / 2.f);
        m_allegiance = allegiance;

        m_health = BASE_HEALTH;
        m_speed = {0.f, BASE_SPEED};

        ApplyDifficultyScaling(m_health, m_speed, m_sprite);
    }

    else
    {
        CT_LOG_ERROR("BasicShip texture not found.");
    }
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update frame.
void BasicShip::Update(float dt)
{
    if (!m_alive)
    {
        return;
    }

    // Move downward
    sf::Vector2f pos = m_sprite.getPosition();
    pos.y += m_speed.y * dt;
    m_sprite.setPosition(pos);

    // Destroy when off-screen
    float windowHeight = static_cast<float>(WindowManager::Instance().GetWindow().getSize().y);

    if (pos.y - m_sprite.getGlobalBounds().height / 2.f > windowHeight)
    {
        m_alive = false;
        CT_LOG_DEBUG("BasicShip: Ship exited screen and was destroyed.");
    }
}

/// @brief Initialize gun stats. Not used for a BasicShip.
void BasicShip::InitializeGunStats()
{
    m_gunStats.damage = 0.f;
    m_gunStats.fireRate = 0.f;
    m_gunStats.homing = false;
    m_gunStats.piercing = false;
    m_gunStats.projectilesPerShot = 0;
    m_gunStats.speed = 0.f;
    m_gunStats.tint = sf::Color::White;
}
