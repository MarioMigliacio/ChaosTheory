// ============================================================================
//  File        : BackgroundTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-16
//  Description : Unit tests for Background parallax scrolling system
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "Background.h"
#include "AssetManager.h"
#include "LogManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>

class BackgroundTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        if (!LogManager::Instance().IsInitialized())
        {
            LogManager::Instance().Init();
        }

        if (!AssetManager::Instance().IsInitialized())
        {
            AssetManager::Instance().Init(CreateTestSettings());
        }

        AssetManager::Instance().LoadTexture("test_layer", "assets/backgrounds/ChaosTheorySplash.png");
    }

    void TearDown() override
    {
        if (AssetManager::Instance().IsInitialized())
        {
            AssetManager::Instance().Shutdown();
        }
    }
};

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(BackgroundTest, CanInitStaticBackground)
{
    Background bg;
    bg.InitStatic("test_layer");

    // Confirm one layer exists
    EXPECT_EQ(bg.GetLayerCount(), 1);
}

TEST_F(BackgroundTest, CanInitParallaxWithMultipleLayers)
{
    Background bg;
    bg.InitParallax({{"test_layer", 0.2f}, {"test_layer", 0.4f}});

    EXPECT_EQ(bg.GetLayerCount(), 2);
}

TEST_F(BackgroundTest, UpdateScrollsLayerOffsets)
{
    Background bg;
    bg.InitParallax({{"test_layer", 1.0f}});

    // Set motion to right only
    bg.SetLayerMotion("test_layer", {1.0f, 0.0f});
    bg.Update(1.0f); // simulate 1 second at default scrollSpeed (20.0f)

    auto offset = bg.GetLayerOffset("test_layer");
    EXPECT_GT(offset.x, 0.0f);
    EXPECT_FLOAT_EQ(offset.y, 0.0f);
}
