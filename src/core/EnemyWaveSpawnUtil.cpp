// ============================================================================
//  File        : EnemyWaveSpawnUtil.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-21
//  Description : Time-based, toggleable enemy wave scheduler.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "EnemyWaveSpawnUtil.h"

/// @brief Clears the current list of maintained waves, and resets timer.
void EnemyWaveSpawnUtil::Clear()
{
    m_waves.clear();
    m_time = 0.f;
    m_lastFiredIndex = -1;
}

/// @brief Add a Wave configuration to the list of maintained waves.
/// @param wave Wave struct with wave configuration.
void EnemyWaveSpawnUtil::AddWave(const Wave &wave)
{
    m_waves.push_back(wave);
}

/// @brief Tick down the timer and use the spawner method if enabled and enough time elapses.
/// @param dt Delta time since last frame.
void EnemyWaveSpawnUtil::Update(float dt)
{
    m_time += dt;

    // Time-gated spawns
    for (std::size_t i = 0; i < m_waves.size(); ++i)
    {
        auto &w = m_waves[i];
        if (w.enabled && !w.fired && m_time >= w.atSeconds)
        {
            FireWaveAtIndex(i);
        }
    }

    // Optional defeat-driven auto-advance
    if (m_autoAdvance && IsWaveDefeated() && HasUnfiredWave())
    {
        SpawnNextWave();
    }
}

/// @brief Mark all waves as having not been fired, and reset timer to either 0 or startTime.
/// @param startTime Optional float of new startTime value.
void EnemyWaveSpawnUtil::Reset(float startTime)
{
    m_time = startTime;

    for (auto &wave : m_waves)
    {
        wave.fired = false;
    }

    m_lastFiredIndex = -1;
}

/// @brief Allow a function pointer to determine logic for when a wave is considered defeated.
/// @param pred Function pointer, generally a conditional check to ShipManager.
void EnemyWaveSpawnUtil::SetDefeatPredicate(std::function<bool()> pred)
{
    m_isDefeatedPred = std::move(pred);
}

/// @brief Allow auto advance to next wave (used primarily for defeat based wave checks).
/// @param on true / false
void EnemyWaveSpawnUtil::SetAutoAdvance(bool on)
{
    m_autoAdvance = on;
}

/// @brief Immediately call for the next wave to spawn, scan through prpevious fire indexes to find eligible wave index.
/// @return True if FireWaveAtIndex finds correct index, false otherwise.
bool EnemyWaveSpawnUtil::SpawnNextWave()
{
    // find next eligible after last fired; if none, scan from start
    std::size_t start = (m_lastFiredIndex >= 0) ? static_cast<std::size_t>(m_lastFiredIndex + 1) : 0;

    for (std::size_t pass = 0; pass < 2; ++pass)
    {
        for (std::size_t i = start; i < m_waves.size(); ++i)
        {
            if (m_waves[i].enabled && !m_waves[i].fired)
            {
                return FireWaveAtIndex(i);
            }
        }

        start = 0; // wrap once
    }

    return false;
}

/// @brief Return whether or not a wave has been defeated, based on set isDefeatedPred.
/// @return true / false.
bool EnemyWaveSpawnUtil::IsWaveDefeated() const
{
    return m_isDefeatedPred ? m_isDefeatedPred() : false;
}

/// @brief Utility function which will return the state of if an unfired wave exists in collection.
/// @return true / false.
bool EnemyWaveSpawnUtil::HasUnfiredWave() const
{
    for (const auto &w : m_waves)
    {
        if (w.enabled && !w.fired)
        {
            return true;
        }
    }

    return false;
}

/// @brief Attempts to call the spawn method at the index for the current wave.
/// @param i index.
/// @return true / false, based on whether a wave was fired or not.
bool EnemyWaveSpawnUtil::FireWaveAtIndex(std::size_t i)
{
    if (i >= m_waves.size())
    {
        return false;
    }

    auto &w = m_waves[i];

    if (!w.enabled || w.fired)
    {
        return false;
    }

    if (w.spawn)
    {
        w.spawn();
    }

    w.fired = true;
    m_lastFiredIndex = static_cast<int>(i);

    return true;
}