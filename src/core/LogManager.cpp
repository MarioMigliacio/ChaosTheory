// ============================================================================
//  File        : LogManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Log Manager is the CT library wrapper for the open source
//                spd logger external logic
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "LogManager.h"

#include <filesystem>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

LogManager &LogManager::Instance()
{
    static LogManager instance;
    return instance;
}

void LogManager::Init()
{
    std::filesystem::create_directories("log");

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto vs_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
    auto file_sink =
        std::make_shared<spdlog::sinks::basic_file_sink_mt>("log/ct.log", true);

    auto sink_group = std::make_shared<spdlog::sinks::dist_sink_mt>();
    sink_group->add_sink(console_sink);
    sink_group->add_sink(vs_sink);
    sink_group->add_sink(file_sink);

    logger = std::make_shared<spdlog::logger>("CT", sink_group);
    spdlog::set_default_logger(logger);

    spdlog::set_pattern("[%T] [%^%l%$] %v");

    spdlog::set_level(
        static_cast<spdlog::level::level_enum>(spdlog::level::debug));
    CT_LOG_INFO("Logger initialized.");
}

void LogManager::Shutdown()
{
    CT_LOG_INFO("Logger shutdown.");
    spdlog::shutdown();
    logger.reset();
}

std::shared_ptr<spdlog::logger> LogManager::GetLogger()
{
    return logger;
}
