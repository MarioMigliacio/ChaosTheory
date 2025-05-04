// ============================================================================
//  File        : RadioButtonTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-27
//  Description : Main test for the Chaos Theory RadioButton class
//
//  License     : N/A Open source
// ============================================================================

#include "RadioButton.h"
#include "AssetManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class RadioButtonTest : public ::testing::Test
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

TEST_F(RadioButtonTest, CanCreateRadioButton)
{
    RadioButton button({100.f, 100.f}, {180.f, 40.f});
    EXPECT_TRUE(button.Contains({150, 120}));
}

TEST_F(RadioButtonTest, SetPositionUpdatesCorrectly)
{
    RadioButton button({0.f, 0.f}, {180.f, 40.f});
    button.SetPosition({300.f, 300.f});
    EXPECT_TRUE(button.Contains({310, 310}));
}

TEST_F(RadioButtonTest, SetSizeUpdatesCorrectly)
{
    RadioButton button({100.f, 100.f}, {180.f, 40.f});
    button.SetSize({300.f, 60.f});
    EXPECT_TRUE(button.Contains({150, 120}));
}

TEST_F(RadioButtonTest, CanToggleSelectedState)
{
    RadioButton btn({100.f, 100.f}, {180.f, 40.f});
    btn.SetSelected(true);
    EXPECT_TRUE(btn.IsSelected());

    btn.SetSelected(false);
    EXPECT_FALSE(btn.IsSelected());
}

TEST_F(RadioButtonTest, CanSetTextAndFont)
{
    RadioButton btn({100.f, 100.f}, {180.f, 40.f});
    const auto &font = AssetManager::Instance().GetFont("Default.ttf");

    btn.SetText("Updated", font, 28);
    // No crash = pass; visual test only
    SUCCEED();
}

TEST_F(RadioButtonTest, CanSetTextColor)
{
    RadioButton btn({100.f, 100.f}, {180.f, 40.f});
    btn.SetTextColor(sf::Color::Red);
    SUCCEED(); // Visual test only
}

TEST_F(RadioButtonTest, CanSetSelectedColor)
{
    RadioButton btn({100.f, 100.f}, {180.f, 40.f});
    btn.SetSelectedColor(sf::Color::Green, sf::Color::Blue);
    btn.SetSelected(true);

    // Force visual update
    btn.Update({150, 120}, false);

    SUCCEED();
}

TEST_F(RadioButtonTest, CanSetHoverColor)
{
    RadioButton btn({100.f, 100.f}, {180.f, 40.f});
    btn.SetHoverColor(sf::Color::Yellow);
    btn.Update({150, 120}, false); // simulate hover
    SUCCEED();
}