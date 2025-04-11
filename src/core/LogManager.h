#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class LogManager {
public:
    static LogManager& Instance();

    void Init();
    void Shutdown();
    std::shared_ptr<spdlog::logger> GetLogger();

    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;
    LogManager(LogManager&&) = delete;
    LogManager& operator=(LogManager&&) = delete;

private:
    LogManager() = default;
    ~LogManager() = default;

    std::shared_ptr<spdlog::logger> logger;
};