// ============================================================================
//  File        : CollectableIconManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-27
//  Description : Manages collectible UIIcon objects that exist in game world.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "CollectableIconManager.h"
#include "AssetManager.h"
#include "Macros.h"
#include "UIFactory.h"

/// @brief Get the current Instance for this CollectableIconManager singleton.
/// @return reference to existing CollectableIconManager interface.
CollectableIconManager &CollectableIconManager::Instance()
{
    static CollectableIconManager instance;

    return instance;
}

/// @brief Initializes the CollectableIconManager.
void CollectableIconManager::Init()
{
    if (m_initialized)
    {
        return;
    }

    m_icons.clear();
    m_initialized = true;

    CT_LOG_INFO("CollectableIconManager initialized.");
}

/// @brief Returns whether the CollectableIconManager has been initialized.
/// @return m_isInitialized.
bool CollectableIconManager::IsInitialized() const
{
    return m_initialized;
}

/// @brief Shuts down the CollectableIconManager and resets internal state.
void CollectableIconManager::Shutdown()
{
    Clear();
    m_initialized = false;

    CT_LOG_INFO("CollectableIconManager shutdown.");
}

/// @brief Removes all existing objects from the managed collection.
void CollectableIconManager::Clear()
{
    m_icons.clear();
}

/// @brief Spawns an Icon of the requested configuration directly into the collection of living icons.
/// @param config IconConfig to generate from.
void CollectableIconManager::SpawnIcon(const IconConfig &config)
{
    auto icon = UIFactory::Instance().CreateIcon(config);

    if (icon)
    {
        m_icons.push_back(icon);

        CT_LOG_DEBUG("CollectableIcon spawned at ({}, {})", config.position.x, config.position.y);
    }
}

/// @brief Returns the collection of managed icons.
/// @return m_icons.
const std::vector<std::shared_ptr<UIIcon>> &CollectableIconManager::GetIcons() const
{
    return m_icons;
}

/// @brief Updates the collection of managed Icons, removes if dead.
/// @param dt delta time since last update frame.
void CollectableIconManager::Update(float dt)
{
    for (auto &icon : m_icons)
    {
        if (icon && !icon->IsExpired())
        {
            icon->Update(sf::Vector2i(), false, false, dt); // Don't care about mouse input in world context
        }
    }

    // Remove dead or null icons
    for (auto it = m_icons.begin(); it != m_icons.end();)
    {
        if (!(*it) || !(*it)->IsAlive())
        {
            it = m_icons.erase(it);
        }

        else
        {
            ++it;
        }
    }
}

/// @brief Render the Icons to the target window.
/// @param target Render target window to draw icons on.
void CollectableIconManager::Draw(sf::RenderTarget &target)
{
    for (auto &icon : m_icons)
    {
        if (icon && !icon->IsExpired())
        {
            target.draw(*icon);
        }
    }
}

/// @brief Forward the managed list of icons to be registered with CollisionManager.
/// @param cm CollisionManager instance, instead of CollisionManager::Instance().<x>
void CollectableIconManager::RegisterForCollision(CollisionManager &cm)
{
    for (auto &icon : m_icons)
    {
        if (icon && !icon->IsExpired())
        {
            cm.RegisterObject(icon);
        }
    }
}
