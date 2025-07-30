// ============================================================================
//  File        : PlayerShip.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-25
//  Description : Represents the ChaosTheory Playable space ship.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "PlayerShip.h"
#include "AssetManager.h"
#include "Assets.h"
#include "BasicGun.h"
#include "InputManager.h"
#include "KeyBindings.h"
#include "Macros.h"
#include "ProjectileManager.h"
#include "ResolutionScaleManager.h"
#include "WindowManager.h"
#include <cmath>

/// @brief Constants that can be adjusted throughout the PlayerShip.
namespace
{
constexpr float INITIAL_GAS_MAX = 100.f;

constexpr int INITIAL_HEALTH_MAX = 100;

constexpr float GAS_DRAIN_RATE = 20.f;

constexpr float ACCEL_VELOCITY_MULTIPLIER = 2.f;

constexpr float BASE_SHIP_VELOCITY = 200.f;

constexpr float BASE_PLAYER_GUN_FIRE_COOLDOWN = .5f;
} // namespace

/// @brief Constructor for the PlayerShip.
PlayerShip::PlayerShip()
{
    m_allegiance = Allegiance::Player;
    m_sprite.setTexture(*AssetManager::Instance().GetTexture(SpriteAssets::PlayerAssets::PlayerShipWhiteKey));
    m_sprite.setScale(1.0f, 1.0f);

    m_health = m_maxHealth = INITIAL_HEALTH_MAX;
    m_gas = m_maxGas = INITIAL_GAS_MAX;

    m_gasDrainRate = GAS_DRAIN_RATE;
    m_accelerationMultiplier = ACCEL_VELOCITY_MULTIPLIER;
    m_baseSpeed = BASE_SHIP_VELOCITY;

    m_speed = {m_baseSpeed * ResolutionScaleManager::Instance().GetScaleX(),
               m_baseSpeed * ResolutionScaleManager::Instance().GetScaleY()};

    m_gun = std::make_shared<BasicGun>(BASE_PLAYER_GUN_FIRE_COOLDOWN, m_allegiance);

    CT_LOG_DEBUG("PlayerShip constructed.");
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update frame.
void PlayerShip::Update(float dt)
{
    ProcessInput(dt);
    HandleGunUpdate(dt);
}

/// @brief Performs a sprite position movement based on offset and current position.
/// @param offset Position to offset the current sprite.
void PlayerShip::Move(const sf::Vector2f &offset)
{
    auto newPos = GetPosition() + offset;
    auto bounds = m_sprite.getGlobalBounds();
    auto winSize = WindowManager::Instance().GetWindow().getSize();

    newPos.x = std::max(0.f, std::min(newPos.x, winSize.x - bounds.width));
    newPos.y = std::max(0.f, std::min(newPos.y, winSize.y - bounds.height));

    SetPosition(newPos);
}

/// @brief Call the attached gun to perform its TryFire logic. If null, return nullptr.
/// @return Safe pointer to a BaseProjectile entity.
std::shared_ptr<BaseProjectile> PlayerShip::TryFire()
{
    if (m_gun)
    {
        auto projectile = m_gun->TryFire();

        if (projectile)
        {
            // We might logically reset some internal timers here in the future.
            return projectile;
        }
    }

    return nullptr;
}

/// @brief The primary input interaction between PlayerShip and keyboard input.
/// @param dt delta time since last update.
void PlayerShip::ProcessInput(const float dt)
{
    if (!IsAlive())
    {
        return;
    }

    sf::Vector2f dir{0.f, 0.f};

    if (InputManager::Instance().IsKeyPressed(KeyBindings::MoveUpConstantKey))
    {
        dir.y -= 1.f;
    }

    if (InputManager::Instance().IsKeyPressed(KeyBindings::MoveDownConstantKey))
    {
        dir.y += 1.f;
    }

    if (InputManager::Instance().IsKeyPressed(KeyBindings::MoveLeftConstantKey))
    {
        dir.x -= 1.f;
    }

    if (InputManager::Instance().IsKeyPressed(KeyBindings::MoveRightConstantKey))
    {
        dir.x += 1.f;
    }

    // Normalize movement vector (diagonal fix)
    if (dir.x != 0.f || dir.y != 0.f)
    {
        float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        dir /= length;

        sf::Vector2f delta{dir.x * m_speed.x * dt, dir.y * m_speed.y * dt};
        Move(delta);
    }

    // Fire gun button (cooldown handled in gun)
    if (InputManager::Instance().IsKeyPressed(KeyBindings::FireGunConstantKey))
    {
        TryFire();
    }

    // Fire bomb button
    if (InputManager::Instance().IsKeyPressed(KeyBindings::LaunchBombConstantKey))
    {
        // TODDO: attempt to launch a bomb if available.
    }

    // Accelerate button
    if (InputManager::Instance().IsKeyPressed(KeyBindings::AccelerateConstantKey) && m_gas > 0.f)
    {
        // Only allow gas drain if there is movement input
        if (dir.x != 0.f || dir.y != 0.f)
        {
            m_speed.x = m_baseSpeed * m_accelerationMultiplier * ResolutionScaleManager::Instance().GetScaleX();
            m_speed.y = m_baseSpeed * m_accelerationMultiplier * ResolutionScaleManager::Instance().GetScaleY();

            float drainAmount = m_gasDrainRate * dt;
            m_gas = std::max(0.f, m_gas - drainAmount);
        }
    }

    else
    {
        // use normal speed, no gas drain
        m_speed.x = m_baseSpeed * ResolutionScaleManager::Instance().GetScaleX();
        m_speed.y = m_baseSpeed * ResolutionScaleManager::Instance().GetScaleY();
    }
}

/// @brief Returns the current tracked Gas amount for this PlayerShip.
/// @return m_gas.
float PlayerShip::GetGas() const
{
    return m_gas;
}

/// @brief Replenish the PlayerShip's current Gas, used for acceleration.
/// @param amount Float gas to replenish.
void PlayerShip::ReplenishGas(float amount)
{
    m_gas = std::min(m_maxGas, m_gas + amount);
}

/// @brief Request the Gun to update for this PlayerShip.
/// @param dt delta time since last update.
void PlayerShip::HandleGunUpdate(float dt)
{
    if (m_gun)
    {
        m_gun->SetOwnerPosition(m_sprite.getPosition());
        m_gun->Update(dt);
    }
}

/// @brief No-Op for player, this is an interface override generally meant for every enemy ship.
void PlayerShip::ApplyDifficultyScaling()
{
}
