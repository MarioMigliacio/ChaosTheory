// ============================================================================
//  File        : ScoreManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-14
//  Description : Tracks and manages player score in a manager-style singleton.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <cstdint>

// ============================================================================
//  Class       : SettingsManager
//  Purpose     : Singleton for accessing and managing player score.
//
//  Responsibilities:
//      - Load default config from file
//      - Provide runtime-safe access to mutable/shared Settings
//      - Encapsulate persistence logic if needed
//
// ============================================================================
class ScoreManager
{
  public:
    static ScoreManager &Instance();

    void Init();
    bool IsInitialized() const;
    void Shutdown();
    void Clear();

    void Add(int amount);
    void Set(int value);
    int Get() const;

  private:
    ScoreManager() = default;
    ~ScoreManager() = default;

    ScoreManager(const ScoreManager &) = delete;
    ScoreManager &operator=(const ScoreManager &) = delete;

  private:
    int m_score = 0;
    bool m_initialized = false;
};
