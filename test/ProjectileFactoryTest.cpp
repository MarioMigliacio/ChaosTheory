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
#include "Macros.h"
#include "SettingsManager.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class ProjectileFactoryTest : public ::testing::Test
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
    }

    void TearDown() override
    {
        SettingsManager::Instance().Shutdown();
    }
};

TEST_F(ProjectileFactoryTest, CreateProjectile_Player_ValidStats)
{
    ProjectileStats stats;
    stats.fireRate = 0.5f;
    stats.damage = 12.5f;
    stats.speed = 300.f;
    stats.tint = sf::Color::Green;
    stats.projectilesPerShot = 1;

    sf::Vector2f position(100.f, 200.f);
    sf::Vector2f direction(0.f, -1.f);

    auto projectile =
        ProjectileFactory::Instance().CreateBasicProjectile(position, direction, stats, Allegiance::Player);

    ASSERT_NE(projectile, nullptr);
    EXPECT_EQ(projectile->GetAllegiance(), Allegiance::Player);
    EXPECT_TRUE(projectile->IsAlive());
    EXPECT_EQ(projectile->GetPosition(), position);
    EXPECT_FLOAT_EQ(projectile->GetDamage(), stats.damage);
}

TEST_F(ProjectileFactoryTest, CreateProjectile_Enemy_CustomStats)
{
    ProjectileStats stats;
    stats.fireRate = 1.0f;
    stats.damage = 8.f;
    stats.speed = 250.f;
    stats.tint = sf::Color::Red;
    stats.projectilesPerShot = 1;

    sf::Vector2f position(300.f, 400.f);
    sf::Vector2f direction(0.f, 1.f); // Enemy fires downward

    auto projectile =
        ProjectileFactory::Instance().CreateBasicProjectile(position, direction, stats, Allegiance::Enemy);

    ASSERT_NE(projectile, nullptr);
    EXPECT_EQ(projectile->GetAllegiance(), Allegiance::Enemy);
    EXPECT_TRUE(projectile->IsAlive());
    EXPECT_EQ(projectile->GetPosition(), position);
    EXPECT_FLOAT_EQ(projectile->GetDamage(), stats.damage);
    EXPECT_FLOAT_EQ(projectile->GetVelocity().y, stats.speed);
}
