// ============================================================================
//  File        : SettingsManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-22
//  Description : Unit tests suite for the Chaos Theory SettingsManager class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SettingsManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class SettingsManagerTest : public ::testing::Test
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
    }

    void TearDown() override
    {
        SettingsManager::Instance().Shutdown();
        m_settings.reset();
    }
};

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(SettingsManagerTest, InitializesCorrectly)
{
    EXPECT_TRUE(SettingsManager::Instance().IsInitialized());
}

TEST_F(SettingsManagerTest, ReturnsSharedSettingsInstance)
{
    auto returned = SettingsManager::Instance().GetSettings();
    EXPECT_EQ(returned.get(), m_settings.get());
}

TEST_F(SettingsManagerTest, ModifiesSettingsCorrectly)
{
    auto settings = SettingsManager::Instance().GetSettings();
    settings->m_masterVolume = 42.0f;

    EXPECT_FLOAT_EQ(SettingsManager::Instance().GetSettings()->m_masterVolume, 42.0f);
}

TEST_F(SettingsManagerTest, ShutdownClearsState)
{
    SettingsManager::Instance().Shutdown();

    EXPECT_FALSE(SettingsManager::Instance().IsInitialized());
    EXPECT_EQ(SettingsManager::Instance().GetSettings(), nullptr);
}

TEST_F(SettingsManagerTest, LoadDefaultsFromConfigSucceeds)
{
    SettingsManager::Instance().Shutdown(); // reset clean
    SettingsManager::Instance().LoadFromFile("config.json");

    auto loaded = SettingsManager::Instance().GetSettings();
    EXPECT_EQ(loaded->m_audioDirectory, "assets/audio/");
    EXPECT_EQ(loaded->m_fontDirectory, "assets/fonts/");
    EXPECT_EQ(loaded->m_spriteDirectory, "assets/sprites/");
}

TEST_F(SettingsManagerTest, LoadDefaultsFromBadPathFails)
{
    SettingsManager::Instance().Shutdown(); // clean up first

    bool success = SettingsManager::Instance().LoadFromFile("bad/path/config.json");
    EXPECT_FALSE(success);
    EXPECT_FALSE(SettingsManager::Instance().IsInitialized());
}
