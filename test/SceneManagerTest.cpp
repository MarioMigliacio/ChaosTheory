// ============================================================================
//  File        : SceneManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-15
//  Description : Unit tests for the Chaos Theory Scene Manager class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SceneManager.h"
#include "DummyScene.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class SceneManagerTest : public ::testing::Test
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

        SceneManager::Instance().Init(m_settings);
    }

    void TearDown() override
    {
        if (SceneManager::Instance().IsInitialized())
        {
            SceneManager::Instance().Shutdown();
        }

        m_settings.reset();
    }
};

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(SceneManagerTest, CanPushScene)
{
    auto scene = std::make_unique<DummyScene>();
    SceneManager::Instance().PushScene(std::move(scene));
    EXPECT_EQ(SceneManager::Instance().GetSceneCount(), 1);
}

TEST_F(SceneManagerTest, ClearScenesEmptiesStack)
{
    SceneManager::Instance().PushScene(std::make_unique<DummyScene>());
    SceneManager::Instance().ClearScenes();
    EXPECT_EQ(SceneManager::Instance().GetSceneCount(), 0);
}

TEST_F(SceneManagerTest, PushSceneInitializesScene)
{
    auto scene = std::make_unique<DummyScene>();
    SceneManager::Instance().PushScene(std::move(scene));
    EXPECT_TRUE(SceneManager::Instance().GetActiveScene()->IsInitialized());
}