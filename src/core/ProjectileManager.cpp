// ============================================================================
//  File        : ProjectileManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : ProjectileManager is the CT collector of ingame projectiles.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "ProjectileManager.h"
#include "Macros.h"
#include <algorithm>

/// @brief Get the current Instance for this ProjectileManager singleton.
/// @return reference to existing ProjectileManager interface.
ProjectileManager &ProjectileManager::Instance()
{
    static ProjectileManager instance;
    return instance;
}

/// @brief Initializes the ProjectileManager using the provided settings.
/// @param settings Settings object to initalize with.
void ProjectileManager::Init(std::shared_ptr<const Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;

    if (!m_isInitialized)
    {
        m_projectiles.clear();
        m_isInitialized = true;
    }

    CT_LOG_INFO("ProjectileManager initialized.");
}

/// @brief Shuts down the ProjectileManager and resets internal state.
void ProjectileManager::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("ProjectileManager", "Shutdown");

    if (m_isInitialized)
    {
        m_projectiles.clear();
        m_isInitialized = false;
    }

    CT_LOG_INFO("ProjectileManager shutdown.");
}

/// @brief Returns whether the ProjectileManager has been initialized.
/// @return m_isInitialized.
bool ProjectileManager::IsInitialized() const
{
    return m_isInitialized;
}

/// @brief Adds the requested BaseProjectile interfacing projectile to the collection to be managed.
/// @param projectile BaseProjectile adhering object.
void ProjectileManager::AddProjectile(const std::shared_ptr<BaseProjectile> &projectile)
{
    if (projectile)
    {
        m_projectiles.push_back(projectile);
    }
}

/// @brief Removes the requested BaseProjectile interfacing projectile to the collection to be managed.
/// @param projectile BaseProjectile adhering object.
void ProjectileManager::RemoveProjectile(const std::shared_ptr<BaseProjectile> &projectile)
{
    m_projectiles.erase(std::remove(m_projectiles.begin(), m_projectiles.end(), projectile), m_projectiles.end());
}

/// @brief Removes all existing objects from the managed collection.
void ProjectileManager::ClearAllProjectiles()
{
    m_projectiles.clear();
}

/// @brief Returns the list of collected Projectiles.
/// @return m_projectiles.
std::vector<std::shared_ptr<BaseProjectile>> ProjectileManager::GetProjectiles() const
{
    return m_projectiles;
}

/// @brief Updates the collection of managed projectiles, removes if dead.
/// @param dt delta time since last update frame.
void ProjectileManager::Update(float dt)
{
    for (auto &p : m_projectiles)
    {
        if (p && p->IsAlive())
        {
            p->Update(dt);
        }
    }

    // Remove dead or null projectiles
    for (auto it = m_projectiles.begin(); it != m_projectiles.end();)
    {
        if (!(*it) || !(*it)->IsAlive())
        {
            it = m_projectiles.erase(it);
        }

        else
        {
            ++it;
        }
    }
}

/// @brief Render the Projectiles to the target window.
/// @param target Render target window to draw projectiles on.
void ProjectileManager::Draw(sf::RenderTarget &target)
{
    for (const auto &p : m_projectiles)
    {
        if (p && p->IsAlive())
        {
            p->Draw(target);
        }
    }
}
