// ============================================================================
//  File        : main_test.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Main entry point for the Chaos Theory Project test
//                executable
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "LogTestEnvironment.h"
#include <gtest/gtest.h>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new LogTestEnvironment());

    return RUN_ALL_TESTS();
}