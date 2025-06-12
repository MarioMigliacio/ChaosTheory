// ============================================================================
//  File        : SandBoxScene.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-28
//  Description : Hosts the definitions for SandBox Scene Object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Background.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Settings.h"
#include "UITextLabel.h"
#include <memory>

// ============================================================================
//  Class       : SandBoxScene
//  Purpose     : Leaf node class inheriting interface to define a Scene.
//                Game scene logic upheld.
//
//  Responsibilities:
//      - Initializes and shuts down
//      - OnExit to call scene specific logic attributes
//      - Init to accept input and event logic
//      - Handles the render loop and time delta
//
// ============================================================================
class SandBoxScene final : public Scene
{
  public:
    SandBoxScene(std::shared_ptr<Settings> settings);
    ~SandBoxScene() override = default;

    SandBoxScene(const SandBoxScene &) = delete;
    SandBoxScene &operator=(const SandBoxScene &) = delete;

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
    void SetupSceneComponents();
    void LoadBackground();
    void CreateTitleText();
    void PlayGameMusic();
    void BindInputKeys();
    void CheckActionsPressed();

  private:
    std::shared_ptr<Settings> m_settings;
    std::shared_ptr<UITextLabel> m_titleLabel;
    std::shared_ptr<UITextLabel> m_helpLabel;
    std::unique_ptr<Background> m_background;
    SceneID m_requestedScene;
};
