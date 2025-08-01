// ============================================================================
//  File        : EnemyGunTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-31
//  Description : Unit tests for EnemyGun behavior including difficulty scaling.
//
//  License     : N/A Open source
// ============================================================================

#include "EnemyGun.h"
#include "AssetManager.h"
#include "BaseProjectile.h"
#include "LogManager.h"
#include "ProjectileManager.h"
#include "SettingsManager.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class EnemyGunTest : public ::testing::Test
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

TEST_F(EnemyGunTest, FiresSingleProjectileDownward)
{
    SettingsManager::Instance().GetSettings()->m_gameDifficulty = GameDifficultySetting::Easy;

    ProjectileStats stats;
    stats.fireRate = 0.1f;
    stats.damage = 10.f;
    stats.speed = 300.f;
    stats.tint = sf::Color::Red;

    EnemyGun gun(stats);
    gun.SetOwnerPosition({100.f, 200.f});
    gun.SetAllegiance(Allegiance::Enemy);

    gun.Update(0.016f);
    auto proj = gun.TryFire();

    ASSERT_NE(proj, nullptr);
    EXPECT_EQ(proj->GetAllegiance(), Allegiance::Enemy);
    EXPECT_FLOAT_EQ(proj->GetDamageAsInt(), stats.damage);
    EXPECT_GT(proj->GetVelocity().y, 0.f); // downward
}

TEST_F(EnemyGunTest, AppliesDifficultyScalingOnInit)
{
    SettingsManager::Instance().GetSettings()->m_gameDifficulty = GameDifficultySetting::Hard;

    ProjectileStats stats;
    stats.damage = 10.f;
    stats.speed = 200.f;

    EnemyGun gun(stats);
    gun.SetOwnerPosition({0.f, 0.f});

    // After scaling, values should increase
    EXPECT_GT(gun.TryFire()->GetDamageAsInt(), 10.f);
}

TEST_F(EnemyGunTest, CanUpgradeDuringBossPhase)
{
    ProjectileStats stats;
    stats.damage = 10.f;
    stats.speed = 200.f;
    stats.fireRate = 1.f;

    EnemyGun gun(stats);
    gun.SetOwnerPosition({50.f, 50.f});

    // Phase upgrade
    gun.UpgradeFireRate(0.5f); // fires faster
    gun.UpgradeDamage(2.0f);   // double damage
    gun.SetProjectilesPerShot(3);

    auto proj = gun.TryFire();
    ASSERT_NE(proj, nullptr);
    EXPECT_GT(proj->GetDamageAsInt(), 10.f);
}
