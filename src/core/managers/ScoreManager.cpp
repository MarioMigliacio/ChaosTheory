// ============================================================================
//  File        : ScoreManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-14
//  Description : Tracks and manages player score in a manager-style singleton.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "ScoreManager.h"
#include "Macros.h"
#include <algorithm>

/// @brief Get the current Instance for this ScoreManager singleton.
/// @return reference to existing ScoreManager interface.
ScoreManager &ScoreManager::Instance()
{
    static ScoreManager s_instance;
    return s_instance;
}

/// @brief Initializes this ScoreManager.
void ScoreManager::Init()
{
    if (m_initialized)
    {
        return;
    }

    m_score = 0;
    m_initialized = true;

    CT_LOG_INFO("ScoreManager initialized.");
}

/// @brief Returns whether or not the ScoreManager has been initialized.
/// @return m_isInitialized.
bool ScoreManager::IsInitialized() const
{
    return m_initialized;
}

/// @brief Resets the internal states for the ScoreManager.
void ScoreManager::Shutdown()
{
    if (!m_initialized)
    {
        return;
    }

    Clear();
    m_initialized = false;

    CT_LOG_INFO("ScoreManager shutdown.");
}

/// @brief Function to reset the currently managed score.
void ScoreManager::Clear()
{
    m_score = 0;
}

/// @brief Add to the currently tracked score.
/// @param amount m_score to add to.
void ScoreManager::Add(int amount)
{
    if (!m_initialized || amount <= 0)
    {
        return;
    }

    m_score += amount;
}

/// @brief Set the currently tracked score to a value.
/// @param value new m_score value.
void ScoreManager::Set(int value)
{
    if (!m_initialized)
    {
        return;
    }

    m_score = value;
}

/// @brief Gets the currently tracked score value.
/// @return m_score if this ScoreManager has been initialized, else 0.
int ScoreManager::Get() const
{
    return m_initialized ? m_score : 0;
}
