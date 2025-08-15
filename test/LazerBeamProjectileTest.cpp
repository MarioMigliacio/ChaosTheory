// ============================================================================
//  File        : LazerBeamProjectileTest.cpp
//  Project     : ChaosTheory (CT) - Tests
//  Author      : Mario Migliacio
//  Created     : 2025-08-13
//  Description : Unit tests for LazerBeam (segment + stream emission)
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include <gtest/gtest.h>

#include "AssetManager.h"
#include "Assets.h"
#include "ConfigurableGun.h"
#include "LogManager.h"
#include "ProjectileFactory.h"
#include "ProjectileManager.h"
#include "SettingsManager.h"
#include "TestHelpers.h"

class LazerBeamProjectileTest : public ::testing::Test
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

        AssetManager::Instance().LoadTexture("LazerBeamProjectile",
                                             "assets/sprites/projectiles/LazerBeamProjectile.png");

        ProjectileManager::Instance().Init();
    }

    void TearDown() override
    {
        ProjectileManager::Instance().Shutdown();
        AssetManager::Instance().Shutdown();
        SettingsManager::Instance().Shutdown();
    }
};

TEST_F(LazerBeamProjectileTest, SegmentMovesForwardAndCullsOffscreen)
{
    // Build a single segment via the factory (like the game would)
    ProjectileStats stats;
    auto seg = ProjectileFactory::Instance().CreateLazerBeamProjectile({200.f, 300.f}, {0.f, -1.f}, stats,
                                                                       Allegiance::Player, 0.25f);

    ASSERT_NE(seg, nullptr);
    const auto y0 = seg->GetPosition().y;

    // advance a little — should move up (y decreases)
    seg->Update(0.05f);
    EXPECT_LT(seg->GetPosition().y, y0);

    // march far enough that it should leave the window and die
    float t = 0.f;

    while (t < 5.0f && seg->IsAlive())
    {
        seg->Update(0.05f);
        t += 0.05f;
    }

    EXPECT_FALSE(seg->IsAlive());
}

TEST_F(LazerBeamProjectileTest, StreamEmissionScalesWithFireRate)
{
    // Set up a gun with LazerBeam pattern and a deterministic stream config
    ProjectileStats stats;
    stats.pattern = GunPattern::LazerBeam;
    stats.damage = 10.f;
    stats.speed = 400.f;

    ConfigurableGun gun(stats);
    gun.SetAllegiance(Allegiance::Player, /*spriteSize*/ {16.f, 16.f});
    gun.SetOwnerPosition({320.f, 480.f}); // near bottom of screen

    // Tune stream: interval = fireRate * factor (clamped by minInterval)
    LazerBeamStreamConfig streamCfg;
    streamCfg.intervalFactor = 0.20f;
    streamCfg.minInterval = 0.015f; // allow dense emission
    gun.SetLazerBeamStreamConfig(streamCfg);

    auto countSegments = []() -> size_t { return ProjectileManager::Instance().GetProjectiles().size(); };

    // Case A: default fireRate = 0.30s
    gun.SetPattern(GunPattern::LazerBeam);
    ProjectileManager::Instance().Clear(); // if you have a helper; otherwise pop them manually

    float simTime = 0.f;

    while (simTime < 0.60f) // simulate holding trigger for 0.6s
    {
        gun.Update(0.01f);
        gun.TryFire(); // call each frame as if the trigger is held
        simTime += 0.01f;
    }

    const size_t countA = countSegments();
    EXPECT_GT(countA, 5u); // should definitely spawn several

    // Case B: better fireRate (twice as fast ⇒ more density)
    ProjectileStats fast = stats;
    fast.fireRate = 0.15f; // upgraded fire rate
    ConfigurableGun gunFast(fast);
    gunFast.SetAllegiance(Allegiance::Player, {16.f, 16.f});
    gunFast.SetOwnerPosition({320.f, 480.f});
    gunFast.SetLazerBeamStreamConfig(streamCfg);

    // clear projectiles produced by the previous run
    ProjectileManager::Instance().Clear();

    simTime = 0.f;

    while (simTime < 0.60f)
    {
        gunFast.Update(0.01f);
        gunFast.TryFire();
        simTime += 0.01f;
    }

    const size_t countB = countSegments();

    // Expect more segments with better fire rate (not strictly 2x; just higher)
    EXPECT_GT(countB, countA);
}
