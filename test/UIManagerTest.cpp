// ============================================================================
//  File        : UIManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-27
//  Description : Main test for the Chaos Theory UI Manager class
//
//  License     : N/A Open source
// ============================================================================

#include "UIManager.h"
#include "Macros.h"
#include "UIButton.h"
#include <gtest/gtest.h>

class UIManagerTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        if (!LogManager::Instance().IsInitialized())
        {
            LogManager::Instance().Init();
        }

        UIManager::Instance().Init();
    }

    void TearDown() override
    {
        UIManager::Instance().Clear();
    }
};

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(UIManagerTest, CanAddElement)
{
    auto button = std::make_shared<UIButton>(sf::Vector2f(100.f, 100.f), sf::Vector2f(180.f, 40.f));
    UIManager::Instance().AddElement(button);

    EXPECT_EQ(UIManager::Instance().GetElements().size(), 1);
}

TEST_F(UIManagerTest, ClearRemovesAllElements)
{
    auto button1 = std::make_shared<UIButton>(sf::Vector2f(100.f, 100.f), sf::Vector2f(180.f, 40.f));
    auto button2 = std::make_shared<UIButton>(sf::Vector2f(200.f, 200.f), sf::Vector2f(180.f, 40.f));

    UIManager::Instance().AddElement(button1);
    UIManager::Instance().AddElement(button2);

    UIManager::Instance().Clear();

    EXPECT_EQ(UIManager::Instance().GetElements().size(), 0);
}

TEST_F(UIManagerTest, UpdateDoesNotCrashWithNoElements)
{
    // Should not throw or crash
    UIManager::Instance().Update({0, 0}, false, false, 0.f);

    SUCCEED();
}
