// ============================================================================
//  File        : BasicGunTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-21
//  Description : Unit tests for the BasicGun component.

//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "BasicGun.h"
#include "AssetManager.h"
#include "BaseProjectile.h"
#include "Macros.h"
#include "ProjectileFactory.h"
#include "SettingsManager.h"
#include "TestHelpers.h"
#include "WindowManager.h"
#include <cmath>
#include <gtest/gtest.h>

class BasicGunTest : public ::testing::Test
{
  protected:
    std::shared_ptr<Settings> m_settings;
    std::shared_ptr<BasicGun> gun;

    void SetUp() override
    {
        m_settings = CreateTestSettings();

        SettingsManager::Instance().Init(m_settings);

        if (!LogManager::Instance().IsInitialized())
        {
            LogManager::Instance().Init();
        }

        gun = std::make_shared<BasicGun>(0.1f, Allegiance::Player);
        gun->SetOwnerPosition({100.f, 100.f});

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

TEST_F(BasicGunTest, DoesNotFireBeforeCooldown)
{
    auto proj1 = gun->TryFire();
    EXPECT_NE(proj1, nullptr);

    auto proj2 = gun->TryFire(); // should still be cooling down
    EXPECT_EQ(proj2, nullptr);
}

TEST_F(BasicGunTest, ProjectileFiresFromCorrectPosition)
{
    auto proj = gun->TryFire();
    ASSERT_NE(proj, nullptr);

    auto pos = proj->GetPosition();
    EXPECT_NEAR(pos.x, 100.f + gun->GetBarrelOffset().x, 1.0f);
    EXPECT_NEAR(pos.y, 100.f + gun->GetBarrelOffset().y, 1.0f);
}

TEST_F(BasicGunTest, TryFireTowardsFiresCorrectly)
{
    sf::Vector2f target(200.f, 100.f); // Rightward direction
    auto proj = gun->TryFireTowards(target);

    ASSERT_NE(proj, nullptr);

    const auto firePos = sf::Vector2f(100.f, 100.f) + gun->GetBarrelOffset();
    const auto pos = proj->GetPosition();

    EXPECT_NEAR(pos.x, firePos.x, 1.0f);
    EXPECT_NEAR(pos.y, firePos.y, 1.0f);

    const sf::Vector2f velocity = proj->GetVelocity();
    const float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    EXPECT_NEAR(length, ProjectileFactory::Instance().GetStats(ProjectileCategory::Red).speed, 1.0f);
}
