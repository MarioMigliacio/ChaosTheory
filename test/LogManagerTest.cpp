#include <gtest/gtest.h>
#include "core/LogManager.h"

TEST(LogManagerTest, SingletonInstanceIsConsistent) {
    LogManager& first = LogManager::Instance();
    LogManager& second = LogManager::Instance();

    EXPECT_EQ(&first, &second);
}

TEST(LogManagerTest, LoggerCanBeInitializedAndUsed) {
    LogManager::Instance().Init();
    auto logger = LogManager::Instance().GetLogger();

    ASSERT_TRUE(logger != nullptr);
    logger->info("This is a test log message from GoogleTest.");
}