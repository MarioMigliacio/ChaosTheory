// ============================================================================
//  File        : BasicProjectileTest.cpp
//  Project     : ChaosTheory (CT) - Tests
//  Author      : Mario Migliacio
//  Created     : 2025-08-13
//  Description : Unit tests for BasicProjectile (movement and spawn)
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include <gtest/gtest.h>

#include "AssetManager.h"
#include "Assets.h"
#include "BasicProjectile.h"
#include "CollisionManager.h"
#include "LogManager.h"
#include "ProjectileManager.h"
#include "SettingsManager.h"
#include "TestHelpers.h"

class BasicProjectileTest : public ::testing::Test
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

        AssetManager::Instance().LoadTexture(SpriteAssets::ProjectileAssets::BasicBulletSpriteKey,
                                             "assets/sprites/projectiles/BasicBullet.png");

        ProjectileManager::Instance().Init();
        CollisionManager::Instance().Init(64.f);
    }

    void TearDown() override
    {
        CollisionManager::Instance().Shutdown();
        ProjectileManager::Instance().Shutdown();
        AssetManager::Instance().Shutdown();
        SettingsManager::Instance().Shutdown();
    }
};

TEST_F(BasicProjectileTest, MovesForwardWithUpdate)
{
    // start near center, shoot up
    BasicProjectile p({200.f, 300.f}, {0.f, -400.f}, 10.f, Allegiance::Player, sf::Color::White);

    // initial scale snapshot
    auto pos0 = p.GetPosition();

    const float dt = 0.25f; // 250 ms
    p.Update(dt);

    auto pos1 = p.GetPosition();

    EXPECT_FLOAT_EQ(pos0.x, 200.f);
    EXPECT_FLOAT_EQ(pos1.y, 300.f + (-400.f * dt));
}
