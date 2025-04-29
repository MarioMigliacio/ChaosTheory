// ============================================================================
//  File        : ButtonTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-27
//  Description : Main test for the Chaos Theory Button class
//
//  License     : N/A Open source
// ============================================================================

#include "Button.h"
#include "AssetManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class ButtonTest : public ::testing::Test
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

TEST_F(ButtonTest, CanCreateButton)
{
    Button button({100.f, 100.f}, {180.f, 40.f});
    EXPECT_TRUE(button.Contains({150, 120}));
}

TEST_F(ButtonTest, SetPositionUpdatesCorrectly)
{
    Button button({0.f, 0.f}, {180.f, 40.f});
    button.SetPosition({300.f, 300.f});
    EXPECT_TRUE(button.Contains({310, 310}));
}

TEST_F(ButtonTest, SetSizeUpdatesCorrectly)
{
    Button button({100.f, 100.f}, {180.f, 40.f});
    button.SetSize({300.f, 60.f});
    EXPECT_TRUE(button.Contains({150, 120}));
}
