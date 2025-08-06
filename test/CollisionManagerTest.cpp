// ============================================================================
//  File        : CollisionManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-27
//  Description : Unit tests for the CollisionManager broad-phase and resolution
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "CollisionManager.h"
#include "AssetManager.h"
#include "CollectableIconManager.h"
#include "LogManager.h"
#include "Macros.h"
#include "ProjectileFactory.h"
#include "ProjectileManager.h"
#include "SettingsManager.h"
#include "ShipFactory.h"
#include "ShipManager.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class CollisionManagerTest : public ::testing::Test
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
        AssetManager::Instance().LoadTexture("BasicShip", "assets/sprites/enemies/BasicShip.png");
        AssetManager::Instance().LoadTexture("AlienShip", "assets/sprites/enemies/AlienShip.png");
        AssetManager::Instance().LoadTexture("GasRestoreIcon", "assets/sprites/icons/GasRestoreIcon.png");

        CollisionManager::Instance().Init(64.f);
        ProjectileManager::Instance().Init();
        ShipManager::Instance().Init();
    }

    void TearDown() override
    {
        ShipManager::Instance().Shutdown();
        ProjectileManager::Instance().Shutdown();
        CollisionManager::Instance().Shutdown();
        AssetManager::Instance().Shutdown();
        SettingsManager::Instance().Shutdown();
    }
};

TEST_F(CollisionManagerTest, PlayerProjectileDamagesEnemy)
{
    ShipManager::Instance().SpawnBasicEnemy({300.f, 300.f});

    ProjectileStats stats;
    stats.damage = 10.f;
    stats.speed = 300.f;
    stats.tint = sf::Color::White;

    auto proj =
        ProjectileFactory::Instance().CreateBasicProjectile({300.f, 295.f}, {0.f, 1.f}, stats, Allegiance::Player);
    ProjectileManager::Instance().AddProjectile(proj);

    auto enemy = ShipManager::Instance().GetEnemies().front();
    int healthBefore = enemy->GetHealth();

    CollisionManager &cm = CollisionManager::Instance();
    cm.Clear();
    ShipManager::Instance().RegisterForCollision(cm);
    ProjectileManager::Instance().RegisterForCollision(cm);
    cm.Update(0.016f);

    EXPECT_LT(enemy->GetHealth(), healthBefore);
}

TEST_F(CollisionManagerTest, PlayerCollidesWithEnemyKillsPlayer)
{
    ShipManager::Instance().SpawnPlayer({400.f, 400.f});
    ShipManager::Instance().SpawnAlienEnemy({400.f, 400.f});

    auto player = ShipManager::Instance().GetPlayer();
    EXPECT_TRUE(player->IsAlive());

    CollisionManager &cm = CollisionManager::Instance();
    cm.Clear();
    ShipManager::Instance().RegisterForCollision(cm);
    cm.Update(0.016f);

    EXPECT_FALSE(player->IsAlive());
}

TEST_F(CollisionManagerTest, PlayerCollectsIconExpiresIt)
{
    ShipManager::Instance().SpawnPlayer({250.f, 250.f});

    IconConfig cfg{.position = {250.f, 250.f},
                   .size = {32.f, 32.f},
                   .textureKey = "GasRestoreIcon",
                   .type = IconType::GasBoostIcon};
    CollectableIconManager::Instance().SpawnIcon(cfg);

    auto icon = CollectableIconManager::Instance().GetIcons().front();
    EXPECT_FALSE(icon->IsExpired());

    CollisionManager &cm = CollisionManager::Instance();
    cm.Clear();
    ShipManager::Instance().RegisterForCollision(cm);
    CollectableIconManager::Instance().RegisterForCollision(cm);
    cm.Update(0.016f);

    EXPECT_TRUE(icon->IsExpired());
}
