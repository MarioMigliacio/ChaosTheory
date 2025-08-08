// ============================================================================
//  File        : BombProjectileTest.cpp
//  Project     : ChaosTheory (CT) - Tests
//  Author      : Mario Migliacio
//  Created     : 2025-08-08
//  Description : Unit tests for the BombProjectile (flight, fuse, collision)
// ============================================================================

#include <gtest/gtest.h>

#include "AssetManager.h"
#include "Assets.h"
#include "BombProjectile.h"
#include "CollisionManager.h"
#include "LogManager.h"
#include "ProjectileManager.h"
#include "SettingsManager.h"
#include "TestHelpers.h"

class BombProjectileTest : public ::testing::Test
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

        // Load test textures that match production keys used by BombProjectile
        // - 64x64 rocket sprite
        // - 512x512 (4x4 of 128) explosion sheet
        AssetManager::Instance().LoadTexture(SpriteAssets::ProjectileAssets::BombProjectileSpriteKey,
                                             "assets/sprites/projectiles/BombProjectile.png");
        AssetManager::Instance().LoadTexture(SpriteAssets::ProjectileAssets::BombBlastSpriteKey,
                                             "assets/sprites/projectiles/BombBlast.png");

        ProjectileManager::Instance().Init();

        // Spatial grid for collision checks
        CollisionManager::Instance().Init(/*cellSize*/ 64.f);
    }

    void TearDown() override
    {
        CollisionManager::Instance().Shutdown();
        ProjectileManager::Instance().Shutdown();
        AssetManager::Instance().Shutdown();
        SettingsManager::Instance().Shutdown();
    }
};

TEST_F(BombProjectileTest, FlightMovesStraightByVelocityTimesDt)
{
    BombProjectileConfig cfg{.fuseTime = 5.0f,
                             .blastRadius = 128.f,
                             .damage = 50.f,
                             .frameDelay = 0.05f,
                             .projFrameWidth = 64,
                             .projFrameHeight = 64,
                             .frameWidth = 128,
                             .frameHeight = 128,
                             .framesPerRow = 4,
                             .totalFrames = 16};

    BombProjectile bomb(Allegiance::Player, {200.f, 300.f}, {0.f, -400.f}, cfg);

    const float dt = 0.25f; // 250 ms
    bomb.Update(dt);

    auto pos = bomb.GetPosition();
    EXPECT_FLOAT_EQ(pos.x, 200.f);
    EXPECT_FLOAT_EQ(pos.y, 300.f + (-400.f * dt));
    EXPECT_TRUE(bomb.IsAlive());
}

TEST_F(BombProjectileTest, FuseTriggersExplosionPlaysAnimationThenKills)
{
    BombProjectileConfig cfg{.fuseTime = 0.15f,
                             .blastRadius = 128.f,
                             .damage = 80.f,
                             .frameDelay = 0.01f,
                             .projFrameWidth = 64,
                             .projFrameHeight = 64,
                             .frameWidth = 128,
                             .frameHeight = 128,
                             .framesPerRow = 4,
                             .totalFrames = 16};

    BombProjectile bomb(Allegiance::Player, {0.f, 0.f}, {0.f, -100.f}, cfg);

    // Advance past fuse to trigger OnImpact (animation starts)
    float t = 0.f;

    while (t < cfg.fuseTime + 0.005f)
    {
        bomb.Update(0.01f);
        t += 0.01f;
    }

    ASSERT_TRUE(bomb.IsAlive());

    // Advance until animation should finish
    const float animTime = cfg.totalFrames * cfg.frameDelay + 0.05f;
    t = 0.f;

    while (t < animTime)
    {
        bomb.Update(0.01f);
        t += 0.01f;
    }

    EXPECT_FALSE(bomb.IsAlive());
}

TEST_F(BombProjectileTest, CollisionWithEnemyTriggersExplosionFlow)
{
    BombProjectileConfig cfg{.fuseTime = 5.0f,
                             .frameDelay = 0.03f,
                             .projFrameWidth = 64,
                             .projFrameHeight = 64,
                             .frameWidth = 128,
                             .frameHeight = 128,
                             .framesPerRow = 4,
                             .totalFrames = 16};

    // Place a dummy enemy just above the bomb's start so a straight up shot will hit it
    auto enemy = std::make_shared<DummyEnemyCollidable>(sf::FloatRect{198.f, 150.f, 32.f, 32.f});
    CollisionManager::Instance().RegisterObject(enemy);

    BombProjectile bomb(Allegiance::Player, {200.f, 200.f}, {0.f, -250.f}, cfg);

    // Step forward until we expect an overlap
    float t = 0.f;

    // 16 frames * .03 frameTime = .48s time. Allow update to step through just before Kill happens.
    while (t < 0.45f)
    {
        bomb.Update(0.01f);
        t += 0.01f;
    }

    // Explosion should have been triggered and animation running (still alive)
    ASSERT_TRUE(bomb.IsAlive());

    // Let the animation complete and verify bomb is removed
    const float animTime = cfg.totalFrames * cfg.frameDelay + 0.05f;
    t = 0.f;

    while (t < animTime)
    {
        bomb.Update(0.01f);
        t += 0.01f;
    }

    EXPECT_FALSE(bomb.IsAlive());
}
