// ============================================================================
//  File        : TestHelpers.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : A Settings object used for internal testing
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseCollidable.h"
#include "Settings.h"
#include <SFML/Graphics.hpp>
#include <memory>

// ----------------------------- Settings Helper ------------------------------
inline std::shared_ptr<Settings> CreateTestSettings()
{
    auto settings = std::make_shared<Settings>();
    settings->m_resolution = ResolutionSetting::Res720p;
    settings->m_gameDifficulty = GameDifficultySetting::Normal;
    settings->m_windowWidth = 1280;
    settings->m_windowHeight = 720;
    settings->m_targetFramerate = 60;
    settings->m_verticleSyncEnabled = true;
    settings->m_isFullscreen = false;
    settings->m_windowTitle = "Test Window";
    settings->m_audioDirectory = "assets/audio/";
    settings->m_masterVolume = 50.0f;
    settings->m_musicVolume = 50.0f;
    settings->m_sfxVolume = 50.0f;
    settings->m_isMuted = false;
    return settings;
}

// ------------------------- Dummy Collidable Helpers -------------------------
// A lightweight enemy collidable for collision-trigger tests.
// Use to provoke overlaps with projectiles without pulling in full ship types.
//
// Example:
//   auto enemy = std::make_shared<DummyEnemyCollidable>(sf::FloatRect{198,150,32,32});
//
class DummyEnemyCollidable final : public BaseCollidable
{
  public:
    explicit DummyEnemyCollidable(const sf::FloatRect &rect) : m_rect(rect)
    {
    }

    // --- BaseCollidable overrides ---
    sf::FloatRect GetBounds() const override
    {
        return m_rect;
    }
    bool IsAlive() const override
    {
        return m_alive;
    }
    CollisionCategory GetCollisionCategory() const override
    {
        return CollisionCategory::Enemy;
    }

    // --- Test convenience methods ---
    void SetPos(const sf::Vector2f &p)
    {
        m_rect.left = p.x;
        m_rect.top = p.y;
    }

    void SetSize(const sf::Vector2f &s)
    {
        m_rect.width = s.x;
        m_rect.height = s.y;
    }

    void SetRect(const sf::FloatRect &r)
    {
        m_rect = r;
    }

    void Kill()
    {
        m_alive = false;
    }

    void Revive()
    {
        m_alive = true;
    }

  private:
    sf::FloatRect m_rect;
    bool m_alive{true};
};