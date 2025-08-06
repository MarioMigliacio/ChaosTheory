// ============================================================================
//  File        : PlayerShipTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-25
//  Description : Unit tests for the PlayerShip
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "PlayerShip.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "LogManager.h"
#include "Macros.h"
#include "ProjectileManager.h"
#include "SettingsManager.h"
#include "TestHelpers.h"

#include <gtest/gtest.h>

class PlayerShipTest : public ::testing::Test
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

        if (AssetManager::Instance().IsInitialized())
        {
            AssetManager::Instance().Shutdown();
        }

        AssetManager::Instance().Init(m_settings);
        AssetManager::Instance().LoadTexture("PlayerShipWhite", "assets/sprites/players/PlayerShipWhite.png");
        AssetManager::Instance().LoadTexture("BasicBullet", "assets/sprites/projectiles/BasicBullet.png");

        ProjectileManager::Instance().Init();
        InputManager::Instance().Init(m_settings);
    }

    void TearDown() override
    {
        ProjectileManager::Instance().Shutdown();
        AssetManager::Instance().Shutdown();
        InputManager::Instance().Shutdown();
        SettingsManager::Instance().Shutdown();
    }
};

TEST_F(PlayerShipTest, PlayerShipInitialState)
{
    PlayerShip player;
    EXPECT_TRUE(player.IsAlive());
    EXPECT_EQ(player.GetHealth(), 100);
    EXPECT_GT(player.GetGas(), 0.0f);
}

TEST_F(PlayerShipTest, PlayerShipTakesDamageLosesLife)
{
    PlayerShip player;

    int before = player.GetLifeCount();

    player.TakeDamage(50);
    EXPECT_EQ(player.GetHealth(), 50);

    player.TakeDamage(100);
    int after = player.GetLifeCount();

    EXPECT_FALSE(before == after);
}

TEST_F(PlayerShipTest, GasReplenishmentDoesNotExceedMax)
{
    PlayerShip player;
    player.ReplenishGas(1000.0f);
    EXPECT_LE(player.GetGas(), 100.0f);
}

TEST_F(PlayerShipTest, TryFireSpawnsProjectile)
{
    PlayerShip player;
    player.SetPosition({200.f, 200.f});
    size_t before = ProjectileManager::Instance().GetProjectiles().size();

    auto proj = player.TryFire();
    ASSERT_NE(proj, nullptr);

    size_t after = ProjectileManager::Instance().GetProjectiles().size();
    EXPECT_EQ(after - before, 1);
}
