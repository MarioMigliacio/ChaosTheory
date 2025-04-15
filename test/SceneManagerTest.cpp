// ============================================================================
//  File        : SceneManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-15
//  Description : Main test for the Chaos Theory Scene Manager class
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
    std::unique_ptr<SceneManager> m_manager;

    void SetUp() override
    {
        m_settings = CreateTestSettings();
        m_manager = std::make_unique<SceneManager>(m_settings);
        m_manager->Init();
    }

    void TearDown() override
    {
        m_manager->Shutdown();
        m_manager.reset();
    }
};

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(SceneManagerTest, CanPushScene)
{
    auto scene = std::make_unique<DummyScene>();
    m_manager->PushScene(std::move(scene));
    EXPECT_EQ(m_manager->GetSceneCount(), 1);
}

TEST_F(SceneManagerTest, ClearScenesEmptiesStack)
{
    m_manager->PushScene(std::make_unique<DummyScene>());
    m_manager->ClearScenes();
    EXPECT_EQ(m_manager->GetSceneCount(), 0);
}

TEST_F(SceneManagerTest, PushSceneInitializesScene)
{
    auto scene = std::make_unique<DummyScene>();
    m_manager->PushScene(std::move(scene));
    EXPECT_TRUE(m_manager->GetActiveScene()->IsInitialized());
}