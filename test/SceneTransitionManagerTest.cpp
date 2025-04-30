// ============================================================================
//  File        : SceneTransitionManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-28
//  Description : Main test for the Chaos Theory Scene Transition Manager
//
//  License     : N/A Open source
// ============================================================================

#include "SceneTransitionManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class SceneTransitionManagerTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        if (!LogManager::Instance().IsInitialized())
        {
            LogManager::Instance().Init();
        }

        SceneTransitionManager::Instance().Reset();
    }

    void TearDown() override
    {
        SceneTransitionManager::Instance().Reset();
    }
};

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(SceneTransitionManagerTest, StartsFadeInCorrectly)
{
    SceneTransitionManager::Instance().StartFadeIn(1.0f);
    EXPECT_TRUE(SceneTransitionManager::Instance().IsFading());
}

TEST_F(SceneTransitionManagerTest, StartsFadeOutCorrectly)
{
    SceneTransitionManager::Instance().StartFadeOut(1.0f);
    EXPECT_TRUE(SceneTransitionManager::Instance().IsFading());
}

TEST_F(SceneTransitionManagerTest, FadeProgressesOverTime)
{
    SceneTransitionManager::Instance().StartFadeIn(1.0f);

    // Simulate update steps
    for (int i = 0; i < 60; ++i) // assuming 60 frames
    {
        SceneTransitionManager::Instance().Update(1.0f / 60.0f);
    }

    EXPECT_TRUE(SceneTransitionManager::Instance().IsFadeComplete());
}

TEST_F(SceneTransitionManagerTest, FullyOpaqueCompletesFade)
{
    SceneTransitionManager::Instance().StartFadeIn(1.0f);
    EXPECT_TRUE(SceneTransitionManager::Instance().IsFading());
    SceneTransitionManager::Instance().ForceFullyOpaque();
    EXPECT_FALSE(SceneTransitionManager::Instance().IsFading());
    EXPECT_TRUE(SceneTransitionManager::Instance().IsFadeComplete());
}
