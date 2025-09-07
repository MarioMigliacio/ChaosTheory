// ============================================================================
//  File        : ShipManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-26
//  Description : Manages all ship entities (player and enemies) in the game.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "ShipManager.h"
#include "Macros.h"
#include "ShipFactory.h"
#include "WindowManager.h"

/// @brief Get the current Instance for this ShipManager singleton.
/// @return reference to existing ShipManager interface.
ShipManager &ShipManager::Instance()
{
    static ShipManager instance;

    return instance;
}

/// @brief Initializes the ShipManager.
void ShipManager::Init()
{
    if (m_initialized)
    {
        return;
    }

    m_enemies.clear();
    m_player.reset();
    m_initialized = true;

    CT_LOG_INFO("ShipManager initialized.");
}

/// @brief Returns whether the ShipManager has been initialized.
/// @return m_isInitialized.
bool ShipManager::IsInitialized() const
{
    return m_initialized;
}

/// @brief Shuts down the ShipManager and resets internal state.
void ShipManager::Shutdown()
{
    Clear();
    m_initialized = false;

    CT_LOG_INFO("ShipManager shutdown.");
}

/// @brief Removes all existing objects from the managed collection.
void ShipManager::Clear()
{
    m_enemies.clear();
    m_player.reset();
}

/// @brief Creates a PlayerShip object to be managed by ShipManager.
/// @param pos Coordinate position to spawn at.
void ShipManager::SpawnPlayer(const sf::Vector2f &pos)
{
    m_player = std::dynamic_pointer_cast<PlayerShip>(ShipFactory::Instance().CreatePlayerShip(pos));
    m_player->SetPosition(pos);

    CT_LOG_INFO("PlayerShip spawned.");
}

/// @brief Creates a BasicShip object to be managed by ShipManager.
/// @param pos Coordinate position to spawn at.
void ShipManager::SpawnBasicEnemy(const sf::Vector2f &pos)
{
    auto enemy = ShipFactory::Instance().CreateBasicShip(pos, Allegiance::Enemy);
    m_enemies.push_back(enemy);
}

/// @brief Creates an AlienShip object to be managed by ShipManager.
/// @param pos Coordinate position to spawn at.
void ShipManager::SpawnAlienEnemy(const sf::Vector2f &pos)
{
    auto enemy = ShipFactory::Instance().CreateAlienShip(pos, Allegiance::Enemy);
    m_enemies.push_back(enemy);
}

/// @brief Creates a BerserkerShip object to be managed by ShipManager.
/// @param pos Coordinate position to spawn at.
void ShipManager::SpawnBerserkerEnemy(const sf::Vector2f &pos)
{
    auto enemy = ShipFactory::Instance().CreateBerserkerShip(pos, Allegiance::Enemy);
    m_enemies.push_back(enemy);
}

/// @brief Creates a CrusaderShip object to be managed by ShipManager.
/// @param pos Coordinate position to spawn at.
void ShipManager::SpawnCrusaderEnemy(const sf::Vector2f &pos)
{
    auto enemy = ShipFactory::Instance().CreateCrusaderShip(pos, Allegiance::Enemy);
    m_enemies.push_back(enemy);
}

/// @brief Creates a GruntShip object to be managed by ShipManager.
/// @param pos Coordinate position to spawn at.
void ShipManager::SpawnGruntEnemy(const sf::Vector2f &pos)
{
    auto enemy = ShipFactory::Instance().CreateGruntShip(pos, Allegiance::Enemy);
    m_enemies.push_back(enemy);
}

/// @brief Returns a reference to the PlayerShip.
/// @return Safe pointer to m_player.
std::shared_ptr<PlayerShip> ShipManager::GetPlayer() const
{
    return m_player;
}

/// @brief Returns a reference to the managed collection of Enemy Ships.
/// @return m_enemies.
const std::vector<std::shared_ptr<BaseShip>> &ShipManager::GetEnemies() const
{
    return m_enemies;
}

/// @brief Updates the collection of managed ships, removes if dead.
/// @param dt delta time since last update frame.
void ShipManager::Update(float dt)
{
    // Update player
    if (m_player && m_player->IsAlive())
    {
        m_player->Update(dt);
    }

    // Update enemies
    for (auto &enemy : m_enemies)
    {
        if (enemy && enemy->IsAlive())
        {
            enemy->Update(dt);
        }
    }

    // Manual cleanup of dead or inactive ships.
    for (auto it = m_enemies.begin(); it != m_enemies.end();)
    {
        if (!(*it)->IsAlive())
        {
            it = m_enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

/// @brief Render the Ships to the target window.
/// @param target Render target window to draw projectiles on.
void ShipManager::Draw(sf::RenderTarget &target)
{
    if (m_player && m_player->IsAlive())
    {
        m_player->Draw(target);
    }

    for (auto &enemy : m_enemies)
    {
        if (enemy && enemy->IsAlive())
        {
            enemy->Draw(target);
        }
    }
}

/// @brief Forward the managed list of Ships to be registered with CollisionManager.
/// @param cm CollisionManager instance, instead of CollisionManager::Instance().<x>
void ShipManager::RegisterForCollision(CollisionManager &cm)
{
    if (m_player && m_player->IsAlive())
    {
        cm.RegisterObject(m_player);
    }

    for (auto &enemy : m_enemies)
    {
        if (enemy && enemy->IsAlive())
        {
            cm.RegisterObject(enemy);
        }
    }
}
