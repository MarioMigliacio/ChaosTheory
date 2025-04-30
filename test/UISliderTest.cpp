// ============================================================================
//  File        : UISliderTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-27
//  Description : Main test for the Chaos Theory UISlider class
//
//  License     : N/A Open source
// ============================================================================

#include "UISlider.h"
#include "AssetManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class UISliderTest : public ::testing::Test
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

TEST_F(UISliderTest, CanCreateSlider)
{
    UISlider slider("test", 0.f, 100.f, 50.f, {100.f, 100.f}, {300.f, 20.f}, nullptr);
    EXPECT_TRUE(slider.Contains({150, 110}));
}

TEST_F(UISliderTest, SetPositionUpdatesCorrectly)
{
    UISlider slider("test", 0.f, 100.f, 50.f, {100.f, 100.f}, {300.f, 20.f}, nullptr);
    slider.SetPosition({200.f, 200.f});
    EXPECT_TRUE(slider.Contains({250, 210}));
}

TEST_F(UISliderTest, SetSizeUpdatesCorrectly)
{
    UISlider slider("test", 0.f, 100.f, 50.f, {100.f, 100.f}, {300.f, 20.f}, nullptr);
    slider.SetSize({400.f, 30.f});
    EXPECT_TRUE(slider.Contains({150, 110}));
}
