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
#include "Macros.h"
#include "Settings.h"

AudioManager &AudioManager::Instance()
{
    static AudioManager instance;
    return instance;
}

void AudioManager::Init(std::shared_ptr<Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;
    m_masterVolume = m_settings->m_masterVolume;
    m_music = std::make_unique<sf::Music>();
    m_isMuted = m_settings->m_audioMuted;
    m_isInitialized = true;

    CT_LOG_INFO("AudioManager initialized. Volume: {}, Muted: {}", m_masterVolume, m_isMuted ? "Yes" : "No");
}

void AudioManager::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "Shutdown");

    if (m_music)
    {
        m_music->stop();
    }

    m_music.reset();
    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("AudioManager shutdown complete.");
}

void AudioManager::Update(float dt)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "Update");

    if (!m_music)
    {
        return;
    }

    if (m_isFadingOut)
    {
        m_fadeOutTimer -= dt;
        float factor = std::max(0.f, m_fadeOutTimer / m_fadeOutDuration);
        float newVolume = m_musicVolume * m_masterVolume / 100.f * factor;
        m_music->setVolume(newVolume);

        if (m_fadeOutTimer <= 0.f)
        {
            m_music->stop();
            m_isFadingOut = false;
            CT_LOG_INFO("Music fade-out complete.");
        }
    }

    if (m_isFadingIn)
    {
        m_fadeInTimer -= dt;
        float factor = 1.f - std::max(0.f, m_fadeInTimer / m_fadeInDuration);
        float newVolume = m_musicVolume * m_masterVolume / 100.f * factor;
        m_music->setVolume(newVolume);

        if (m_fadeInTimer <= 0.f)
        {
            m_isFadingIn = false;
            CT_LOG_INFO("Music fade-in complete.");
        }
    }
}

void AudioManager::PlayMusic(const std::string &filename, bool loop, bool fadeIn, float fadeDuration)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "PlayMusic");

    if (m_isMuted)
    {
        CT_LOG_WARN("AudioManager: music is muted, not playing '{}'", filename);
        return;
    }

    CT_LOG_INFO("Attempting to load: {}", filename);

    if (!m_music->openFromFile(filename))
    {
        CT_LOG_WARN("Failed to open music file: {}", filename);
        return;
    }

    m_music->setLoop(loop);

    if (fadeIn)
    {
        m_isFadingIn = true;
        m_fadeInDuration = fadeDuration;
        m_fadeInTimer = m_fadeInDuration;
        m_music->setVolume(0.f);
    }
    else
    {
        m_music->setVolume(m_musicVolume * m_masterVolume / 100.f);
    }

    m_music->play();

    CT_LOG_INFO("Now playing: {}", filename);
}

void AudioManager::PauseMusic()
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "PauseMusic");

    if (m_music->getStatus() == sf::Music::Playing)
    {
        m_music->pause();
        CT_LOG_INFO("Music paused");
    }
}

void AudioManager::ResumeMusic()
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "ResumeMusic");

    if (m_music->getStatus() == sf::Music::Paused)
    {
        m_music->play();
        CT_LOG_INFO("Music resumed");
    }
}

void AudioManager::StopMusic(bool fadeOut, float fadeDuration)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "StopMusic");

    if (!m_music)
    {
        return;
    }

    if (fadeOut)
    {
        m_isFadingOut = true;
        m_fadeOutDuration = fadeDuration;
        m_fadeOutTimer = fadeDuration;
    }
    else
    {
        m_music->stop();
    }
}

bool AudioManager::IsMusicPlaying() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "IsMusicPlaying", false);

    return m_music->getStatus() == sf::Music::Playing;
}

bool AudioManager::IsFading() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "IsFading", false);

    return m_isFadingOut;
}

void AudioManager::SetVolume(float volume)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "SetVolume");

    m_masterVolume = std::clamp(volume, 0.0f, 100.0f);
    m_music->setVolume(m_masterVolume);

    if (m_settings)
    {
        m_settings->m_masterVolume = m_masterVolume;
    }

    CT_LOG_INFO("Volume set to {}", m_masterVolume);
}

float AudioManager::GetVolume() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "GetVolume", 0.0f);

    return m_masterVolume;
}

void AudioManager::Mute()
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "Mute");

    m_isMuted = true;
    m_music->setVolume(0.0f);

    if (m_settings)
    {
        m_settings->m_audioMuted = true;
    }

    CT_LOG_INFO("AudioManager muted");
}

void AudioManager::Unmute()
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "Unmute");

    m_isMuted = false;
    m_music->setVolume(m_masterVolume);

    if (m_settings)
    {
        m_settings->m_audioMuted = false;
    }

    CT_LOG_INFO("AudioManager unmuted");
}

bool AudioManager::IsMuted() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "IsMuted", false);

    return m_isMuted;
}

void AudioManager::SwitchTrack(const std::string &filename, bool loop)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "SwitchTrack");

    if (filename == m_currentTrack)
    {
        CT_LOG_INFO("Requested track '{}' is already playing", filename);
        return;
    }

    StopMusic();
    PlayMusic(filename, loop);
}
