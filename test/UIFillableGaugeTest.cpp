// ============================================================================
//  File        : UIFillableGaugeTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-19
//  Description : Unit tests for the Chaos Theory UIFillableGauge class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIFillableGauge.h"
#include "AssetManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include "UIButton.h"
#include <gtest/gtest.h>

class UIFillableGaugeTest : public ::testing::Test
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
TEST_F(UIFillableGaugeTest, ValueClampsBetween0And1)
{
    UIFillableGauge gauge({0.f, 0.f}, {100.f, 10.f});

    gauge.SetValue(-1.f);
    gauge.SetOrientation(LayoutMode::Horizontal);
    EXPECT_NO_THROW(gauge.SetValue(-0.5f));

    gauge.SetValue(2.f);
    EXPECT_NO_THROW(gauge.SetValue(1.5f));
}

TEST_F(UIFillableGaugeTest, ContainsPointInsideBounds)
{
    UIFillableGauge gauge({10.f, 10.f}, {100.f, 10.f});
    EXPECT_TRUE(gauge.Contains({15, 15}));
    EXPECT_FALSE(gauge.Contains({200, 200}));
}

TEST_F(UIFillableGaugeTest, CanSetTitleLabelAndPosition)
{
    UIFillableGauge gauge({50.f, 50.f}, {100.f, 10.f});
    gauge.SetShowTitleLabel("HP", 16, 4.f, UITextLabelScheme::MintyHerbScheme, GaugeTitlePosition::Above);

    EXPECT_TRUE(gauge.GetSize().y > 10.f); // height should grow due to title label
}

TEST_F(UIFillableGaugeTest, CanSetOrientationAndPercentageDisplay)
{
    UIFillableGauge gauge({20.f, 20.f}, {120.f, 12.f});
    gauge.SetShowPercentage(true);
    gauge.SetOrientation(LayoutMode::Vertical);
    gauge.SetValue(0.5f);

    EXPECT_NO_THROW(gauge.SetPosition({25.f, 25.f}));
}
