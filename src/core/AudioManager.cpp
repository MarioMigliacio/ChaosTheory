// ============================================================================
//  File        : AudioManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-12
//  Description : AudioManager is the CT library dedicated manager of
//                sound and music buffers
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "AudioManager.h"
#include "AssetManager.h"
#include "Macros.h"
#include "Settings.h"

namespace
{
/// @brief Defines the max size of the sound effect ring buffer.
const size_t MAX_SIMULTANEOUS_SOUNDS = 16;
} // namespace

/// @brief Get the current Instance for this AudioManager singleton.
/// @return reference to existing AudioManager interface.
AudioManager &AudioManager::Instance()
{
    static AudioManager instance;
    return instance;
}

/// @brief Initializes the SFML audio entities using the provided settings.
/// @param settings Settings to initialize with.
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

/// @brief Provides interface to reload settings and internal variable sync.
/// @param settings Settings to reload with.
void AudioManager::HotReload(std::shared_ptr<Settings> settings)
{
    m_settings = settings;

    m_isMuted = m_settings->m_isMuted;
    SetMasterVolume(m_settings->m_masterVolume);
    SetMusicVolume(m_settings->m_musicVolume);
    SetSFXVolume(m_settings->m_sfxVolume);

    CT_LOG_INFO("AudioManager hot reloaded settings.");
}

/// @brief Shuts down the AudioManager and resets internal state.
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

/// @brief Returns whether or not the AudioManager has been initialized.
/// @return m_isInitialized.
bool AudioManager::IsInitialized() const
{
    return m_isInitialized;
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update.
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

/// @brief Request to begin playing a music file, with optional loop and fade features.
/// @param filename Music file to play.
/// @param loop Whether or not to loop.
/// @param fadeIn IsFadingIn?
/// @param fadeDuration How long to fade.
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

/// @brief Request to halt any playing music file, with optional fade feature.
/// @param fadeOut Opt to slowly diminish volume on stop.
/// @param fadeDuration Duration to fade if fadeOut true.
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

/// @brief Request to pause any playing music file.
void AudioManager::PauseMusic()
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "PauseMusic");

    if (m_music->getStatus() == sf::Music::Playing)
    {
        m_music->pause();
        CT_LOG_INFO("Music paused");
    }
}

/// @brief Request to continue playing any paused music file.
void AudioManager::ResumeMusic()
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "ResumeMusic");

    if (m_music->getStatus() == sf::Music::Paused)
    {
        m_music->play();
        CT_LOG_INFO("Music resumed");
    }
}

/// @brief Return the state of whether any music is currently playing.
/// @return true / false
bool AudioManager::IsMusicPlaying() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "IsMusicPlaying", false);

    return m_music->getStatus() == sf::Music::Playing;
}

/// @brief Return the state of whether any music file is currently in the process of fading out.
/// @return true / false
bool AudioManager::IsFadingOut() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "IsFadingOut", false);

    return m_isFadingOut;
}

/// @brief Return the state of whether any music file is currently in the process of fading in.
/// @return true / false
bool AudioManager::IsFadingIn() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "IsFadingIn", false);

    return m_isFadingIn;
}

/// @brief Request to play a sound effect from a ring buffer of SFML managed sound buffers.
/// @param filename Sound Effect file to add to ring buffer in support of up to 16 concurrent sfx files.
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

/// @brief Synchronizes the Settings object with the internals of the AudioManager volume controls.
/// @param volume new masterVolume Settings to use.
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

/// @brief Returns the AudioManagers current master volume.
/// @return m_masterVolume.
float AudioManager::GetMasterVolume() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "GetMasterVolume", 0.f);

    return m_masterVolume;
}

/// @brief Synchronizes the Settings object with the internals of the AudioManager volume controls.
/// @param volume new m_musicVolume.
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

/// @brief Returns the AudioManagers current music volume.
/// @return m_musicVolume.
float AudioManager::GetMusicVolume() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "GetMusicVolume", 0.f);
    return m_musicVolume;
}

/// @brief Synchronizes the Settings object with the internals of the AudioManager volume controls.
/// @param volume new m_sfxVolume.
void AudioManager::SetSFXVolume(float volume)
{
    CT_WARN_IF_UNINITIALIZED("AudioManager", "SetSFXVolume");

    m_sfxVolume = std::clamp(volume, 0.f, 100.f);

    if (m_settings)
    {
        m_settings->m_sfxVolume = m_sfxVolume;
    }
}

/// @brief Returns the AudioManagers current sfx volume.
/// @return m_sfxVolume.
float AudioManager::GetSFXVolume() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "GetSFXVolume", 0.f);

    return m_sfxVolume;
}

/// @brief Sets the current volume to zero (mute), synchronizes the Settings object.
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

/// @brief Sets the current volume to the musicVolume previously set before mute. Synchronizes the Settings object.
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

/// @brief Returns the state of whether or not the AudioManager is muted.
/// @return true / false
bool AudioManager::IsMuted() const
{
    CT_WARN_IF_UNINITIALIZED_RET("AudioManager", "IsMuted", false);

    return m_isMuted;
}

/// @brief Adjusts the currently playing sound track to the requested file with optional loop feature.
/// @param filename new File track to attempt to play.
/// @param loop whether or not to loop the file.
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

/// @brief Returns the current music string name.
/// @return m_currentTrack.
const std::string &AudioManager::GetCurrentMusicName() const
{
    return m_currentTrack;
}
