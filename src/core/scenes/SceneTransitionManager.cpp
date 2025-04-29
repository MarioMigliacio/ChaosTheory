// ============================================================================
//  File        : SceneTransitionManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-26
//  Description : Hosts the definition for Scene Transition Managemer
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SceneTransitionManager.h"

SceneTransitionManager &SceneTransitionManager::Instance()
{
    static SceneTransitionManager instance;
    return instance;
}

void SceneTransitionManager::Reset()
{
    m_isFadingOut = false;
    m_isFadingIn = false;
    m_fadeComplete = true;
    m_pendingFadeIn = false;
    m_opacity = 0.f;
}

void SceneTransitionManager::StartFadeOut(float duration)
{
    m_isFadingOut = true;
    m_isFadingIn = false;
    m_fadeComplete = false;
    m_opacity = 0.f;
    m_fadeSpeed = 255.f / duration;

    m_fadeRectangle.setSize({800.f, 600.f}); // Will be resized dynamically to window size
    m_fadeRectangle.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_opacity)));
}

void SceneTransitionManager::StartFadeIn(float duration)
{
    m_isFadingOut = false;
    m_isFadingIn = true;
    m_fadeComplete = false;
    m_opacity = 255.f;
    m_fadeSpeed = 255.f / duration;

    m_fadeRectangle.setSize({800.f, 600.f});
    m_fadeRectangle.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_opacity)));
}

void SceneTransitionManager::Update(float dt)
{
    if (m_isFadingOut)
    {
        m_opacity += m_fadeSpeed * dt;

        if (m_opacity >= 255.f)
        {
            m_opacity = 255.f;
            m_fadeComplete = true;
            m_isFadingOut = false;
        }
    }

    else if (m_isFadingIn)
    {
        m_opacity -= m_fadeSpeed * dt;

        if (m_opacity <= 0.f)
        {
            m_opacity = 0.f;
            m_fadeComplete = true;
            m_isFadingIn = false;
        }
    }

    m_fadeRectangle.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_opacity)));
}

void SceneTransitionManager::Render(sf::RenderWindow &window)
{
    if (m_isFadingOut || m_isFadingIn || m_pendingFadeIn)
    {
        m_fadeRectangle.setSize(sf::Vector2f(window.getSize()));
        window.draw(m_fadeRectangle);

        // Start Fade In after one frame when pending
        if (m_pendingFadeIn)
        {
            StartFadeIn(1.0f);
            m_pendingFadeIn = false;
        }
    }
}

bool SceneTransitionManager::IsFading() const
{
    return m_isFadingOut || m_isFadingIn;
}

bool SceneTransitionManager::IsFadeComplete() const
{
    return m_fadeComplete;
}

void SceneTransitionManager::ForceFullyOpaque()
{
    m_opacity = 255.f;
    m_fadeRectangle.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_opacity)));
    m_isFadingOut = false;
    m_isFadingIn = false;
    m_fadeComplete = true;
}
