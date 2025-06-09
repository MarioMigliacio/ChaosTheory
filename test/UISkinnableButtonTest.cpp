// ============================================================================
//  File        : UISkinnableButtonTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-28
//  Description : Unit tests for the Chaos Theory UISkinnableButton class
//
//  License     : N/A Open source
// ============================================================================

#include "UISkinnableButton.h"
#include "AssetManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class UISkinnableButtonTest : public ::testing::Test
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
            AssetManager::Instance().LoadFont("Default", "assets/fonts/Default.ttf");
            AssetManager::Instance().LoadTexture("BlueIdle", "assets/ui/button/blue/BlueButtonIdle.png");
            AssetManager::Instance().LoadTexture("BlueHover", "assets/ui/button/blue/BlueButtonHover.png");
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

TEST_F(UISkinnableButtonTest, CreationSetsCorrectPositionAndSize)
{
    UISkinnableButton btn({100.f, 200.f}, {140.f, 30.f});
    EXPECT_EQ(btn.GetPosition(), sf::Vector2f(100.f, 200.f));
    EXPECT_EQ(btn.GetSize(), sf::Vector2f(140.f, 30.f));
}

TEST_F(UISkinnableButtonTest, SetPositionUpdatesCorrectly)
{
    UISkinnableButton btn({50.f, 50.f}, {140.f, 30.f});
    btn.SetPosition({300.f, 300.f});
    EXPECT_EQ(btn.GetPosition(), sf::Vector2f(300.f, 300.f));
}

TEST_F(UISkinnableButtonTest, SetSizeUpdatesCorrectly)
{
    UISkinnableButton btn({100.f, 200.f}, {120.f, 20.f});
    btn.SetSize({160.f, 40.f});
    EXPECT_EQ(btn.GetSize(), sf::Vector2f(160.f, 40.f));
}

TEST_F(UISkinnableButtonTest, ContainsReturnsTrueWithinBounds)
{
    UISkinnableButton btn({100.f, 100.f}, {200.f, 50.f});

    btn.SetTextureSkins("BlueIdle", "BlueHover");

    EXPECT_TRUE(btn.Contains({150, 120}));
    EXPECT_FALSE(btn.Contains({10, 10}));
}

TEST_F(UISkinnableButtonTest, SetTextAppliesFontAndSize)
{
    UISkinnableButton btn({100.f, 100.f}, {140.f, 30.f});
    auto font = AssetManager::Instance().GetFont("Default");

    ASSERT_NE(font, nullptr);
    btn.SetText("Hello", *font, 20);

    EXPECT_EQ(btn.GetLabel(), "Hello");
}
