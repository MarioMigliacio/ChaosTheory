// ============================================================================
//  File        : ProjectileFactoryTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-21
//  Description : Unit tests for the ProjectileFactory

//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "ProjectileFactory.h"
#include "AssetManager.h"
#include "Assets.h"
#include "BaseProjectile.h"
#include "BasicGun.h"
#include "Macros.h"
#include "SettingsManager.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class ProjectileFactoryTest : public ::testing::Test
{
  protected:
    std::shared_ptr<Settings> m_settings;
    std::shared_ptr<BasicGun> gun;

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
            AssetManager::Instance().LoadTexture("BulletWhite", "assets/sprites/projectiles/BulletWhite.png");
        }
    }

    void TearDown() override
    {
        SettingsManager::Instance().Shutdown();
    }
};

TEST_F(ProjectileFactoryTest, CreateBasicProjectile_Player)
{
    sf::Vector2f position(100.f, 200.f);
    auto projectile =
        ProjectileFactory::Instance().CreateBasicProjectile(position, ProjectileCategory::White, Allegiance::Player);

    ASSERT_NE(projectile, nullptr);
    EXPECT_EQ(projectile->GetAllegiance(), Allegiance::Player);
    EXPECT_TRUE(projectile->IsAlive());
    EXPECT_EQ(projectile->GetPosition(), position);

    // Get base stats from factory for comparison
    const auto stats = ProjectileFactory::Instance().GetStats(ProjectileCategory::White);
    EXPECT_EQ(projectile->GetDamage(), stats.damage);
    EXPECT_FLOAT_EQ(projectile->GetVelocity().y, -stats.speed);
}

TEST_F(ProjectileFactoryTest, CreateBasicProjectile_Enemy_HardDifficulty)
{
    SettingsManager::Instance().GetSettings()->m_gameDifficulty = GameDifficultySetting::Hard;

    sf::Vector2f position(300.f, 400.f);
    auto projectile =
        ProjectileFactory::Instance().CreateBasicProjectile(position, ProjectileCategory::Red, Allegiance::Enemy);

    ASSERT_NE(projectile, nullptr);
    EXPECT_EQ(projectile->GetAllegiance(), Allegiance::Enemy);
    EXPECT_TRUE(projectile->IsAlive());

    const auto stats = ProjectileFactory::Instance().GetStats(ProjectileCategory::Red);
    const float expectedSpeed = stats.speed * 1.1f;
    const int expectedDamage = static_cast<int>(stats.damage * 1.25f);

    EXPECT_EQ(projectile->GetDamage(), expectedDamage);
    EXPECT_FLOAT_EQ(projectile->GetVelocity().y, expectedSpeed);
    EXPECT_EQ(projectile->GetPosition(), position);
}
