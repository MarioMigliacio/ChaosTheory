// ============================================================================
//  File        : AssetManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Main test for the Chaos Theory Asset Manager class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "core/AssetManager.h"
#include <gtest/gtest.h>

TEST(AssetManagerTest, SingletonBehavior)
{
    AssetManager &first = AssetManager::Instance();
    AssetManager &second = AssetManager::Instance();

    EXPECT_EQ(&first, &second);
}