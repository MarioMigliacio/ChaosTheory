// ============================================================================
//  File        : CrusaderShip.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-24
//  Description : A telegraphed laser attacker. Aligns to player's X, shakes,
//                glows red, then fires a lethal beam stream.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "CrusaderShip.h"
#include "AssetManager.h"
#include "Assets.h"
#include "EnemyGun.h"
#include "Macros.h"
#include "ShipStatsScaling.h"
#include "WindowManager.h"

/// @brief Constants that can be adjusted throughout the CrusaderShip.
namespace
{
/// @brief Base health for the CrusaderShip.
constexpr int CRUSADER_BASE_HEALTH = 45;

/// @brief Beam defaults, damage IS important for ship.
constexpr float CRUSADER_BEAM_DAMAGE = 20.f;

/// @brief Beam defaults, irrelevant once behavior upgrades.
constexpr float CRUSADER_BEAM_FIRERATE = 2.0f;

/// @brief Beam defaults, irrelevant once behavior upgrades.
constexpr float CRUSADER_BEAM_SPEED = 800.f;

/// @brief Configurable general drift magnitude.
const sf::Vector2f CRUSADER_BASE_SPEED = {0.f, 120.f};

/// @brief Configurable color for laser beam.
const sf::Color CRUSADER_BEAM_COLOR = sf::Color(255, 80, 80);
} // namespace

/// @brief Constructs a CrusaderShip
/// @param startPos Starting position.
/// @param allegiance Allegiance.
CrusaderShip::CrusaderShip(const sf::Vector2f &startPos, Allegiance allegiance)
{
    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::EnemyAssets::CrusaderShipSpriteKey);

    if (!tex)
    {
        CT_LOG_ERROR("CrusaderShip texture not found.");

        return;
    }

    m_sprite.setTexture(*tex);
    m_sprite.setPosition(startPos);
    m_sprite.setOrigin(tex->getSize().x * 0.5f, tex->getSize().y * 0.5f);
    m_allegiance = allegiance;

    m_health = m_maxHealth = ShipStatsScaling::ScaleHealthToDifficulty(CRUSADER_BASE_HEALTH);
    m_speed = ShipStatsScaling::ScaleSpeedToDifficulty(CRUSADER_BASE_SPEED);
    ShipStatsScaling::SetSpriteColorFromDifficulty(m_sprite);

    InitializeGunStats();
}

/// @brief Performs routine update during a frame.
/// @param dt Delta time since last update.
void CrusaderShip::Update(float dt)
{
    if (!m_alive)
    {
        return;
    }

    UpdateBehavior(*this, dt);
    CullIfOffscreen();
}

/// @brief Initializes the CrusaderShip with gun statistics.
void CrusaderShip::InitializeGunStats()
{
    // Base gun for beam; behavior will swap to laser stream and crank stats at fire time.
    m_gunStats.fireRate = CRUSADER_BEAM_FIRERATE;
    m_gunStats.damage = CRUSADER_BEAM_DAMAGE;
    m_gunStats.speed = CRUSADER_BEAM_SPEED;
    m_gunStats.tint = CRUSADER_BEAM_COLOR;
    m_gunStats.homing = false;

    m_gun = std::make_unique<EnemyGun>(m_gunStats);

    const sf::Vector2f spriteSize(static_cast<float>(m_sprite.getTexture()->getSize().x),
                                  static_cast<float>(m_sprite.getTexture()->getSize().y));
    m_gun->SetAllegiance(Allegiance::Enemy, spriteSize);
}
