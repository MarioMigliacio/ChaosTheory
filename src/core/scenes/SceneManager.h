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

class SceneManager
{
  public:
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

    [[nodiscard]] bool IsEmpty() const;
    [[nodiscard]] std::size_t GetSceneCount() const;

  private:
    std::stack<std::unique_ptr<Scene>> m_scenes;
    std::shared_ptr<Settings> m_settings;
    bool m_isInitialized = false;
};
