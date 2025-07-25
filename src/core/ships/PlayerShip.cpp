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
#include "ProjectileManager.h"
#include "ResolutionScaleManager.h"
#include "WindowManager.h"
#include <cmath>

PlayerShip::PlayerShip()
{
    m_allegiance = Allegiance::Player;
    m_sprite.setTexture(*AssetManager::Instance().GetTexture(SpriteAssets::PlayerAssets::PlayerShipWhiteKey));
    m_sprite.setScale(1.0f, 1.0f);

    SetHealth(100);

    m_speed = {m_baseSpeed * ResolutionScaleManager::Instance().GetScaleX(),
               m_baseSpeed * ResolutionScaleManager::Instance().GetScaleY()};

    // Initialize gun
    m_gun = std::make_shared<BasicGun>(1.f, m_allegiance);
}

void PlayerShip::Update(float dt)
{
    ProcessInput(dt);
    HandleGunUpdate(dt);
}

void PlayerShip::ApplyDifficultyScaling()
{
    // Intentionally no-op for PlayerShip
}

void PlayerShip::Move(const sf::Vector2f &offset)
{
    auto newPos = GetPosition() + offset;
    auto bounds = m_sprite.getGlobalBounds();
    auto winSize = WindowManager::Instance().GetWindow().getSize();

    newPos.x = std::max(0.f, std::min(newPos.x, winSize.x - bounds.width));
    newPos.y = std::max(0.f, std::min(newPos.y, winSize.y - bounds.height));

    SetPosition(newPos);
}

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

void PlayerShip::ProcessInput(const float dt)
{
    if (!IsAlive())
        return;

    sf::Vector2f dir{0.f, 0.f};

    if (InputManager::Instance().IsKeyPressed(KeyBindings::MoveUpConstantKey))
        dir.y -= 1.f;
    if (InputManager::Instance().IsKeyPressed(KeyBindings::MoveDownConstantKey))
        dir.y += 1.f;
    if (InputManager::Instance().IsKeyPressed(KeyBindings::MoveLeftConstantKey))
        dir.x -= 1.f;
    if (InputManager::Instance().IsKeyPressed(KeyBindings::MoveRightConstantKey))
        dir.x += 1.f;

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
        TryFire(); // gun cooldown ensures safety
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

float PlayerShip::GetGas() const
{
    return m_gas;
}

void PlayerShip::ReplenishGas(float amount)
{
    m_gas = std::min(m_maxGas, m_gas + amount);
}

void PlayerShip::HandleGunUpdate(float dt)
{
    if (m_gun)
    {
        m_gun->SetOwnerPosition(m_sprite.getPosition());
        m_gun->Update(dt);
    }
}
