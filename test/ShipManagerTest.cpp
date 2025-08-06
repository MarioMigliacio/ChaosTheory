// ============================================================================
//  File        : ShipManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-27
//  Description : Unit tests for the ShipManager
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "ShipManager.h"
#include "AssetManager.h"
#include "LogManager.h"
#include "Macros.h"
#include "SettingsManager.h"
#include "ShipFactory.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class ShipManagerTest : public ::testing::Test
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

        SettingsManager::Instance().Init(m_settings);
        AssetManager::Instance().Init(m_settings);
        AssetManager::Instance().LoadTexture("PlayerShipWhite", "assets/sprites/players/PlayerShipWhite.png");

        ShipManager::Instance().Init();
    }

    void TearDown() override
    {
        ShipManager::Instance().Shutdown();
        AssetManager::Instance().Shutdown();
        SettingsManager::Instance().Shutdown();
    }
};

TEST_F(ShipManagerTest, SpawnPlayerAndEnemies)
{
    ShipManager::Instance().SpawnPlayer({200.f, 200.f});
    EXPECT_NE(ShipManager::Instance().GetPlayer(), nullptr);
    EXPECT_TRUE(ShipManager::Instance().GetPlayer()->IsAlive());

    ShipManager::Instance().SpawnBasicEnemy({300.f, 300.f});
    EXPECT_FALSE(ShipManager::Instance().GetEnemies().empty());
}

TEST_F(ShipManagerTest, UpdateCleansDeadEnemies)
{
    ShipManager::Instance().SpawnBasicEnemy({300.f, 300.f});
    auto enemy = ShipManager::Instance().GetEnemies().front();
    enemy->TakeDamage(999);

    ShipManager::Instance().Update(0.016f);
    EXPECT_TRUE(ShipManager::Instance().GetEnemies().empty());
}
