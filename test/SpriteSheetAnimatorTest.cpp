// ============================================================================
//  File        : SpriteSheetAnimatorTest.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-08
//  Description : Unit tests for SpriteSheetAnimator
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SpriteSheetAnimator.h"
#include "LogManager.h"
#include "TestHelpers.h"
#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>

class SpriteSheetAnimatorTest : public ::testing::Test
{
  protected:
    sf::Sprite m_sprite;
    SpriteSheetAnimator m_anim{m_sprite};
    sf::Texture m_dummyTexture;

    void SetUp() override
    {
        if (!LogManager::Instance().IsInitialized())
        {
            LogManager::Instance().Init();
        }

        // Create a dummy texture in memory — 2x2 grid, each frame 16x16 px
        m_dummyTexture.create(32, 32);

        m_anim.SetSheet(m_dummyTexture, 16, 16, 2, 4);
        m_anim.SetFrameDuration(0.05f);
    }
};

TEST_F(SpriteSheetAnimatorTest, PlaysAndAdvancesFrames)
{
    m_anim.Play();
    EXPECT_TRUE(m_anim.IsPlaying());
    EXPECT_EQ(m_anim.GetFrame(), 0);

    // Advance enough to change a frame
    m_anim.Update(0.06f);
    EXPECT_EQ(m_anim.GetFrame(), 1);
}

TEST_F(SpriteSheetAnimatorTest, StopsAtEndWhenNotLooping)
{
    m_anim.SetLooping(false);
    m_anim.SetFrameRange(0, 1);
    m_anim.Play();

    // Enough updates to pass both frames
    m_anim.Update(0.06f);
    m_anim.Update(0.06f);

    EXPECT_TRUE(m_anim.IsFinished());
    EXPECT_FALSE(m_anim.IsPlaying());
}

TEST_F(SpriteSheetAnimatorTest, LoopsWhenLoopingEnabled)
{
    m_anim.SetLooping(true);
    m_anim.SetFrameRange(0, 1);
    m_anim.Play();

    // Two updates cycle back to frame 0
    m_anim.Update(0.06f);
    m_anim.Update(0.06f);

    EXPECT_FALSE(m_anim.IsFinished());
    EXPECT_TRUE(m_anim.IsPlaying());
    EXPECT_EQ(m_anim.GetFrame(), 0);
}
