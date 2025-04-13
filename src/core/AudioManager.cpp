// ============================================================================
//  File        : AudioManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-12
//  Description : Audio Manager is the CT library dedicated manager of
//                sound and music buffers
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "AudioManager.h"
#include "LogManager.h"
#include "Settings.h"

AudioManager &AudioManager::Instance()
{
    static AudioManager instance;
    return instance;
}

void AudioManager::Init(std::shared_ptr<Settings> settings)
{
    m_settings = settings;
    m_music = std::make_unique<sf::Music>();
    m_volume = m_settings->m_masterVolume;
    m_muted = m_settings->m_audioMuted;

    CT_LOG_INFO("AudioManager initialized. Volume: {}, Muted: {}", m_volume, m_muted ? "Yes" : "No");
}

void AudioManager::Shutdown()
{
    StopMusic();

    m_music.reset();
    m_settings.reset();

    CT_LOG_INFO("AudioManager shutdown complete.");
}

void AudioManager::PlayMusic(const std::string &filename, bool loop)
{
    if (m_muted)
    {
        CT_LOG_WARN("AudioManager: music is muted, not playing '{}'", filename);
        return;
    }

    CT_LOG_INFO("Attempting to load: {}", filename);

    if (!m_music->openFromFile(filename))
    {
        CT_LOG_ERROR("Failed to load music '{}'", filename);
        return;
    }

    m_music->setLoop(loop);
    m_music->setVolume(m_volume);
    m_music->play();
    m_currentTrack = filename;

    CT_LOG_INFO("Now playing '{}'", filename);
}

void AudioManager::PauseMusic()
{
    if (m_music->getStatus() == sf::Music::Playing)
    {
        m_music->pause();
        CT_LOG_INFO("Music paused");
    }
}

void AudioManager::ResumeMusic()
{
    if (m_music->getStatus() == sf::Music::Paused)
    {
        m_music->play();
        CT_LOG_INFO("Music resumed");
    }
}

void AudioManager::StopMusic()
{
    m_music->stop();
    CT_LOG_INFO("Music stopped");
}

bool AudioManager::IsMusicPlaying() const
{
    return m_music->getStatus() == sf::Music::Playing;
}

void AudioManager::SetVolume(float volume)
{
    m_volume = std::clamp(volume, 0.0f, 100.0f);
    m_music->setVolume(m_volume);

    if (m_settings)
    {
        m_settings->m_masterVolume = m_volume;
    }

    CT_LOG_INFO("Volume set to {}", m_volume);
}

float AudioManager::GetVolume() const
{
    return m_volume;
}

void AudioManager::Mute()
{
    m_muted = true;
    m_music->setVolume(0.0f);

    if (m_settings)
    {
        m_settings->m_audioMuted = true;
    }

    CT_LOG_INFO("AudioManager muted");
}

void AudioManager::Unmute()
{
    m_muted = false;
    m_music->setVolume(m_volume);

    if (m_settings)
    {
        m_settings->m_audioMuted = false;
    }

    CT_LOG_INFO("AudioManager unmuted");
}

bool AudioManager::IsMuted() const
{
    return m_muted;
}

void AudioManager::SwitchTrack(const std::string &filename, bool loop)
{
    if (filename == m_currentTrack)
    {
        CT_LOG_INFO("Requested track '{}' is already playing", filename);
        return;
    }

    StopMusic();
    PlayMusic(filename, loop);
}
