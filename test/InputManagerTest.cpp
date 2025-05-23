// ============================================================================
//  File        : InputManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Unit tests for the Chaos Theory Input Manager class
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
    EXPECT_FALSE(InputManager::Instance().IsKeyJustPressed("MoveLeft")); // PostUpdate() clears just pressed
}

TEST_F(InputManagerTest, KeyJustPressedDetected)
{
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = sf::Keyboard::D;

    InputManager::Instance().Update(event);
    EXPECT_TRUE(InputManager::Instance().IsKeyJustPressed("MoveRight"));

    InputManager::Instance().PostUpdate();
    EXPECT_FALSE(InputManager::Instance().IsKeyJustPressed("MoveRight")); // Only true on the first frame
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
    EXPECT_FALSE(InputManager::Instance().IsKeyJustPressed("MoveRight"));
}

TEST_F(InputManagerTest, UnboundActionReturnsUnknownKey)
{
    EXPECT_EQ(InputManager::Instance().GetBoundKey("Fly"), sf::Keyboard::Unknown);
}

TEST_F(InputManagerTest, MousePositionTrackingWorks)
{
    sf::Vector2i testPosition(123, 456);
    InputManager::Instance().SetMousePosition(testPosition);

    EXPECT_EQ(InputManager::Instance().GetMousePosition(), testPosition);
}

TEST_F(InputManagerTest, MouseButtonPressedAndReleased)
{
    InputManager::Instance().SetMouseButtonState(sf::Mouse::Left, true);
    EXPECT_TRUE(InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left));

    InputManager::Instance().SetMouseButtonState(sf::Mouse::Left, false);
    EXPECT_FALSE(InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left));
}

TEST_F(InputManagerTest, MouseButtonJustPressed)
{
    // Frame 1: Mouse was not pressed
    InputManager::Instance().SetMouseButtonState(sf::Mouse::Left, false);
    InputManager::Instance().PostUpdate();

    // Frame 2: Mouse is pressed now
    InputManager::Instance().SetMouseButtonState(sf::Mouse::Left, true);
    EXPECT_TRUE(InputManager::Instance().IsMouseButtonJustPressed(sf::Mouse::Left));

    // Advance again
    InputManager::Instance().PostUpdate();
    EXPECT_FALSE(InputManager::Instance().IsMouseButtonJustPressed(sf::Mouse::Left));
}

TEST_F(InputManagerTest, MouseButtonJustReleased)
{
    // Frame 1: Pressed
    InputManager::Instance().SetMouseButtonState(sf::Mouse::Right, true);
    InputManager::Instance().PostUpdate();

    // Frame 2: Released
    InputManager::Instance().SetMouseButtonState(sf::Mouse::Right, false);
    EXPECT_TRUE(InputManager::Instance().IsMouseButtonJustReleased(sf::Mouse::Right));

    // Frame 3: Still released
    InputManager::Instance().PostUpdate();
    EXPECT_FALSE(InputManager::Instance().IsMouseButtonJustReleased(sf::Mouse::Right));
}