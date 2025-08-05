// ============================================================================
//  File        : CollectableIconManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-27
//  Description : Unit tests for the CollectableIconManager
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "CollectableIconManager.h"
#include "AssetManager.h"
#include "LogManager.h"
#include "Macros.h"
#include "SettingsManager.h"
#include "TestHelpers.h"
#include "UIFactory.h"
#include <gtest/gtest.h>

class CollectableIconManagerTest : public ::testing::Test
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
        AssetManager::Instance().Init(m_settings);
        AssetManager::Instance().LoadTexture("GasRestoreIcon", "assets/sprites/icons/GasRestoreIcon.png");

        CollectableIconManager::Instance().Init();
    }

    void TearDown() override
    {
        CollectableIconManager::Instance().Shutdown();
        AssetManager::Instance().Shutdown();
        SettingsManager::Instance().Shutdown();
    }
};

TEST_F(CollectableIconManagerTest, SpawnAndExpireIcon)
{
    IconConfig cfg{.position = {100.f, 100.f},
                   .size = {32.f, 32.f},
                   .textureKey = "GasRestoreIcon",
                   .type = IconType::GasRestoreIcon};
    CollectableIconManager::Instance().SpawnIcon(cfg);
    EXPECT_FALSE(CollectableIconManager::Instance().GetIcons().empty());

    auto icon = CollectableIconManager::Instance().GetIcons().front();
    icon->Expire();

    CollectableIconManager::Instance().Update(0.016f);
    EXPECT_TRUE(CollectableIconManager::Instance().GetIcons().empty());
}
