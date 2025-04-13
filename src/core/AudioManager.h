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

class AudioManager
{
  public:
    static AudioManager &Instance();

    void Init(std::shared_ptr<Settings> settings); // non-const - May adjust settings
    void Shutdown();

    void PlayMusic(const std::string &filename, bool loop = true);
    void PauseMusic();
    void ResumeMusic();
    void StopMusic();
    bool IsMusicPlaying() const;

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

    std::string m_currentTrack;

    float m_volume = 100.0f;
    bool m_muted = false;

    std::unique_ptr<sf::Music> m_music;
    std::shared_ptr<Settings> m_settings; // non-const - May adjust settings
};
