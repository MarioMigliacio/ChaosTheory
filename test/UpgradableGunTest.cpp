// ============================================================================
//  File        : UpgradableGunTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-31
//  Description : Unit tests for UpgradableGun (player weapon) behavior.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UpgradableGun.h"
#include "AssetManager.h"
#include "BaseProjectile.h"
#include "LogManager.h"
#include "ProjectileManager.h"
#include "SettingsManager.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class UpgradableGunTest : public ::testing::Test
{
  protected:
    std::shared_ptr<Settings> m_settings;

    void SetUp() override
    {
        if (!LogManager::Instance().IsInitialized())
        {
            LogManager::Instance().Init();
        }

        m_settings = CreateTestSettings();
        SettingsManager::Instance().Init(m_settings);

        if (!AssetManager::Instance().IsInitialized())
        {
            AssetManager::Instance().Init(m_settings);
        }

        AssetManager::Instance().LoadTexture("BasicBullet", "assets/sprites/projectiles/BasicBullet.png");
        ProjectileManager::Instance().Init();
    }

    void TearDown() override
    {
        ProjectileManager::Instance().Shutdown();
        AssetManager::Instance().Shutdown();
        SettingsManager::Instance().Shutdown();
    }
};

TEST_F(UpgradableGunTest, FiresProjectileUpward)
{
    ProjectileStats stats;
    stats.damage = 15.f;
    stats.speed = 400.f;

    UpgradableGun gun(stats);
    gun.SetOwnerPosition({200.f, 300.f});
    gun.SetAllegiance(Allegiance::Player, {8.f, 8.f});

    gun.Update(0.016f);
    auto proj = gun.TryFire();

    ASSERT_NE(proj, nullptr);
    EXPECT_EQ(proj->GetAllegiance(), Allegiance::Player);
    EXPECT_LT(proj->GetVelocity().y, 0.f); // upward
}

TEST_F(UpgradableGunTest, MultiShotFiresMultipleProjectiles)
{
    ProjectileStats stats;
    stats.damage = 5.f;
    stats.speed = 300.f;
    stats.pattern = GunPattern::DoubleShot;

    UpgradableGun gun(stats);
    gun.SetOwnerPosition({100.f, 100.f});

    auto proj = gun.TryFire();

    EXPECT_EQ(ProjectileManager::Instance().GetProjectiles().size(), 2);
}

TEST_F(UpgradableGunTest, UpgradesIncreaseStats)
{
    ProjectileStats stats;
    stats.damage = 5.f;
    stats.speed = 200.f;
    stats.fireRate = 1.0f;

    UpgradableGun gun(stats);
    gun.UpgradeDamageByFlat(2);
    gun.UpgradeVelocity(1.5f);
    gun.UpgradeFireRate(0.5f);

    auto proj = gun.TryFire();
    ASSERT_NE(proj, nullptr);
    EXPECT_GT(proj->GetDamage(), 5.f);
}
