// ============================================================================
//  File        : UITextLabelTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-07
//  Description : Unit tests for the Chaos Theory UITextLabel class
//
//  License     : N/A Open source
// ============================================================================

#include "UITextLabel.h"
#include "AssetManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class UITextLabelTest : public ::testing::Test
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

TEST_F(UITextLabelTest, CreationSetsCorrectPosition)
{
    AssetManager::Instance().LoadFont("Default", "assets/fonts/Default.ttf");
    const auto &font = *AssetManager::Instance().GetFont("Default");

    UITextLabel label("Test", font, 24, {0.f, 0.f});

    EXPECT_EQ(label.GetPosition(), sf::Vector2f(0.f, 0.f));
}

TEST_F(UITextLabelTest, SetPositionUpdatesCorrectly)
{
    AssetManager::Instance().LoadFont("Default", "assets/fonts/Default.ttf");
    const auto &font = *AssetManager::Instance().GetFont("Default");

    UITextLabel label("Test", font, 24, {0.f, 0.f});
    label.SetPosition({100.f, 100.f});

    EXPECT_EQ(label.GetPosition(), sf::Vector2f(100.f, 100.f));
}
