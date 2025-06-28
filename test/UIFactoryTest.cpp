// ============================================================================
//  File        : UIFactoryTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-27
//  Description : Unit tests for the Chaos Theory UIFactory class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
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
            AssetManager::Instance().LoadFont("Default", "assets/fonts/Default.ttf");
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
    const sf::Vector2f size = {100.f, 100.f};
    const sf::Vector2f pos = {180.f, 40.f};

    auto button = UIFactory::Instance().CreateButton(INIT_BUTTON_CONFIG(pos, size, "TestButton", nullptr));
    EXPECT_NE(button, nullptr);
}

TEST_F(UIFactoryTest, CanCreateSelectableButton)
{
    const sf::Vector2f size = {100.f, 100.f};
    const sf::Vector2f pos = {180.f, 40.f};

    auto sb =
        UIFactory::Instance().CreateSelectableButton(INIT_SELECTABLE_BUTTON_CONFIG(pos, size, "TestRadio", nullptr));
    EXPECT_NE(sb, nullptr);
}

TEST_F(UIFactoryTest, CanCreateSlider)
{
    const sf::Vector2f size = {100.f, 100.f};
    const sf::Vector2f pos = {300.f, 20.f};

    auto slider =
        UIFactory::Instance().CreateSlider(INIT_SLIDER_CONFIG("TestSlider", pos, size, 0.f, 100.f, 50.f, nullptr));
    EXPECT_NE(slider, nullptr);
}
