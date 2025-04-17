// ============================================================================
//  File        : AudioManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-12
//  Description : Audio Manager is the CT library dedicated manager of
//                sound and music buffers
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Settings.h"
#include <SFML/Audio.hpp>
#include <memory>
#include <string>

// ============================================================================
//  Class       : AudioManager
//  Purpose     : Singleton class that manages the SFML Audio.
//
//  Responsibilities:
//      - Initializes and shuts down
//      - Returns Music, volumes, and mute states. More in the future
//
// ============================================================================
class AudioManager
{
  public:
    static AudioManager &Instance();

    void Init(std::shared_ptr<Settings> settings);
    void Shutdown();

    bool IsInitialized() const;
    void Update(float dt);

    void PlayMusic(const std::string &filename, bool loop = true, bool fadeIn = false, float fadeDuration = 1.0f);
    void StopMusic(bool fadeOut = false, float fadeDuration = 2.0f);
    void PauseMusic();
    void ResumeMusic();
    bool IsMusicPlaying() const;
    bool IsFadingOut() const;

    void SetVolume(float volume);
    float GetVolume() const;
    void Mute();
    void Unmute();
    bool IsMuted() const;

    void SwitchTrack(const std::string &filename, bool loop = true);

  private:
    AudioManager() = default;
    ~AudioManager() = default;

    AudioManager(const AudioManager &) = delete;
    AudioManager &operator=(const AudioManager &) = delete;

  private:
    std::unique_ptr<sf::Music> m_music;
    std::shared_ptr<Settings> m_settings;
    std::string m_currentTrack;

    float m_masterVolume = 100.0f;
    float m_musicVolume = 100.0f;

    bool m_isMuted = false;

    bool m_isFadingOut = false;
    float m_fadeOutTimer = 0.0f;
    float m_fadeOutDuration = 0.0f;

    bool m_isFadingIn = false;
    float m_fadeInTimer = 0.0f;
    float m_fadeInDuration = 0.0f;

    bool m_isInitialized = false;
};
