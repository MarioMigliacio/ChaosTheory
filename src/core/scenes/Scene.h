// ============================================================================
//  File        : Scene.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-13
//  Description : Hosts the definition for scene architecture and logic
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <SFML/Window/Event.hpp>
#include <functional>

class Scene
{
  public:
    Scene() = default;
    virtual ~Scene() = default;

    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    virtual void Init() = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void OnExit() = 0;
    virtual void Shutdown() = 0;

  public:
    virtual bool ShouldExit() const
    {
        return m_shouldExit;
    }
    virtual bool IsInitialized() const
    {
        return m_isInitialized;
    }
    virtual void HandleEvent(const sf::Event &event)
    {
    }

  protected:
    bool m_shouldExit = false;
    bool m_isInitialized = false;
};
