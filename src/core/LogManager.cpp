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
#include "Macros.h"
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

// Initializes the spdlog logger and applies sinks for msvc, console, and file logging.
void LogManager::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    std::filesystem::create_directories("log");

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto vs_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log/ct.log", true);

    auto sink_group = std::make_shared<spdlog::sinks::dist_sink_mt>();
    sink_group->add_sink(console_sink);
    sink_group->add_sink(vs_sink);
    sink_group->add_sink(file_sink);

    m_logger = std::make_shared<spdlog::logger>("CT", sink_group);
    spdlog::set_default_logger(m_logger);

    spdlog::set_pattern("[%T] [%^%l%$] %v");

    spdlog::set_level(static_cast<spdlog::level::level_enum>(spdlog::level::debug));
    m_isInitialized = true;

    CT_LOG_INFO("Logger initialized.");
}

// Shuts down the Log manager and resets internal state.
void LogManager::Shutdown()
{
    CT_LOG_INFO("Logger shutdown.");

    spdlog::shutdown();
    m_logger.reset();
    m_isInitialized = false;
}

// Returns whether the Log manager has been initialized.
bool LogManager::IsInitialized() const
{
    return m_isInitialized;
}

// Returns a reference to the spd logger.
std::shared_ptr<spdlog::logger> LogManager::GetLogger()
{
    return m_logger;
}
