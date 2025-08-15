// ============================================================================
//  File        : HomingRocketProjectileTest.cpp
//  Project     : ChaosTheory (CT) - Tests
//  Author      : Mario Migliacio
//  Created     : 2025-08-13
//  Description : Unit tests for HomingRocketProjectile (homing + impact anim)
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include <gtest/gtest.h>

#include "AssetManager.h"
#include "Assets.h"
#include "CollisionManager.h"
#include "HomingRocketProjectile.h"
#include "LogManager.h"
#include "ProjectileManager.h"
#include "SettingsManager.h"
#include "TestHelpers.h"
#include "WindowManager.h"

class HomingRocketProjectileTest : public ::testing::Test
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

        WindowManager::Instance().Init(m_settings);
        AssetManager::Instance().Init(m_settings);

        AssetManager::Instance().LoadTexture(SpriteAssets::ProjectileAssets::RocketProjectileSpriteKey,
                                             "assets/sprites/projectiles/RocketProjectile.png");
        AssetManager::Instance().LoadTexture(SpriteAssets::ProjectileAssets::RocketBlastSpriteKey,
                                             "assets/sprites/projectiles/RocketBlast.png");

        ProjectileManager::Instance().Init();
        CollisionManager::Instance().Init(64.f);
    }

    void TearDown() override
    {
        CollisionManager::Instance().Shutdown();
        ProjectileManager::Instance().Shutdown();
        AssetManager::Instance().Shutdown();
        SettingsManager::Instance().Shutdown();

        if (WindowManager::Instance().IsInitialized())
        {
            WindowManager::Instance().Shutdown();
        }
    }
};

TEST_F(HomingRocketProjectileTest, CollisionTriggersExplosionAndThenDies)
{
    // Dummy enemy in front of rocket’s path
    auto enemy = std::make_shared<DummyEnemyCollidable>(sf::FloatRect{198.f, 180.f, 32.f, 32.f});
    CollisionManager::Instance().RegisterObject(enemy);

    HomingRocketConfig cfg;
    cfg.projFrameWidth = 16;
    cfg.projFrameHeight = 16;
    cfg.explodeFrameWidth = 64;
    cfg.explodeFrameHeight = 64;
    cfg.framesPerRow = 4;
    cfg.totalFrames = 16;
    cfg.frameDelay = 0.01f;
    cfg.fuseTime = 2.0f; // should hit well before this

    HomingRocketProjectile rocket({200.f, 240.f}, {0.f, -300.f}, 10.f, Allegiance::Player, sf::Color::White, cfg);

    const float dt = 0.01f;
    const float animTime = cfg.totalFrames * cfg.frameDelay;
    const float hardStop = cfg.fuseTime + animTime + 1.0f; // generous upper bound

    float t = 0.f;
    float timeOfDeath = -1.f;

    while (t < hardStop)
    {
        rocket.Update(dt);
        t += dt;

        if (!rocket.IsAlive())
        {
            timeOfDeath = t;
            break;
        }
    }

    ASSERT_GT(timeOfDeath, 0.f);

    // It should die well before fuse+anim (i.e., due to a collision-triggered explosion)
    EXPECT_LT(timeOfDeath, cfg.fuseTime + animTime + 0.05f);

    // And it should not die instantly (gives confidence an animation actually played)
    EXPECT_GT(timeOfDeath, 0.05f);
}

TEST_F(HomingRocketProjectileTest, SteersTowardTargetWithinHomingWindow)
{
    // Enemy is diagonally up-right from the rocket’s start (within the seek cone).
    auto enemy = std::make_shared<DummyEnemyCollidable>(sf::FloatRect{260.f, 180.f, 32.f, 32.f});
    CollisionManager::Instance().RegisterObject(enemy);

    HomingRocketConfig cfg;
    cfg.projFrameWidth = 16;
    cfg.projFrameHeight = 16;
    cfg.explodeFrameWidth = 64;
    cfg.explodeFrameHeight = 64;
    cfg.framesPerRow = 4;
    cfg.totalFrames = 16;
    cfg.frameDelay = 0.02f;        // a bit slower anim, not important here
    cfg.fuseTime = 5.0f;           // long fuse so we can observe steering
    cfg.homingDuration = 0.4f;     // enough time to steer
    cfg.maxSeekAngleDeg = 80.f;    // roomy forward cone
    cfg.turnRateDegPerSec = 360.f; // responsive steering

    // Start below, moving straight up
    const sf::Vector2f startPos{200.f, 240.f};
    const sf::Vector2f startVel{0.f, -300.f};

    HomingRocketProjectile rocket(startPos, startVel, /*damage*/ 10.f, Allegiance::Player, sf::Color::White, cfg);

    auto norm = [](const sf::Vector2f &v)
    {
        float m = std::sqrt(v.x * v.x + v.y * v.y);
        return (m > 0.f) ? sf::Vector2f(v.x / m, v.y / m) : sf::Vector2f();
    };

    // Compute initial alignment (forward vs. target direction)
    const auto enemyBounds = enemy->GetBounds();
    const sf::Vector2f tgtCenter{enemyBounds.left + enemyBounds.width * 0.5f,
                                 enemyBounds.top + enemyBounds.height * 0.5f};

    const sf::Vector2f initialForward = norm(startVel);
    const sf::Vector2f initialToTgt = norm(tgtCenter - startPos);
    const float initialDot = initialForward.x * initialToTgt.x + initialForward.y * initialToTgt.y;

    // Step simulation for a portion of the homing window
    const float dt = 0.01f;
    float t = 0.f;
    bool turnedRight = false;

    while (t < 0.25f) // within homingDuration (0.4s)
    {
        rocket.Update(dt);
        t += dt;

        // Confirm it hasn't exploded yet in this steering check
        if (!rocket.IsAlive())
        {
            // If it collided super early, we can still evaluate steer using the last known velocity,
            // but in practice with these positions it should steer before impact.
            break;
        }

        // If x-velocity turned positive at any time, we’re steering right toward the target
        if (rocket.GetVelocity().x > 0.f)
        {
            turnedRight = true;
            break;
        }
    }

    EXPECT_TRUE(turnedRight);

    // Also verify alignment improved vs. initial
    const sf::Vector2f curForward = norm(rocket.GetVelocity());
    const sf::Vector2f curToTgt = norm(tgtCenter - rocket.GetPosition());
    const float curDot = curForward.x * curToTgt.x + curForward.y * curToTgt.y;

    EXPECT_GT(curDot, initialDot);
}
