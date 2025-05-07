// ============================================================================
//  File        : UIFactoryTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-27
//  Description : Main test for the Chaos Theory UIFactory class
//
//  License     : N/A Open source
// ============================================================================

#include "UIFactory.h"
#include "AssetManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include "UIButton.h"
#include "UISelectableButton.h"
#include "UISlider.h"
#include <gtest/gtest.h>

class UIFactoryTest : public ::testing::Test
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

TEST_F(UIFactoryTest, CanCreateClassicButton)
{
    auto button =
        UIFactory::Instance().CreateButton(ButtonType::Classic, {100.f, 100.f}, {180.f, 40.f}, "TestButton", nullptr);
    EXPECT_NE(button, nullptr);
}

TEST_F(UIFactoryTest, CanCreateRadioButton)
{
    auto radio =
        UIFactory::Instance().CreateButton(ButtonType::Radio, {100.f, 100.f}, {180.f, 40.f}, "TestRadio", nullptr);
    EXPECT_NE(radio, nullptr);
}

TEST_F(UIFactoryTest, CanCreateSlider)
{
    auto slider =
        UIFactory::Instance().CreateSlider("TestSlider", {100.f, 100.f}, {300.f, 20.f}, 0.f, 100.f, 50.f, nullptr);
    EXPECT_NE(slider, nullptr);
}
