#include <gtest/gtest.h>
#include "core/WindowManager.h"

TEST(WindowManagerTest, SingletonBehavior) {
    WindowManager& first = WindowManager::Instance();
    WindowManager& second = WindowManager::Instance();

    EXPECT_EQ(&first, &second); // should point to the same instance
}