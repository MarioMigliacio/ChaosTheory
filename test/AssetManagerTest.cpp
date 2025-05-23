// ============================================================================
//  File        : AssetManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Unit tests for the Chaos Theory Asset Manager class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "AssetManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class AssetManagerTest : public ::testing::Test
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

        AssetManager::Instance().Init(m_settings);
    }

    void TearDown() override
    {
        if (AssetManager::Instance().IsInitialized())
        {
            AssetManager::Instance().Shutdown();
        }

        m_settings.reset();
    }
};

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(AssetManagerTest, CanLoadAndRetrieveTextureFontAndSounds)
{
    AssetManager::Instance().LoadTexture("PlayerShip", "assets/sprites/playerShip.png");
    const auto &texture = *AssetManager::Instance().GetTexture("PlayerShip");
    EXPECT_NE(&texture, nullptr);

    AssetManager::Instance().LoadFont("Default", "assets/fonts/Default.ttf");
    const auto &font = *AssetManager::Instance().GetFont("Default");
    EXPECT_NE(&font, nullptr);

    AssetManager::Instance().LoadSound("Bomb", "assets/audio/Bomb.wav");
    const auto &sound = *AssetManager::Instance().GetSound("Bomb");
    EXPECT_NE(&sound, nullptr);
}

TEST_F(AssetManagerTest, ReturnsNullptrForMissingTexture)
{
    const auto &texture = *AssetManager::Instance().GetTexture("nonexistent");
    EXPECT_EQ(&texture, nullptr);

    const auto &font = *AssetManager::Instance().GetFont("nonexistent");
    EXPECT_EQ(&font, nullptr);

    const auto &sound = *AssetManager::Instance().GetSound("nonexistent");
    EXPECT_EQ(&sound, nullptr);
}
