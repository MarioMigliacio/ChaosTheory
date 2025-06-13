// ============================================================================
//  File        : UIHUDPanelTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-13
//  Description : Unit tests for the Chaos Theory UIHUDPanel class
//
//  License     : N/A Open source
// ============================================================================

#include "UIHUDPanel.h"
#include "AssetManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include "UIButton.h"
#include <gtest/gtest.h>

class UIHUDPanelTest : public ::testing::Test
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

TEST_F(UIHUDPanelTest, CanAddLeftAlignedElement)
{
    UIHUDPanel panel({0.f, 0.f}, {300.f, 50.f});
    panel.SetLayoutMode(LayoutMode::Horizontal);
    panel.SetEdgePadding(10.f);
    panel.SetInternalPadding(5.f);
    panel.SetCenterChildren(false);

    auto button = std::make_shared<UIButton>(sf::Vector2f{0.f, 0.f}, sf::Vector2f{80.f, 30.f});
    panel.AddElement(button, HUDSlotAlignment::Left);

    const auto &children = panel.GetChildren();
    EXPECT_EQ(children.size(), 1);
    EXPECT_EQ(children[0], button);

    auto pos = button->GetPosition();
    EXPECT_GE(pos.x, 0.f);
    EXPECT_GE(pos.y, 0.f);
}

TEST_F(UIHUDPanelTest, ContainsPointWithinPanelBounds)
{
    UIHUDPanel panel({10.f, 10.f}, {120.f, 40.f});
    EXPECT_TRUE(panel.Contains({15, 15}));
    EXPECT_FALSE(panel.Contains({200, 200}));
}
