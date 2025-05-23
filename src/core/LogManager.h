// ============================================================================
//  File        : LogManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Log Manager is the CT library wrapper for the open source
//                spd logger external logic
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <memory>
#include <spdlog/spdlog.h>

// ============================================================================
//  Class       : LogManager
//  Purpose     : Singleton class that manages the Log handing.
//
//  Responsibilities:
//      - Initializes and shuts down
//      - Prints Trace, Debug, Info, Warn, Error, and Critical statements.
//
// ============================================================================
class LogManager
{
  public:
    static LogManager &Instance();

    void Init();
    void Shutdown();

    bool IsInitialized() const;

    std::shared_ptr<spdlog::logger> GetLogger();

  private:
    LogManager() = default;
    ~LogManager() = default;

    LogManager(const LogManager &) = delete;
    LogManager &operator=(const LogManager &) = delete;

  private:
    std::shared_ptr<spdlog::logger> m_logger;
    bool m_isInitialized = false;
};
