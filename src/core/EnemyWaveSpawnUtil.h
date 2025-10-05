// ============================================================================
//  File        : EnemyWaveSpawnUtil.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-21
//  Description : Time-based, toggleable enemy wave scheduler.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <functional>
#include <vector>

// ============================================================================
//  Class       : EnemyWaveSpawnUtil
//  Purpose     : EnemyWaveSpawnUtil class supports contained waves of enemies
//                at time intervals through a wave structure configuration.
//
//  Responsibilities:
//      - Add, clear and update a wave of enemies.
//      - Usage of an auto advance option with a SetDefeatPredicate function
//        allows for defeat driven wave spawns.
//      - Supports time-based wave spawns as well.
//
// ============================================================================
class EnemyWaveSpawnUtil
{
  public:
    /// @brief A single wave configuration which will call a spawner function at time interval.
    struct Wave
    {
        /// @brief Time in seconds to fire spawner method.
        float atSeconds = 0.f;

        /// @brief Spawner method to control which ships spawn at desired locations.
        std::function<void()> spawn;

        /// @brief Master switch to enable or disable waves on a config to config basis.
        bool enabled = true;

        /// @brief Whether or not this wave has triggered its spawn function yet.
        bool fired = false;
    };

  public:
    EnemyWaveSpawnUtil() {};
    ~EnemyWaveSpawnUtil() {};

    void Clear();
    void AddWave(const Wave &w);
    void Update(float dt);
    void Reset(float startTime = 0.f);

    void SetDefeatPredicate(std::function<bool()> pred);
    void SetAutoAdvance(bool on);

    bool SpawnNextWave();
    bool IsWaveDefeated() const;
    bool HasUnfiredWave() const;

  private:
    bool FireWaveAtIndex(std::size_t i);

  private:
    std::function<bool()> m_isDefeatedPred;
    std::vector<Wave> m_waves;

    float m_time = 0.f;

    bool m_autoAdvance = false;
    int m_lastFiredIndex = -1;
};
