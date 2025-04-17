// ============================================================================
//  File        : WindowManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Main test for the Chaos Theory Window Manager class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "WindowManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class WindowManagerTest : public ::testing::Test
{
  protected:
    std::shared_ptr<Settings> m_settings;

    void SetUp() override
    {
        m_settings = CreateTestSettings();

        if (!LogManager::Instance().IsInitialized())
        {
            LogManager::Instance().Init();
        }

        WindowManager::Instance().Init(m_settings);
    }

    void TearDown() override
    {
        if (WindowManager::Instance().IsInitialized())
        {
            WindowManager::Instance().Shutdown();
        }

        m_settings.reset();
    }
};

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(WindowManagerTest, CanInitializeWindow)
{
    EXPECT_TRUE(WindowManager::Instance().IsInitialized());
}

TEST_F(WindowManagerTest, CanAccessRenderWindow)
{
    sf::RenderWindow &window = WindowManager::Instance().GetWindow();
    EXPECT_TRUE(window.isOpen());
}

TEST_F(WindowManagerTest, ShutdownClosesWindow)
{
    WindowManager::Instance().Shutdown();
    EXPECT_FALSE(WindowManager::Instance().IsInitialized());
}

TEST_F(WindowManagerTest, GetWindowSizeReturnsCorrectDimensions)
{
    auto size = WindowManager::Instance().GetWindow().getSize();
    EXPECT_EQ(size.x, m_settings->m_windowWidth);
    EXPECT_EQ(size.y, m_settings->m_windowHeight);
}