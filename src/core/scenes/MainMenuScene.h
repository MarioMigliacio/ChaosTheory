// ============================================================================
//  File        : MainMenuScene.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-13
//  Description : Hosts the definitions for Main Menu Scene Object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Scene.h"
#include "Settings.h"
#include <memory>

// ============================================================================
//  Class       : MainMenuScene
//  Purpose     : Leaf node class inheriting interface to define a Scene.
//                Main menu logic upheld.
//  Responsibilities:
//      - Initializes and shuts down
//      - OnExit to call scene specific logic attributes
//      - Init to accept input and event logic
//      - Handles the render loop and time delta
//
// ============================================================================
class MainMenuScene final : public Scene
{
  public:
    using SceneChangeCallback = std::function<void(std::unique_ptr<Scene>)>;

    MainMenuScene(std::shared_ptr<Settings> settings);
    ~MainMenuScene() = default;

    MainMenuScene(const MainMenuScene &) = delete;
    MainMenuScene &operator=(const MainMenuScene &) = delete;

    void Init() override;
    void Shutdown() override;
    void OnExit() override;

    bool IsInitialized() override;

    void Update(float dt) override;
    void HandleEvent(const sf::Event &event) override;
    void Render() override;

    void SetSceneChangeCallback(SceneChangeCallback callback);

    bool ShouldExit() override;

  private:
    std::shared_ptr<Settings> m_settings;
    SceneChangeCallback m_sceneChangeCallback;
};
