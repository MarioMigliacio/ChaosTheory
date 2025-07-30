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
#include "Assets.h"
#include "BasicGun.h"
#include "Macros.h"
#include "SettingsManager.h"
#include "TestHelpers.h"
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

TEST_F(ProjectileManagerTest, TryFireAddsOnlyOneProjectile)
{
    BasicGun gun(0.1f, Allegiance::Enemy);
    gun.SetOwnerPosition({100.f, 100.f});

    size_t before = ProjectileManager::Instance().GetProjectiles().size();

    auto projectile = gun.TryFire();

    ASSERT_NE(projectile, nullptr);

    size_t after = ProjectileManager::Instance().GetProjectiles().size();
    EXPECT_EQ(after - before, 1);
}

TEST_F(ProjectileManagerTest, TryFireTowardsAddsOnlyOneProjectile)
{
    BasicGun gun(0.1f, Allegiance::Enemy);
    gun.SetOwnerPosition({100.f, 100.f});

    size_t before = ProjectileManager::Instance().GetProjectiles().size();

    sf::Vector2f target(200.f, 100.f); // Rightward direction

    auto projectile = gun.TryFireTowards(target);

    ASSERT_NE(projectile, nullptr);

    size_t after = ProjectileManager::Instance().GetProjectiles().size();
    EXPECT_EQ(after - before, 1);
}
