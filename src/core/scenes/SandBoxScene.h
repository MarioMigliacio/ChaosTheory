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
#include "BaseShip.h"
#include "DialogConstants.h"
#include "DialogLine.h"
#include "DialogQueue.h"
#include "Macros.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Settings.h"
#include "UIPresets.h"
#include <memory>

// ============================================================================
//  Class       : SandBoxScene
//  Purpose     : Leaf node class inheriting interface to define a Scene.
//                Game scene logic tested.
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
    void LoadBackground();
    void BindInputKeys();
    void CheckActionsPressed();
    void StartNextDialog();

    void SetupSceneComponents();
    void MockTitleText(const bool enabled);
    void MockHUDPanel(const bool enabled);
    void MockFillableGaugeComponents(const bool enabled);
    void MockShipStatusComponent(const bool enabled);
    void MockIconComponents(const bool enabled);
    void MockChatBox(const bool enabled);

    void MockUnitSpawns(const bool enabled);
    void UpdateMockUnits(float dt, const bool enabled);
    void DrawMockUnits(sf::RenderTarget &target, const bool enabled);

    void PlayGameMusic();
    void UpdateHUD(float dt, const bool enabled);

  private:
    std::shared_ptr<Settings> m_settings;
    std::shared_ptr<UITextLabel> m_titleLabel;
    std::shared_ptr<UITextLabel> m_helpLabel;
    std::shared_ptr<UIChatBox> m_testChatBox;
    std::unique_ptr<Background> m_background;
    std::shared_ptr<UITextLabel> m_scoreLabel;
    std::shared_ptr<UITextLabel> m_timerLabel;
    std::shared_ptr<UIFillableGauge> m_healthGauge;
    std::vector<std::shared_ptr<BaseShip>> m_mockShips;

    DialogQueue m_dialogQueue;

    // Simple test variables for the time being. TODO: Cleanup this to the proper structure later.
    float m_elapsedTime = 0.f;
    int m_secondsPassed = 0;
    int m_currentHealth = HUD_HEALTH_LABEL_START_VALUE;
    int m_currentScore = HUD_SCORE_LABEL_START_VALUE;

    SceneID m_requestedScene;
};
