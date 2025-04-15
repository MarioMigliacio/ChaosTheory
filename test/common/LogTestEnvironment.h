#pragma once

#include "LogManager.h"
#include <gtest/gtest.h>

class LogTestEnvironment : public ::testing::Environment
{
  public:
    void SetUp() override
    {
        if (!LogManager::Instance().IsInitialized())
        {
            LogManager::Instance().Init();
        }
    }

    void TearDown() override
    {
        if (LogManager::Instance().IsInitialized())
        {
            LogManager::Instance().Shutdown();
        }
    }
};