// ============================================================================
//  File        : WindowManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Main test for the Chaos Theory Window Manager class
//  
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include <gtest/gtest.h>
#include "core/WindowManager.h"

TEST(WindowManagerTest, SingletonBehavior) {
    WindowManager& first = WindowManager::Instance();
    WindowManager& second = WindowManager::Instance();

    EXPECT_EQ(&first, &second);
}