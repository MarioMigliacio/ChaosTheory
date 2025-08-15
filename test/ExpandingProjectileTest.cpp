// ============================================================================
//  File        : ExpandingProjectileTest.cpp
//  Project     : ChaosTheory (CT) - Tests
//  Author      : Mario Migliacio
//  Created     : 2025-08-13
//  Description : Unit tests for ExpandingProjectile (movement & growth)
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include <gtest/gtest.h>

#include "AssetManager.h"
#include "Assets.h"
#include "CollisionManager.h"
#include "ExpandingProjectile.h"
#include "LogManager.h"
#include "ProjectileManager.h"
#include "SettingsManager.h"
#include "TestHelpers.h"

class ExpandingProjectileTest : public ::testing::Test
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

TEST_F(ExpandingProjectileTest, MovesForwardAndGrowsWithDistance)
{
    // start near center, shoot up
    ExpandingProjectile p({200.f, 300.f}, {0.f, -400.f}, 10.f, Allegiance::Player, sf::Color::White);

    // initial scale snapshot
    const auto s0 = p.GetBounds();

    float t = 0.f;

    while (t < 0.20f)
    {
        p.Update(0.01f);
        t += 0.01f;
    }

    // moved up
    auto pos = p.GetPosition();
    bool isAlive = p.IsAlive();
    EXPECT_LT(pos.y, 300.f);

    // grew (bounds got larger)
    const auto s1 = p.GetBounds();
    EXPECT_GT(s1.width, s0.width);
    EXPECT_GT(s1.height, s0.height);
}
