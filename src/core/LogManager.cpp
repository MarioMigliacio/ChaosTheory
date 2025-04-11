#include "LogManager.h"

LogManager& LogManager::Instance() {
    static LogManager instance;
    return instance;
}

void LogManager::Init() {
    try {
        logger = spdlog::basic_logger_mt("ct_logger", "logs/debug.log");
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::debug);
        spdlog::info("Logger initialized.");
    } catch (const spdlog::spdlog_ex& ex) {
        fprintf(stderr, "Log initialization failed: %s\n", ex.what());
    }
}

void LogManager::Shutdown() {
    spdlog::info("Logger shutdown.");
    spdlog::shutdown();
}

std::shared_ptr<spdlog::logger> LogManager::GetLogger() {
    return logger;
}