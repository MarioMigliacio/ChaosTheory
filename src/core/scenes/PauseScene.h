// ============================================================================
//  File        : PauseScene.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-09
//  Description : Hosts the definitions for Pause Scene Object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once
#include "SceneManager.h"
#include "Settings.h"
#include <memory>

// ============================================================================
//  Class       : PauseScene
//  Purpose     : Scene that displays over top of the Current Game Scene.
//                Provides options and halts game sounds and updates.
//
//  Responsibilities:
//      - Provides Menu options
//      - Able to adjust Settings
//      - Handle user interaction and route scene transitions
//
// ============================================================================
class PauseScene : public Scene
{
  public:
    PauseScene(std::shared_ptr<Settings> settings);
    ~PauseScene() = default;

    void Init() override;
    void LoadRequiredAssets() override;
    void Shutdown() override;
    void OnExit() override;
    void OnResume() override;

    void Update(float dt) override;
    void HandleEvent(const sf::Event &event) override;
    void OnResize(const sf::Vector2u &newSize) override;
    void Render() override;

  private:
    void SetupUI();
    void PauseAudio();

  private:
    std::shared_ptr<Settings> m_settings;

    SceneID m_requestedScene;
};
