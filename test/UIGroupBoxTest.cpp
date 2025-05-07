// ============================================================================
//  File        : UIGroupBoxTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-07
//  Description : Main test for the Chaos Theory UIGroupBox class
//
//  License     : N/A Open source
// ============================================================================

#include "UIGroupBox.h"
#include "AssetManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include "UIButton.h"
#include <gtest/gtest.h>

class UIGroupBoxTest : public ::testing::Test
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

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(UIGroupBoxTest, CanAddElementAndPositionIsAdjusted)
{
    UIGroupBox box({50.f, 50.f}, {200.f, 100.f});
    box.SetLayoutMode(LayoutMode::Vertical);
    box.SetEdgePadding(10.f);
    box.SetInternalPadding(5.f);
    box.SetCenterChildren(false);

    auto button = std::make_shared<UIButton>(sf::Vector2f{0.f, 0.f}, sf::Vector2f{100.f, 20.f});
    box.AddElement(button);

    auto pos = button->GetPosition();
    EXPECT_GT(pos.x, 50.f);
    EXPECT_GT(pos.y, 50.f);
}

TEST_F(UIGroupBoxTest, ContainsPointWithinBounds)
{
    UIGroupBox box({10.f, 10.f}, {100.f, 100.f});
    EXPECT_TRUE(box.Contains({15, 15}));
    EXPECT_FALSE(box.Contains({200, 200}));
}
