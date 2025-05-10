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

// ============================================================================
//  Class       : Scene
//  Purpose     : Abstract base class with the interface to define a Scene.
//                Scene implies a level in a game world, menu, or art screen.
//
//  Responsibilities:
//      - Initializes and shuts down
//      - OnExit to call scene specific logic attributes
//      - Init to accept input and event logic
//      - Handles the render loop and time delta
//
// ============================================================================
class Scene
{
  public:
    Scene() = default;
    virtual ~Scene() = default;

    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual void LoadRequiredAssets() = 0;

    virtual void OnExit() = 0;

    virtual void Update(float dt) = 0;
    virtual void HandleEvent(const sf::Event &event) = 0;
    virtual void OnResize(const sf::Vector2u &newSize) = 0;
    virtual void Render() = 0;

    // Common
  public:
    virtual bool IsInitialized()
    {
        return m_isInitialized;
    };

  protected:
    bool m_shouldExit = false;
    bool m_isInitialized = false;
    bool m_hasPendingTransition = false;
};
