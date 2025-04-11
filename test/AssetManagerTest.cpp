#include <gtest/gtest.h>
#include "core/AssetManager.h"

TEST(AssetManagerTest, SingletonBehavior) {
    AssetManager& first = AssetManager::Instance();
    AssetManager& second = AssetManager::Instance();

    EXPECT_EQ(&first, &second); // should point to the same instance
}