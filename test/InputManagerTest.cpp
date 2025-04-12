// ============================================================================
//  File        : InputManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Main test for the Chaos Theory Input Manager class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "core/InputManager.h"
#include <gtest/gtest.h>

TEST(InputManagerTest, SingletonBehavior)
{
    InputManager &first = InputManager::Instance();
    InputManager &second = InputManager::Instance();

    EXPECT_EQ(&first, &second);
}