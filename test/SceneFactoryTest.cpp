// ============================================================================
//  File        : SceneFactoryTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-28
//  Description : Main test for the Chaos Theory Scene Factory class
//
//  License     : N/A Open source
// ============================================================================

#include "SceneFactory.h"
#include "DummyScene.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class SceneFactoryTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        if (!LogManager::Instance().IsInitialized())
        {
            LogManager::Instance().Init();
        }
    }

    void TearDown() override
    {
        // No need to clear SceneFactory explicitly because creators are statically held.
        // Optionally, could add a SceneFactory::Clear() method later if needed for testing hygiene.
    }
};

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(SceneFactoryTest, RegisterAndCreateSceneWorks)
{
    SceneFactory::Instance().Register("Dummy", []() { return std::make_unique<DummyScene>(); });

    auto scene = SceneFactory::Instance().Create("Dummy");
    EXPECT_NE(scene, nullptr);
}

TEST_F(SceneFactoryTest, CreateReturnsNullptrIfNotRegistered)
{
    auto scene = SceneFactory::Instance().Create("NonexistentScene");
    EXPECT_EQ(scene, nullptr);
}

TEST_F(SceneFactoryTest, RegisterOverwritesExistingCreator)
{
    SceneFactory::Instance().Register("OverwriteTest", []() { return std::make_unique<DummyScene>(); });
    auto scene = SceneFactory::Instance().Create("OverwriteTest");

    SceneFactory::Instance().Register("OverwriteTest", []() { return std::make_unique<DummyScene>(); });
    auto scene2 = SceneFactory::Instance().Create("OverwriteTest");

    EXPECT_NE(scene, scene2);
}
