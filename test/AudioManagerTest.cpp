// ============================================================================
//  File        : AudioManagerTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-21
//  Description : Main test for the Chaos Theory Audio Manager class
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "AudioManager.h"
#include "AssetManager.h"
#include "Macros.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

class AudioManagerTest : public ::testing::Test
{
  protected:
    std::shared_ptr<Settings> m_settings;

    void SetUp() override
    {
        m_settings = CreateTestSettings(); // sets up test audio paths + volumes

        AssetManager::Instance().Init(m_settings);
        AudioManager::Instance().Init(m_settings);

        if (!LogManager::Instance().IsInitialized())
        {
            LogManager::Instance().Init();
        }
    }

    void TearDown() override
    {
        if (AudioManager::Instance().IsInitialized())
        {
            AudioManager::Instance().Shutdown();
        }

        if (AssetManager::Instance().IsInitialized())
        {
            AssetManager::Instance().Shutdown();
        }

        m_settings.reset();
    }
};

// =========================================================================
// TEST CASES
// =========================================================================

TEST_F(AudioManagerTest, InitializesCorrectly)
{
    EXPECT_TRUE(AudioManager::Instance().IsInitialized());
    EXPECT_EQ(AudioManager::Instance().GetMasterVolume(), m_settings->m_masterVolume);
    EXPECT_EQ(AudioManager::Instance().GetMusicVolume(), m_settings->m_musicVolume);
    EXPECT_EQ(AudioManager::Instance().GetSFXVolume(), m_settings->m_sfxVolume);
    EXPECT_EQ(AudioManager::Instance().IsMuted(), m_settings->m_isMuted);
}

TEST_F(AudioManagerTest, MuteAndUnmuteAdjustsVolumeAndSettings)
{
    AudioManager::Instance().Mute();
    EXPECT_TRUE(AudioManager::Instance().IsMuted());
    EXPECT_TRUE(m_settings->m_isMuted);

    AudioManager::Instance().Unmute();
    EXPECT_FALSE(AudioManager::Instance().IsMuted());
    EXPECT_FALSE(m_settings->m_isMuted);
}

TEST_F(AudioManagerTest, SetVolumeSynchronizesWithSettings)
{
    AudioManager::Instance().SetMasterVolume(40.0f);
    AudioManager::Instance().SetMusicVolume(20.0f);
    AudioManager::Instance().SetSFXVolume(10.0f);

    EXPECT_FLOAT_EQ(m_settings->m_masterVolume, 40.0f);
    EXPECT_FLOAT_EQ(m_settings->m_musicVolume, 20.0f);
    EXPECT_FLOAT_EQ(m_settings->m_sfxVolume, 10.0f);
}

TEST_F(AudioManagerTest, PlayMusicSetsState)
{
    AudioManager::Instance().PlayMusic(m_settings->m_audioDirectory + "Default.wav", false);
    EXPECT_TRUE(AudioManager::Instance().IsMusicPlaying());
}

TEST_F(AudioManagerTest, SwitchTrackUpdatesState)
{
    std::string track = m_settings->m_audioDirectory + "Default.wav";

    AudioManager::Instance().PlayMusic(track);
    EXPECT_TRUE(AudioManager::Instance().IsMusicPlaying());

    AudioManager::Instance().SwitchTrack(track);
    // Should log and do nothing since it's the same track
    EXPECT_TRUE(AudioManager::Instance().IsMusicPlaying());
}

TEST_F(AudioManagerTest, PlaySFXDoesNotCrash)
{
    // Should trigger ring buffer assignment, no crash = pass
    AudioManager::Instance().PlaySFX("Bomb");
}
