// ============================================================================
//  File        : SceneManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-13
//  Description : Scene Manager is the CT library dedicated manager of
//                game transitional states
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Scene.h"
#include "Settings.h"
#include <memory>
#include <stack>

// ============================================================================
//  Class       : SceneManager
//  Purpose     : Singleton class that manages the Scene Abstract base class.
//
//  Responsibilities:
//      - Initializes and shuts down
//      - Stores Scene transition logic, and update management
//      - TODO: handle factory logic of scenes and transition logic updates
//
// ============================================================================
class SceneManager
{
  public:
    SceneManager() = default;
    explicit SceneManager(std::shared_ptr<Settings> settings);
    ~SceneManager() = default;

    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;

    void Init();
    void Shutdown();

    void Update(float dt);
    void HandleEvent(const sf::Event &event);
    void Render();

    void PushScene(std::unique_ptr<Scene> scene);
    void PopScene();
    void ClearScenes();

    bool IsEmpty() const;
    std::size_t GetSceneCount() const;
    Scene *GetActiveScene() const;
    bool IsInitialized() const;

  private:
    std::stack<std::unique_ptr<Scene>> m_scenes;
    std::shared_ptr<Settings> m_settings;
    bool m_isInitialized = false;
};
