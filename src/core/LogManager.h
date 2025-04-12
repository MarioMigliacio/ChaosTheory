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

#define CT_LOG_TRACE(...)    LogManager::Instance().GetLogger()->trace(__VA_ARGS__)
#define CT_LOG_DEBUG(...)    LogManager::Instance().GetLogger()->debug(__VA_ARGS__)
#define CT_LOG_INFO(...)     LogManager::Instance().GetLogger()->info(__VA_ARGS__)
#define CT_LOG_WARN(...)     LogManager::Instance().GetLogger()->warn(__VA_ARGS__)
#define CT_LOG_ERROR(...)    LogManager::Instance().GetLogger()->error(__VA_ARGS__)
#define CT_LOG_CRITICAL(...) LogManager::Instance().GetLogger()->critical(__VA_ARGS__)

class Settings;

class LogManager {
public:
    static LogManager& Instance();

    void Init(const Settings& settings);
    void Shutdown();

    std::shared_ptr<spdlog::logger> GetLogger();

private:
    LogManager() = default;
    ~LogManager() = default;

    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

private:
    std::shared_ptr<spdlog::logger> logger;
};
