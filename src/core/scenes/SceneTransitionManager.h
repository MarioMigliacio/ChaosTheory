// ============================================================================
//  File        : SceneTransitionManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-26
//  Description : Hosts the definition for Scene Transition Managemer
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <SFML/Graphics.hpp>

// ============================================================================
//  Class       : SceneTransitionManager
//  Purpose     : Singleton class that manages the fade in and opacity for
//                scene transition callbacks.
//
//  Responsibilities:
//      - Always exists, no need for Init or Shutdown.
//      - Provides interface to Fade screen.
//      - Stores Scene transition logic, and update management.
//
// ============================================================================
class SceneTransitionManager
{
  public:
    static SceneTransitionManager &Instance();

    void Reset();

    void StartFadeOut(float duration = 1.0f);
    void StartFadeIn(float duration = 1.0f);

    void Update(float dt);
    void Render(sf::RenderWindow &window);

    bool IsFading() const;
    bool IsFadeComplete() const;

    void ForceFullyOpaque();

  private:
    SceneTransitionManager() = default;
    ~SceneTransitionManager() = default;

    SceneTransitionManager(const SceneTransitionManager &) = delete;
    SceneTransitionManager &operator=(const SceneTransitionManager &) = delete;

  private:
    sf::RectangleShape m_fadeRectangle;

    bool m_isFadingOut = false;
    bool m_isFadingIn = false;
    bool m_fadeComplete = false;
    bool m_pendingFadeIn = false;
    float m_opacity = 0.f;
    float m_fadeSpeed = 0.f;
};
