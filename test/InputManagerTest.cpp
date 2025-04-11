#include <gtest/gtest.h>
#include "core/InputManager.h"

TEST(InputManagerTest, SingletonBehavior) {
    InputManager& first = InputManager::Instance();
    InputManager& second = InputManager::Instance();

    EXPECT_EQ(&first, &second); // should be the same instance
}