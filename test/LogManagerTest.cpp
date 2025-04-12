// ============================================================================
//  File        : LogManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Main test for the Chaos Theory Log Manager class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "core/LogManager.h"
#include "core/Settings.h"
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

TEST(LogManagerTest, SingletonInstanceIsConsistent)
{
    LogManager &first = LogManager::Instance();
    LogManager &second = LogManager::Instance();

    EXPECT_EQ(&first, &second);
}

TEST(LogManagerTest, InitializationAndLogging)
{
    Settings testSettings;
    testSettings.logLevel = spdlog::level::debug;

    LogManager::Instance().Init(testSettings);

    spdlog::info("This is a test log message from LogManagerTest.");

    auto logger = LogManager::Instance().GetLogger();
    ASSERT_NE(logger, nullptr);
    EXPECT_EQ(logger->name(), "CT");

    LogManager::Instance().Shutdown();
}