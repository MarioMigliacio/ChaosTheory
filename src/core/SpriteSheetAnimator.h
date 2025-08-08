// ============================================================================
//  File        : SpriteSheetAnimator.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-07
//  Description : Lightweight animator for grid-based sprite sheets
//                (e.g., 512x512 sheet of 4x4 frames of 128x128).
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include <cassert>
#include <vector>

// ============================================================================
//  Class       : SpriteSheetAnimator
//  Purpose     : SpriteSheetAnimator class partitions 2D spritesheet into
//                frames. Provides a means of creating an animation effect by
//                managing transition sprite rects in a sheet with looping,
//                or frame delta definitions.
//
//  Responsibilities:
//      - Update a sprite by looping or transitioning through several rect boxes
//        defined by a single spritesheet.
//
// ============================================================================
class SpriteSheetAnimator
{
  public:
    SpriteSheetAnimator(sf::Sprite &sprite);
    ~SpriteSheetAnimator() = default;

    void SetSheet(const sf::Texture &texture, int frameWidth, int frameHeight, int framesPerRow, int totalFrames);

    void SetFrameDuration(float secondsPerFrame);
    void SetLooping(bool loop);
    void Play();
    void Pause();
    void Stop();
    void Reset();

    void SetFrameRange(int firstFrame, int lastFrame);
    void SetFrameOrder(const std::vector<int> &order);
    void SetFrame(int frameIndex);
    int GetFrame() const;

    bool IsPlaying() const;
    bool IsFinished() const;

    void Update(float dt);

  private:
    void ApplyFrameToSprite(int frameIndex);
    int NextFrame(int current) const;

  private:
    sf::Sprite &m_sprite;
    const sf::Texture *m_texture = nullptr;
    std::vector<int> m_customOrder;

    int m_frameWidth = 0;
    int m_frameHeight = 0;
    int m_framesPerRow = 0;
    int m_totalFrames = 0;

    float m_secondsPerFrame = 0.05f;
    float m_timer = 0.f;
    bool m_loop = false;
    bool m_playing = false;
    bool m_finished = false;

    int m_firstFrame = 0;
    int m_lastFrame = 0;
    int m_current = 0;
};
