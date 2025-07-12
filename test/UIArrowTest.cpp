// ============================================================================
//  File        : UIArrowTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-07
//  Description : Unit tests for the Chaos Theory UIArrow class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIArrow.h"
#include "AssetManager.h"
#include "Assets.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class UIArrowTest : public ::testing::Test
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
            AssetManager::Instance().LoadTexture("UIArrowLeft", "assets/ui/arrow/UIArrowLeft.png");
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

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(UIArrowTest, CreationSetsCorrectPositionAndDirection)
{

    UIArrow arrow({100.f, 200.f}, {64.f, 64.f}, UIAssets::UIArrowLeftTextureKey);

    EXPECT_EQ(arrow.GetPosition(), sf::Vector2f(100.f, 200.f));
}

TEST_F(UIArrowTest, SetPositionUpdatesCorrectly)
{
    UIArrow arrow({100.f, 200.f}, {64.f, 64.f}, UIAssets::UIArrowLeftTextureKey);
    arrow.SetPosition({300.f, 300.f});
    EXPECT_EQ(arrow.GetPosition(), sf::Vector2f(300.f, 300.f));
}

TEST_F(UIArrowTest, SetSizeUpdatesCorrectly)
{
    UIArrow arrow({100.f, 200.f}, {64.f, 64.f}, UIAssets::UIArrowLeftTextureKey);
    arrow.SetSize({128.f, 128.f});
    EXPECT_GT(arrow.GetSize().x, 64.f);
    EXPECT_GT(arrow.GetSize().y, 64.f);
}
