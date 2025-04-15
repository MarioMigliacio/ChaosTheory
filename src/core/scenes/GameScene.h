// ============================================================================
//  File        : GameScene.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-14
//  Description : Hosts the definitions for Game Scene Object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Scene.h"
#include "Settings.h"
#include <memory>

// Signals that the class is a leaf class
class GameScene final : public Scene
{
  public:
    using SceneChangeCallback = std::function<void(std::unique_ptr<Scene>)>;

    GameScene(std::shared_ptr<Settings> settings);
    ~GameScene() override = default;

    GameScene(const GameScene &) = delete;
    GameScene &operator=(const GameScene &) = delete;

    void Init() override;
    void Update(float dt) override;
    void HandleEvent(const sf::Event &event);
    void Render() override;
    void OnExit() override;
    void Shutdown() override;

    void SetSceneChangeCallback(SceneChangeCallback callback);

  private:
    std::shared_ptr<Settings> m_settings;
    SceneChangeCallback m_sceneChangeCallback;
    bool m_isInitialized = false;
};
