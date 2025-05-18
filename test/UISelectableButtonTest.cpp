// ============================================================================
//  File        : UISelectableButtonTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-27
//  Description : Unit tests for the Chaos Theory UISelectableButton class
//
//  License     : N/A Open source
// ============================================================================

#include "UISelectableButton.h"
#include "AssetManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class UISelectableButtonTest : public ::testing::Test
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

TEST_F(UISelectableButtonTest, CanCreateRadioButton)
{
    UISelectableButton button({100.f, 100.f}, {180.f, 40.f});
    EXPECT_TRUE(button.Contains({150, 120}));
}

TEST_F(UISelectableButtonTest, SetPositionUpdatesCorrectly)
{
    UISelectableButton button({0.f, 0.f}, {180.f, 40.f});
    button.SetPosition({300.f, 300.f});
    EXPECT_TRUE(button.Contains({310, 310}));
}

TEST_F(UISelectableButtonTest, SetSizeUpdatesCorrectly)
{
    UISelectableButton button({100.f, 100.f}, {180.f, 40.f});
    button.SetSize({300.f, 60.f});
    EXPECT_TRUE(button.Contains({150, 120}));
}

TEST_F(UISelectableButtonTest, CanToggleSelectedState)
{
    UISelectableButton btn({100.f, 100.f}, {180.f, 40.f});
    btn.SetSelected(true);
    EXPECT_TRUE(btn.IsSelected());

    btn.SetSelected(false);
    EXPECT_FALSE(btn.IsSelected());
}

TEST_F(UISelectableButtonTest, CanSetTextAndFont)
{
    UISelectableButton btn({100.f, 100.f}, {180.f, 40.f});
    const auto &font = *AssetManager::Instance().GetFont("Default.ttf");

    btn.SetText("Updated", font, 28);
    // No crash = pass; visual test only
    SUCCEED();
}

TEST_F(UISelectableButtonTest, CanSetTextColor)
{
    UISelectableButton btn({100.f, 100.f}, {180.f, 40.f});
    btn.SetTextColor(sf::Color::Red);
    SUCCEED(); // Visual test only
}

TEST_F(UISelectableButtonTest, CanSetSelectedColor)
{
    UISelectableButton btn({100.f, 100.f}, {180.f, 40.f});
    btn.SetSelectedColor(sf::Color::Green, sf::Color::Blue);
    btn.SetSelected(true);

    // Force visual update
    btn.Update({150, 120}, false, false, 0.f);

    SUCCEED();
}

TEST_F(UISelectableButtonTest, CanSetHoverColor)
{
    UISelectableButton btn({100.f, 100.f}, {180.f, 40.f});
    btn.SetHoverColor(sf::Color::Yellow);
    btn.Update({150, 120}, false, false, 0.f); // simulate hover
    SUCCEED();
}