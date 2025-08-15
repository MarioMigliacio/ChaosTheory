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
#include "BombProjectile.h"
#include "InputManager.h"
#include "KeyBindings.h"
#include "Macros.h"
#include "ProjectileManager.h"
#include "ResolutionScaleManager.h"
#include "UIIcon.h"
#include "UpgradableGun.h"
#include "WindowManager.h"
#include <cmath>

/// @brief Constants that can be adjusted throughout the PlayerShip.
namespace
{
/// @brief Configurable constant for player lives before game over.
constexpr int PLAYER_STARTING_LIVES = 3;

/// @brief Configurable constant for the ships base gas.
constexpr float INITIAL_GAS_MAX = 100.f;

/// @brief Configurable constant for the ships gas drain rate when accelerating.
constexpr float GAS_DRAIN_RATE = 20.f;

/// @brief Configurable constant for the ships base life.
constexpr int INITIAL_HEALTH_MAX = 100;

/// @brief Configurable constant for the ships base velocity.
constexpr float BASE_SHIP_VELOCITY = 200.f;

/// @brief Configurable constant for adjustment to velocity when accelerating.
constexpr float ACCEL_VELOCITY_MULTIPLIER = 2.f;

/// @brief Configurable constant for Gun FireRate.
constexpr float BASE_PLAYER_PROJECTILE_FIRERATE = .5f;

/// @brief Configurable constant for Gun Speed.
constexpr float BASE_PLAYER_PROJECTILE_SPEED = 250.f;

/// @brief Configurable constant for Gun Damage.
constexpr float BASE_PLAYER_PROJECTILE_DAMAGE = 10.f;

/// @brief Configurable constant for Gun Projectile color.
const sf::Color BASE_PLAYER_PROJECTILE_COLOR = sf::Color::White;

/// @brief Configurable time constant for respawn invincible period.
constexpr static float RESPAWN_INVINCIBILITY_DURATION = 2.f;

/// @brief Configurable time constant for fading in and out during respawn period.
constexpr static float BLINK_INTERVAL = 0.25f;

/// @brief Configurable constant for delta time required to be able to fire a 2nd bomb.
constexpr float BOMB_COOLDOWN_SEC = 1.f;

/// @brief Configurable constant for direction vector of initial bomb projectile.
const sf::Vector2f BOMB_VELOCITY = {0.f, -320};
} // namespace

/// @brief Constructor for the PlayerShip.
PlayerShip::PlayerShip()
{
    m_allegiance = Allegiance::Player;

    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerAssets::PlayerShipWhiteKey);

    if (tex)
    {
        m_sprite.setTexture(*tex);
        m_sprite.setOrigin(tex->getSize().x / 2.f, tex->getSize().y / 2.f);
        m_sprite.setScale(1.0f, 1.0f);

        InitializeGenericStats();
        InitializeGunStats();

        CT_LOG_DEBUG("PlayerShip constructed.");
    }

    else
    {
        CT_LOG_ERROR("PlayerShip: ERROR - texture not found.");
    }
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update frame.
void PlayerShip::Update(float dt)
{
    ProcessInput(dt);
    HandleGunUpdate(dt);
    HandleRespawnUpdate(dt);
}

/// @brief Performs a sprite position movement based on offset and current position.
/// @param offset Position to offset the current sprite.
void PlayerShip::Move(const sf::Vector2f &offset)
{
    auto newPos = GetPosition() + offset;
    auto bounds = m_sprite.getGlobalBounds();
    auto winSize = WindowManager::Instance().GetWindow().getSize();

    const float halfWidth = bounds.width / 2.f;
    const float halfHeight = bounds.height / 2.f;

    newPos.x = std::max(halfWidth, std::min(newPos.x, winSize.x - halfWidth));
    newPos.y = std::max(halfHeight, std::min(newPos.y, winSize.y - halfHeight));

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
            return projectile;
        }
    }

    return nullptr;
}

/// @brief Sync with the collision of a UIIcon and get a boost for the player.
/// @param icon UIIcon to check effect against.
void PlayerShip::ApplyIconEffect(const std::shared_ptr<UIIcon> &icon)
{
    if (!icon)
    {
        return;
    }

    switch (icon->GetEffectType())
    {
        case IconEffectType::GunDamageBoost:
            if (auto *gun = dynamic_cast<ConfigurableGun *>(GetGun()))
            {
                gun->UpgradeDamageByFlat(2);
            }
            break;
        case IconEffectType::GunFireRateBoost:
            if (auto *gun = dynamic_cast<ConfigurableGun *>(GetGun()))
            {
                gun->UpgradeFireRate(.90f);
            }
            break;
        case IconEffectType::GunVelocityBoost:
            if (auto *gun = dynamic_cast<ConfigurableGun *>(GetGun()))
            {
                gun->UpgradeVelocity(12.5f);
            }
            break;
        case IconEffectType::GunUpgradeBoost:
            if (auto *gun = dynamic_cast<ConfigurableGun *>(GetGun()))
            {
                gun->UpgradePattern();
            }
            break;
        case IconEffectType::BombQuantityBoost:
            GainBombCount();
            break;
        case IconEffectType::HealthRestore:
            ReplenishHealth(20.f);
            break;
        case IconEffectType::HealthBoost:
            BoostMaxHealth(10.f);
            break;
        case IconEffectType::LifeIncrease:
            GainLifeCount();
            break;
        case IconEffectType::GasRestore:
            ReplenishGas(20.f);
            break;
        case IconEffectType::GasBoost:
            BoostMaxGas(10.f);
            break;
        case IconEffectType::Teleport:
            // TODO: implement a teleport to warp icon pair method.
            // WarpToLocation(icon->GetIntValue());
            break;

        default:
            CT_LOG_WARN("PlayerShip: Unhandled IconEffectType {}", ToString(icon->GetIconType()));
            break;
    }

    icon->Expire();
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
        TryFireBomb();
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

/// @brief Returns the maximum value for player gas.
/// @return m_maxGas.
float PlayerShip::GetMaxGas() const
{
    return m_maxGas;
}

/// @brief Replenish the PlayerShip's current Gas, used for acceleration.
/// @param amount Float gas to replenish.
void PlayerShip::ReplenishGas(const float amount)
{
    m_gas = std::min(m_maxGas, m_gas + amount);
}

/// @brief Boosts the max end gas for this PlayerShip.
/// @param amount Amount to increase maximum by.
void PlayerShip::BoostMaxGas(const float amount)
{
    m_maxGas += amount;
}

/// @brief Restores current player health.
/// @param amount Amount to heal.
void PlayerShip::ReplenishHealth(const float amount)
{
    m_health = std::min(m_maxHealth, m_health + amount);
}

/// @brief Boosts the max end life for this PlayerShip.
/// @param amount Amount to increase maximum by.
void PlayerShip::BoostMaxHealth(const float amount)
{
    m_maxHealth += amount;
}

/// @brief Override the default TakeDamage method to account for Player Lives and GameOver logic.
/// @param amount Damage to be taken.
void PlayerShip::TakeDamage(const float amount)
{
    if (m_invincible)
    {
        return;
    }

    m_health -= amount;

    if (m_health <= 0)
    {
        LoseLife();
        Respawn();
    }
}

/// @brief Outside hook to kill player by making player take its maxHealth at once.
void PlayerShip::Kill()
{
    if (m_invincible)
    {
        return;
    }

    TakeDamage(m_maxHealth);
}

/// @brief Increment the current player lives count.
void PlayerShip::GainLifeCount()
{
    m_lives++;
}

/// @brief Decrement the current player lives count, and signals for game over if player has no lives left.
void PlayerShip::LoseLife()
{
    m_lives = std::max(0, m_lives - 1);

    CT_LOG_DEBUG("PlayerShip lost a life in combat, lives remaining: {}", m_lives);

    if (m_lives == 0)
    {
        m_alive = false;
        m_gameOver = true;
    }
}

/// @brief Simple getter for the player lives count.
/// @return m_lives.
int PlayerShip::GetLifeCount() const
{
    return m_lives;
}

/// @brief Help respawn the player with proper effects during a death.
void PlayerShip::Respawn()
{
    m_sprite.setColor(sf::Color::White);

    m_health = m_maxHealth;
    m_gas = m_maxGas;

    m_invincible = true;
    m_invincibilityTimer = RESPAWN_INVINCIBILITY_DURATION;
    m_blinkTimer = BLINK_INTERVAL;
    m_visible = true;

    const auto &winSize = WindowManager::Instance().GetWindow().getSize();
    SetPosition(sf::Vector2f(winSize.x / 2.f, winSize.y - m_sprite.getTexture()->getSize().y / 2 - 25));
}

/// @brief Return the state of if this player is invincible or not.
/// @return true / false
bool PlayerShip::GetInvincibleStatus() const
{
    return m_invincible;
}

/// @brief Signal to external management that player lives are negative, game is over.
/// @return m_gameOver.
bool PlayerShip::IsGameOver() const
{
    return m_gameOver;
}

/// @brief Returns the count of current bombs player has.
/// @return m_bombs.
int PlayerShip::GetBombCount() const
{
    return m_bombs;
}

/// @brief Increment the current player bombs count.
void PlayerShip::GainBombCount()
{
    m_bombs++;
}

/// @brief Method to send signal to fire a bomb.
void PlayerShip::TryFireBomb()
{
    if (m_bombs <= 0 || m_bombCooldown > 0.f)
    {
        return;
    }

    BombProjectileConfig cfg{}; // optional to change constants from default.

    auto bomb = ProjectileFactory::Instance().CreateBombProjectile(GetPosition(), BOMB_VELOCITY, m_allegiance, cfg);

    if (bomb)
    {
        ProjectileManager::Instance().AddProjectile(bomb);
        m_bombs--;
        m_bombCooldown = BOMB_COOLDOWN_SEC;

        CT_LOG_DEBUG("PlayerShip attempted to launch a Bomb.");
    }
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

    // Handling bomb projectile cooldown in HandleGunUpdate as opposed to in direct Update() call.
    m_bombCooldown = std::max(0.f, m_bombCooldown - dt);
}

/// @brief Helper method to blink during periods of invulnerability during a respawn.
/// @param dt delta time since last update.
void PlayerShip::HandleRespawnUpdate(const float dt)
{
    if (m_invincible)
    {
        m_invincibilityTimer -= dt;
        m_blinkTimer -= dt;

        if (m_blinkTimer <= 0.f)
        {
            m_visible = !m_visible;
            m_sprite.setColor(m_visible ? sf::Color::White : sf::Color(255, 255, 255, 64));
            m_blinkTimer = BLINK_INTERVAL;
        }

        if (m_invincibilityTimer <= 0.f)
        {
            m_invincible = false;
            m_sprite.setColor(sf::Color::White);
        }
    }
}

/// @brief Initialize gun stats
void PlayerShip::InitializeGunStats()
{
    m_gunStats.fireRate = BASE_PLAYER_PROJECTILE_FIRERATE;
    m_gunStats.damage = BASE_PLAYER_PROJECTILE_DAMAGE;
    m_gunStats.speed = BASE_PLAYER_PROJECTILE_SPEED;
    m_gunStats.tint = BASE_PLAYER_PROJECTILE_COLOR;
    m_gunStats.homing = false;

    m_gun = std::make_unique<UpgradableGun>(m_gunStats);

    sf::Vector2f spriteSize(static_cast<float>(m_sprite.getTexture()->getSize().x),
                            static_cast<float>(m_sprite.getTexture()->getSize().y));

    m_gun->SetAllegiance(Allegiance::Player, spriteSize);
}

/// @brief A helper method to clean up the PlayerShip constructor. Initializes health, gas, and rates.
void PlayerShip::InitializeGenericStats()
{
    m_lives = PLAYER_STARTING_LIVES;
    m_health = m_maxHealth = INITIAL_HEALTH_MAX;
    m_gas = m_maxGas = INITIAL_GAS_MAX;

    m_gasDrainRate = GAS_DRAIN_RATE;
    m_accelerationMultiplier = ACCEL_VELOCITY_MULTIPLIER;
    m_baseSpeed = BASE_SHIP_VELOCITY;

    m_speed = {m_baseSpeed * ResolutionScaleManager::Instance().GetScaleX(),
               m_baseSpeed * ResolutionScaleManager::Instance().GetScaleY()};
}
