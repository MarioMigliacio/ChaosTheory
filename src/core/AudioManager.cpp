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
#include "AssetManager.h"
#include "Macros.h"
#include "Settings.h"

AudioManager &AudioManager::Instance()
{
    static AudioManager instance;
    return instance;
}

// Initializes the SFML audio entities using the provided settings.
void AudioManager::Init(std::shared_ptr<Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;
    m_music = std::make_unique<sf::Music>();
    m_activeSounds.resize(MAX_SIMULTANEOUS_SOUNDS);

    m_masterVolume = m_settings->m_masterVolume;
    m_musicVolume = m_settings->m_musicVolume;
    m_sfxVolume = m_settings->m_sfxVolume;
    m_isMuted = m_settings->m_isMuted;

    m_isInitialized = true;

    CT_LOG_INFO("AudioManager initialized. MasterVolume: {}, MusicVolume: {}, SFXVolume: {}, Muted: {}", m_masterVolume,
                m_musicVolume, m_sfxVolume, m_isMuted ? "Yes" : "No");
}

// Provides interface to reload settings and internal variable sync.
void AudioManager::HotReload(std::shared_ptr<Settings> settings)
{
    m_settings = settings;

    m_isMuted = m_settings->m_isMuted;
    SetMasterVolume(m_settings->m_masterVolume);
    SetMusicVolume(m_settings->m_musicVolume);
    SetSFXVolume(m_settings->m_sfxVolume);

    CT_LOG_INFO("AudioManager hot reloaded settings.");
}

// Shuts down the audio manager and resets internal state.
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

// Returns whether or not the Audio manager has been initialized.
bool AudioManager::IsInitialized() const
{
    return m_isInitialized;
}

// Completes state management during a game frame.
void AudioManager::Update(float dt)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "Update");

    if (m_isFadingOut)
    {
        m_fadeOutTimer += dt;
        float progress = std::min(1.f, m_fadeOutTimer / m_fadeOutDuration);
        float targetVolume = m_musicVolume * m_masterVolume / 100.f;
        m_music->setVolume((1.f - progress) * targetVolume);

        if (progress >= 1.f)
        {
            m_isFadingOut = false;
            m_music->stop();

            CT_LOG_INFO("Music fade-out complete.");
        }
    }

    if (m_isFadingIn)
    {
        m_fadeInTimer += dt;
        float progress = std::min(1.f, m_fadeInTimer / m_fadeInDuration);
        float targetVolume = m_musicVolume * m_masterVolume / 100.f;

        m_music->setVolume(progress * targetVolume);

        if (progress >= 1.f)
        {
            m_isFadingIn = false;

            CT_LOG_INFO("Music fade-in complete.");
        }
    }
}

// Request to begin playing a music file, with optional loop and fade features.
void AudioManager::PlayMusic(const std::string &filename, bool loop, bool fadeIn, float fadeDuration)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "PlayMusic");

    if (!m_music->openFromFile(filename))
    {
        CT_LOG_WARN("Failed to open music file: {}", filename);

        return;
    }

    m_music->setLoop(loop);
    m_currentTrack = filename;

    if (fadeIn)
    {
        m_isFadingIn = true;
        m_fadeInTimer = 0.f;
        m_fadeInDuration = fadeDuration;
        m_music->setVolume(0.f);
    }
    else
    {
        float targetVolume = m_musicVolume * m_masterVolume / 100.f;
        m_music->setVolume(m_isMuted ? 0.f : targetVolume);
    }

    m_music->play();

    CT_LOG_INFO("Playing music: '{}' | Loop: {} | FadeIn: {}", filename, loop, fadeIn);
}

// Request to halt any playing music file, with optional fade feature.
void AudioManager::StopMusic(bool fadeOut, float fadeDuration)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "StopMusic");

    if (fadeOut)
    {
        m_isFadingOut = true;
        m_fadeOutTimer = 0.f;
        m_fadeOutDuration = fadeDuration;
    }
    else
    {
        m_music->stop();
    }

    CT_LOG_INFO("Stopping music. FadeOut: {}", fadeOut);
}

// Request to pause any playing music file.
void AudioManager::PauseMusic()
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "PauseMusic");

    if (m_music->getStatus() == sf::Music::Playing)
    {
        m_music->pause();
        CT_LOG_INFO("Music paused");
    }
}

// Request to continue playing any paused music file.
void AudioManager::ResumeMusic()
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "ResumeMusic");

    if (m_music->getStatus() == sf::Music::Paused)
    {
        m_music->play();
        CT_LOG_INFO("Music resumed");
    }
}

// Return the state of whether any music is currently playing.
bool AudioManager::IsMusicPlaying() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "IsMusicPlaying", false);

    return m_music->getStatus() == sf::Music::Playing;
}

// Return the state of whether any music file is currently in the process of fading out.
bool AudioManager::IsFadingOut() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "IsFadingOut", false);

    return m_isFadingOut;
}

// Return the state of whether any music file is currently in the process of fading in.
bool AudioManager::IsFadingIn() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "IsFadingIn", false);

    return m_isFadingIn;
}

// Request to play a sound effect from a ring buffer of SFML managed sound buffers.
void AudioManager::PlaySFX(const std::string &filename)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "PlaySFX");

    sf::SoundBuffer &buffer = AssetManager::Instance().GetSound(filename);

    sf::Sound &sound = m_activeSounds[m_nextSoundIndex];
    sound.setBuffer(buffer);
    sound.setVolume(m_sfxVolume * m_masterVolume / 100.f);
    sound.play();

    m_nextSoundIndex = (m_nextSoundIndex + 1) % MAX_SIMULTANEOUS_SOUNDS;
}

// Synchronizes the Settings object with the internals of the Audio Manager volume controls.
void AudioManager::SetMasterVolume(float volume)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "SetMasterVolume");

    m_masterVolume = std::clamp(volume, 0.f, 100.f);

    if (m_settings)
    {
        m_settings->m_masterVolume = m_masterVolume;
    }

    // Recalculate final music volume if music is active
    if (m_music)
    {
        m_music->setVolume(m_isMuted ? 0.f : (m_musicVolume * m_masterVolume / 100.f));
    }
}

// Returns the Audio managers current master volume.
float AudioManager::GetMasterVolume() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "GetMasterVolume", 0.f);

    return m_masterVolume;
}

// Synchronizes the Settings object with the internals of the Audio Manager volume controls.
void AudioManager::SetMusicVolume(float volume)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "SetMusicVolume");

    m_musicVolume = std::clamp(volume, 0.f, 100.f);

    if (m_settings)
    {
        m_settings->m_musicVolume = m_musicVolume;
    }

    if (m_music)
    {
        m_music->setVolume(m_isMuted ? 0.f : (m_musicVolume * m_masterVolume / 100.f));
    }
}

// Returns the Audio managers current music volume.
float AudioManager::GetMusicVolume() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "GetMusicVolume", 0.f);
    return m_musicVolume;
}

// Synchronizes the Settings object with the internals of the Audio Manager volume controls.
void AudioManager::SetSFXVolume(float volume)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "SetSFXVolume");

    m_sfxVolume = std::clamp(volume, 0.f, 100.f);

    if (m_settings)
    {
        m_settings->m_sfxVolume = m_sfxVolume;
    }
}

// Returns the Audio managers current sfx volume.
float AudioManager::GetSFXVolume() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "GetSFXVolume", 0.f);

    return m_sfxVolume;
}

// Sets the current volume to zero (mute), synchronizes the Settings object.
void AudioManager::Mute()
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "Mute");

    m_isMuted = true;

    if (m_music)
    {
        m_music->setVolume(0.f);
    }

    if (m_settings)
    {
        m_settings->m_isMuted = true;
    }

    CT_LOG_INFO("AudioManager muted");
}

// Sets the current volume to the masterVolume previously set before mute. Synchronizes the Settings object.
void AudioManager::Unmute()
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "Unmute");

    m_isMuted = false;

    if (m_music)
    {
        m_music->setVolume(m_musicVolume * m_masterVolume / 100.f);
    }

    if (m_settings)
    {
        m_settings->m_isMuted = false;
    }

    CT_LOG_INFO("AudioManager unmuted");
}

// Returns the state of whether or not the audio manager is muted.
bool AudioManager::IsMuted() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "IsMuted", false);

    return m_isMuted;
}

// Adjusts the currently playing sound track to the requested file with optional loop feature.
void AudioManager::SwitchTrack(const std::string &filename, bool loop)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "SwitchTrack");

    if (m_currentTrack == filename)
    {
        CT_LOG_INFO("Requested track '{}' is already playing", filename);

        return;
    }

    StopMusic(true);
    PlayMusic(filename, loop, true);
}

const std::string &AudioManager::GetCurrentMusicName() const
{
    return m_currentTrack;
}
