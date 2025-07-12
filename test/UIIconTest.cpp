// ============================================================================
//  File        : UIIconTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-20
//  Description : Unit tests for the Chaos Theory UIIcon class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIIcon.h"
#include "AssetManager.h"
#include "LogManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class UIIconTest : public ::testing::Test
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
    }

    void TearDown() override
    {
        if (AssetManager::Instance().IsInitialized())
        {
            AssetManager::Instance().Shutdown();
        }
    }
};

// ============================================================================
// TEST CASES
// ============================================================================

TEST_F(UIIconTest, ConstructorSetsPositionAndSizeCorrectly)
{
    UIIcon icon({32.f, 32.f}, {100.f, 200.f});

    EXPECT_EQ(icon.GetPosition(), sf::Vector2f(100.f, 200.f));
    EXPECT_EQ(icon.GetSize(), sf::Vector2f(32.f, 32.f));
}

TEST_F(UIIconTest, SetPositionUpdatesCorrectly)
{
    UIIcon icon({32.f, 32.f}, {0.f, 0.f});
    icon.SetPosition({150.f, 250.f});
    EXPECT_EQ(icon.GetPosition(), sf::Vector2f(150.f, 250.f));
}

TEST_F(UIIconTest, SetSizeUpdatesCorrectly)
{
    UIIcon icon({32.f, 32.f}, {0.f, 0.f});
    icon.SetSize({64.f, 64.f});
    EXPECT_EQ(icon.GetSize(), sf::Vector2f(64.f, 64.f));
}

TEST_F(UIIconTest, ContainsDetectionWorks)
{
    AssetManager::Instance().LoadTexture("test_icon", "assets/sprites/AtomicIcon.png");
    UIIcon icon({32.f, 32.f}, {50.f, 50.f});

    icon.SetTextureSkin("test_icon");

    sf::Vector2i insidePoint(55, 55);
    sf::Vector2i outsidePoint(200, 200);

    EXPECT_TRUE(icon.Contains(insidePoint));
    EXPECT_FALSE(icon.Contains(outsidePoint));
}
