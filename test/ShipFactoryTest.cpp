// ============================================================================
//  File        : ShipFactoryTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-18
//  Description : Unit tests for the Chaos Theory ShipFactory and IShip
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "ShipFactory.h"
#include "AlienShip.h"
#include "AssetManager.h"
#include "BasicShip.h"
#include "Macros.h"
#include "SettingsManager.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class ShipFactoryTest : public ::testing::Test
{
  protected:
    std::shared_ptr<Settings> m_settings;

    void SetUp() override
    {
        m_settings = CreateTestSettings();

        SettingsManager::Instance().Init(m_settings);

        if (!LogManager::Instance().IsInitialized())
        {
            LogManager::Instance().Init();
        }

        if (!AssetManager::Instance().IsInitialized())
        {
            AssetManager::Instance().Init(m_settings);
            AssetManager::Instance().LoadTexture("BasicShip", "assets/sprites/enemies/BasicShip.png");
            AssetManager::Instance().LoadTexture("AlienShip", "assets/sprites/enemies/AlienShip.png");
        }
    }

    void TearDown() override
    {
        if (AssetManager::Instance().IsInitialized())
        {
            AssetManager::Instance().Shutdown();
        }

        SettingsManager::Instance().Shutdown();
        m_settings.reset();
    }
};

TEST_F(ShipFactoryTest, CanCreateBasicShip)
{
    auto ship = ShipFactory::Instance().CreateBasicShip({400.f, 100.f}, 0);
    EXPECT_NE(ship, nullptr);
    EXPECT_TRUE(ship->IsAlive());
    EXPECT_EQ(ship->GetAllegiance(), 0);
}

TEST_F(ShipFactoryTest, BasicShipDiesAfterDamage)
{
    auto ship = ShipFactory::Instance().CreateBasicShip({200.f, 150.f}, 0);
    ship->TakeDamage(500);
    EXPECT_FALSE(ship->IsAlive());
}

TEST_F(ShipFactoryTest, CanCreateAlienShip)
{
    auto ship = ShipFactory::Instance().CreateAlienShip({400.f, 100.f}, 0);
    EXPECT_NE(ship, nullptr);
    EXPECT_TRUE(ship->IsAlive());
    EXPECT_EQ(ship->GetAllegiance(), 0);
}

TEST_F(ShipFactoryTest, AlienShipDiesAfterDamage)
{
    auto ship = ShipFactory::Instance().CreateAlienShip({200.f, 150.f}, 0);
    ship->TakeDamage(500);
    EXPECT_FALSE(ship->IsAlive());
}