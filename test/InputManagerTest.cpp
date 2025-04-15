// ============================================================================
//  File        : InputManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Main test for the Chaos Theory Input Manager class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "InputManager.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class InputManagerTest : public ::testing::Test
{
  protected:
    std::shared_ptr<Settings> m_settings;

    void SetUp() override
    {
        m_settings = CreateTestSettings();
        InputManager::Instance().Init(m_settings);
    }

    void TearDown() override
    {
        if (InputManager::Instance().IsInitialized())
        {
            InputManager::Instance().Shutdown();
        }

        m_settings.reset();
    }
};

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(InputManagerTest, InitializesCorrectly)
{
    EXPECT_TRUE(InputManager::Instance().IsInitialized());
}

// TEST_F(InputManagerTest, KeyBindingExists)
// {
//     const auto &bindings = InputManager::Instance().GetKeyBindings();
//     auto it = bindings.find("MoveUp");
//     EXPECT_NE(it, bindings.end());
// }

// TEST_F(InputManagerTest, CanModifyBinding)
// {
//     InputManager::Instance().SetBinding("MoveUp", sf::Keyboard::Z);
//     const auto &bindings = InputManager::Instance().GetKeyBindings();
//     EXPECT_EQ(bindings.at("MoveUp"), sf::Keyboard::Z);
// }

// TEST_F(InputManagerTest, ShutdownClearsInitializationFlag)
// {
//     InputManager::Instance().Shutdown();
//     EXPECT_FALSE(InputManager::Instance().IsInitialized());
// }
