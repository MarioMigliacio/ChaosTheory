// ============================================================================
//  File        : SpriteSheetAnimator.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-07
//  Description : Lightweight animator for grid-based sprite sheets
//                (e.g., 512x512 sheet of 4x4 frames of 128x128).
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SpriteSheetAnimator.h"

/// @brief Constructs a sprite sheet animator bound to a target sprite.
/// @param sprite Reference to the sf::Sprite that will be updated per frame.
SpriteSheetAnimator::SpriteSheetAnimator(sf::Sprite &sprite) : m_sprite(sprite)
{
}

/// @brief Sets a sprite sheet to this animator.
/// @param texture Sprite sheet texture.
/// @param frameWidth Width of each animation frame in pixels.
/// @param frameHeight Height of each animation frame in pixels.
/// @param framesPerRow Number of frames per horizontal row in the sheet.
/// @param totalFrames Total number of frames in the sheet.
void SpriteSheetAnimator::SetSheet(const sf::Texture &texture, int frameWidth, int frameHeight, int framesPerRow,
                                   int totalFrames)
{
    m_texture = &texture;
    m_frameWidth = frameWidth;
    m_frameHeight = frameHeight;
    m_framesPerRow = framesPerRow;
    m_totalFrames = totalFrames;

    m_firstFrame = 0;
    m_lastFrame = totalFrames - 1;
    m_current = 0;
    m_finished = false;

    m_sprite.setTexture(*m_texture);
    ApplyFrameToSprite(m_current);
}

/// @brief Sets the time each frame is displayed before advancing.
/// @param secondsPerFrame Duration of each frame in seconds.
void SpriteSheetAnimator::SetFrameDuration(float secondsPerFrame)
{
    m_secondsPerFrame = secondsPerFrame;
}

/// @brief Enables or disables looping playback.
/// @param loop True to loop when reaching the end of the frame range.
void SpriteSheetAnimator::SetLooping(bool loop)
{
    m_loop = loop;
}

/// @brief Begins playing the animation from the current frame.s
void SpriteSheetAnimator::Play()
{
    m_playing = true;
    m_finished = false;
}

/// @brief Pauses the animation without resetting it.
void SpriteSheetAnimator::Pause()
{
    m_playing = false;
}

/// @brief Stops the animation and resets to the first frame.
void SpriteSheetAnimator::Stop()
{
    m_playing = false;
    Reset();
}

/// @brief Resets the animation state to the first frame in range.
void SpriteSheetAnimator::Reset()
{
    m_timer = 0.f;
    m_finished = false;
    m_current = (m_customOrder.empty() ? m_firstFrame : m_customOrder.front());
    ApplyFrameToSprite(m_current);
}

/// @brief Sets the inclusive frame range to play.
/// @param firstFrame First frame index in the range.
/// @param lastFrame  Last frame index in the range.
void SpriteSheetAnimator::SetFrameRange(int firstFrame, int lastFrame)
{
    assert(firstFrame >= 0 && lastFrame >= firstFrame && lastFrame < m_totalFrames);

    m_firstFrame = firstFrame;
    m_lastFrame = lastFrame;
    m_customOrder.clear();
    m_current = m_firstFrame;
    ApplyFrameToSprite(m_current);
}

/// @brief Defines a custom playback order for frames.
/// @param order Vector of frame indices in the desired order.
void SpriteSheetAnimator::SetFrameOrder(const std::vector<int> &order)
{
    for (int f : order)
    {
        assert(f >= 0 && f < m_totalFrames);
    }

    m_customOrder = order;

    if (!m_customOrder.empty())
    {
        m_firstFrame = m_customOrder.front();
        m_lastFrame = m_customOrder.back();
        m_current = m_firstFrame;
        ApplyFrameToSprite(m_current);
    }
}

/// @brief Jumps directly to a specific frame.
/// @param frameIndex Frame index to display.
void SpriteSheetAnimator::SetFrame(int frameIndex)
{
    assert(frameIndex >= 0 && frameIndex < m_totalFrames);

    m_current = frameIndex;
    ApplyFrameToSprite(m_current);
}

/// @brief Gets the current frame index.
/// @return Current frame index.
int SpriteSheetAnimator::GetFrame() const
{
    return m_current;
}

/// @brief Returns true if animation is currently playing.
bool SpriteSheetAnimator::IsPlaying() const
{
    return m_playing;
}
/// @brief Returns true if the animation has reached its end in non-looping mode.
bool SpriteSheetAnimator::IsFinished() const
{
    return m_finished;
}

/// @brief Advances the animation by the given delta time.
/// @param dt Elapsed time in seconds since the last update.
void SpriteSheetAnimator::Update(float dt)
{
    if (!m_playing || m_finished || m_texture == nullptr)
    {
        return;
    }

    m_timer += dt;

    if (m_timer >= m_secondsPerFrame)
    {
        m_timer = 0.f;

        int next = NextFrame(m_current);

        if (next == m_current)
        {
            m_finished = true;
            m_playing = false;

            return;
        }

        m_current = next;
        ApplyFrameToSprite(m_current);
    }
}

/// @brief Returns the next index to the frame, provided the current frame.
/// @param current Which index represents the current frame.
/// @return Next frame in line if exists.
int SpriteSheetAnimator::NextFrame(int current) const
{
    if (!m_customOrder.empty())
    {
        for (size_t i = 0; i < m_customOrder.size(); ++i)
        {
            if (m_customOrder[i] == current)
            {
                size_t nextIdx = i + 1;
                if (nextIdx < m_customOrder.size())
                {
                    return m_customOrder[nextIdx];
                }

                return m_loop ? m_customOrder.front() : current;
            }
        }

        return m_customOrder.empty() ? current : m_customOrder.front();
    }

    int next = current + 1;

    if (next <= m_lastFrame)
    {
        return next;
    }

    return m_loop ? m_firstFrame : current;
}

/// @brief Traverse the dimensions of the frames to convert given index to proper rect identifier.
/// @param frameIndex Index to convert and apply rect to frame.
void SpriteSheetAnimator::ApplyFrameToSprite(int frameIndex)
{
    // Convert frameIndex to (x, y) in sheet
    const int col = frameIndex % m_framesPerRow;
    const int row = frameIndex / m_framesPerRow;

    const sf::IntRect rect(col * m_frameWidth, row * m_frameHeight, m_frameWidth, m_frameHeight);
    m_sprite.setTextureRect(rect);
}
