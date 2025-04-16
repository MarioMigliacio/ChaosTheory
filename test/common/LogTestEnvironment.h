// ============================================================================
//  File        : LogTestEnvironment.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Extends the main entry point gtest environment
//                Is useful in automatic base case for SetUp and TearDown
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

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