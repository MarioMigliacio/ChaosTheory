// ============================================================================
//  File        : ProjectileManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-22
//  Description : Unit tests for the ProjectileManager

//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "ProjectileManager.h"
#include "AssetManager.h"
#include "BaseProjectile.h"
#include "EnemyGun.h"
#include "Macros.h"
#include "SettingsManager.h"
#include "TestHelpers.h"
#include "UpgradableGun.h"
#include <gtest/gtest.h>

class ProjectileManagerTest : public ::testing::Test
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

        if (AssetManager::Instance().IsInitialized())
        {
            AssetManager::Instance().Shutdown();
        }

        AssetManager::Instance().Init(m_settings);
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

TEST_F(ProjectileManagerTest, UpgradableGun_AddsSingleProjectile)
{
    ProjectileStats stats;
    stats.fireRate = 0.1f;
    stats.damage = 10.f;
    stats.speed = 300.f;

    UpgradableGun gun(stats);
    gun.SetOwnerPosition({100.f, 100.f});
    gun.SetAllegiance(Allegiance::Player);

    size_t before = ProjectileManager::Instance().GetProjectiles().size();

    auto proj = gun.TryFire();
    ASSERT_NE(proj, nullptr);

    size_t after = ProjectileManager::Instance().GetProjectiles().size();
    EXPECT_EQ(after - before, 1);
}

TEST_F(ProjectileManagerTest, UpgradableGun_MultiShotAddsMultipleProjectiles)
{
    ProjectileStats stats;
    stats.fireRate = 0.1f;
    stats.damage = 5.f;
    stats.speed = 250.f;
    stats.projectilesPerShot = 3;

    UpgradableGun gun(stats);
    gun.SetOwnerPosition({200.f, 200.f});

    size_t before = ProjectileManager::Instance().GetProjectiles().size();
    auto proj = gun.TryFire();
    ASSERT_NE(proj, nullptr);

    size_t after = ProjectileManager::Instance().GetProjectiles().size();
    EXPECT_EQ(after - before, 3);
}

TEST_F(ProjectileManagerTest, EnemyGun_AddsProjectileWithDifficultyScaling)
{
    SettingsManager::Instance().GetSettings()->m_gameDifficulty = GameDifficultySetting::Hard;

    ProjectileStats stats;
    stats.fireRate = 0.1f;
    stats.damage = 5.f;
    stats.speed = 200.f;

    EnemyGun gun(stats);
    gun.SetOwnerPosition({300.f, 300.f});
    gun.SetAllegiance(Allegiance::Enemy);

    size_t before = ProjectileManager::Instance().GetProjectiles().size();

    auto proj = gun.TryFire();
    ASSERT_NE(proj, nullptr);

    size_t after = ProjectileManager::Instance().GetProjectiles().size();
    EXPECT_EQ(after - before, 1);
    EXPECT_GT(proj->GetDamage(), 5.f); // scaling applied
}

TEST_F(ProjectileManagerTest, ManagerRemovesDeadProjectiles)
{
    ProjectileStats stats;
    stats.fireRate = 0.1f;
    stats.damage = 5.f;
    stats.speed = 200.f;

    UpgradableGun gun(stats);
    gun.SetOwnerPosition({400.f, 400.f});

    auto proj = gun.TryFire();
    ASSERT_NE(proj, nullptr);

    proj->Kill(); // mark it as dead
    ProjectileManager::Instance().Update(0.016f);

    auto projectiles = ProjectileManager::Instance().GetProjectiles();
    EXPECT_TRUE(std::none_of(projectiles.begin(), projectiles.end(), [](const auto &p) { return !p->IsAlive(); }));
}
