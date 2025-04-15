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
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class InputManagerTest : public ::testing::Test
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

TEST_F(InputManagerTest, CanBindAndRetrieveKey)
{
    InputManager::Instance().BindKey("Shoot", sf::Keyboard::F);
    EXPECT_EQ(InputManager::Instance().GetBoundKey("Shoot"), sf::Keyboard::F);
}

TEST_F(InputManagerTest, CanUnbindKey)
{
    InputManager::Instance().BindKey("Jump", sf::Keyboard::Space);
    InputManager::Instance().UnbindKey("Jump");

    EXPECT_EQ(InputManager::Instance().GetBoundKey("Jump"), sf::Keyboard::Unknown);
}

TEST_F(InputManagerTest, KeyPressTracking)
{
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = sf::Keyboard::A;

    InputManager::Instance().Update(event);
    InputManager::Instance().PostUpdate();

    EXPECT_TRUE(InputManager::Instance().IsKeyPressed("MoveLeft"));
    EXPECT_FALSE(InputManager::Instance().IsJustPressed("MoveLeft")); // PostUpdate() clears just pressed
}

TEST_F(InputManagerTest, KeyJustPressedDetected)
{
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = sf::Keyboard::D;

    InputManager::Instance().Update(event);
    EXPECT_TRUE(InputManager::Instance().IsJustPressed("MoveRight"));

    InputManager::Instance().PostUpdate();
    EXPECT_FALSE(InputManager::Instance().IsJustPressed("MoveRight")); // Only true on the first frame
}

TEST_F(InputManagerTest, KeyReleasedState)
{
    sf::Event press;
    press.type = sf::Event::KeyPressed;
    press.key.code = sf::Keyboard::D;
    InputManager::Instance().Update(press);

    sf::Event release;
    release.type = sf::Event::KeyReleased;
    release.key.code = sf::Keyboard::D;
    InputManager::Instance().Update(release);

    EXPECT_FALSE(InputManager::Instance().IsKeyPressed("MoveRight"));
    EXPECT_FALSE(InputManager::Instance().IsJustPressed("MoveRight"));
}

TEST_F(InputManagerTest, UnboundActionReturnsUnknownKey)
{
    EXPECT_EQ(InputManager::Instance().GetBoundKey("Fly"), sf::Keyboard::Unknown);
}
