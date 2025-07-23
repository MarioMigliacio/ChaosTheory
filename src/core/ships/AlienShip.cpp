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
#include "BasicGun.h"
#include "Macros.h"
#include "ProjectileManager.h"
#include "ResolutionScaleManager.h"
#include "SettingsManager.h"
#include "ShipPresets.h"
#include "WindowManager.h"
#include <random>

/// @brief Constants that can be adjusted throughout the AlienShip.
namespace
{
/// @brief Base Health before scaling.
constexpr int BASE_HEALTH = 125;

/// @brief Base Speed before scaling.
constexpr float BASE_SPEED = 150.f;

/// @brief Configurable time for ship to randomly try changing x-direction.
constexpr float DIRECTION_CHANGE_INTERVAL = 1.0f;
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
        ApplyDifficultyScaling();
    }

    else
    {
        CT_LOG_ERROR("AlienShip texture not found.");
    }

    m_gun = std::make_shared<BasicGun>(1.f, Allegiance::Enemy); // cooldown, allegiance
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

/// @brief Scales this AlienShip accordingly based on WindowResolution, and GameDifficulty.
void AlienShip::ApplyDifficultyScaling()
{
    const auto difficulty = SettingsManager::Instance().GetSettings()->m_gameDifficulty;
    const float scaleY = ResolutionScaleManager::Instance().GetScaleY();

    switch (difficulty)
    {
        case GameDifficultySetting::Easy:
            m_health = static_cast<int>(BASE_HEALTH * EASY_HEALTH_SCALE);
            m_speed = BASE_SPEED * EASY_SPEED_SCALE * scaleY;
            m_sprite.setColor(EASY_TINT);
            break;

        case GameDifficultySetting::Normal:
            m_health = BASE_HEALTH * NORMAL_HEALTH_SCALE;
            m_speed = BASE_SPEED * NORMAL_SPEED_SCALE * scaleY;
            m_sprite.setColor(NORMAL_TINT);
            break;

        case GameDifficultySetting::Hard:
            m_health = static_cast<int>(BASE_HEALTH * HARD_HEALTH_SCALE);
            m_speed = BASE_SPEED * HARD_SPEED_SCALE * scaleY;
            m_sprite.setColor(HARD_TINT);
            break;
    }
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
    pos.x += m_currentDirection * m_speed * 0.5f * dt;
    pos.y += (m_speed / 4.f) * dt; // prefer ship to travel slowly in y-direction.
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

    m_fireCooldown -= dt;

    if (m_fireCooldown <= 0.f)
    {
        auto proj = m_gun->TryFire();

        if (proj)
        {
            m_fireCooldown = m_fireDelayDist(m_rng);
        }
    }
}
