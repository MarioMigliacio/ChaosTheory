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

// ============================================================================
//  Class       : GameScene
//  Purpose     : Leaf node class inheriting interface to define a Scene.
//                Game scene logic upheld.
//
//  Responsibilities:
//      - Initializes and shuts down
//      - OnExit to call scene specific logic attributes
//      - Init to accept input and event logic
//      - Handles the render loop and time delta
//      - TODO, make a more clean sandbox in the future.
//
// ============================================================================
class GameScene final : public Scene
{
  public:
    GameScene(std::shared_ptr<Settings> settings);
    ~GameScene() override = default;

    GameScene(const GameScene &) = delete;
    GameScene &operator=(const GameScene &) = delete;

    void Init() override;
    void LoadRequiredAssets() override;
    void Shutdown() override;
    void OnExit() override;

    void Update(float dt) override;
    void HandleEvent(const sf::Event &event) override;
    void OnResize(const sf::Vector2u &newSize) override;
    void Render() override;

  private:
    std::shared_ptr<Settings> m_settings;
};
