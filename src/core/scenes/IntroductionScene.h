// ============================================================================
//  File        : IntroductionScene.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-13
//  Description : Scene for intro cinematic with formation and dialog
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Background.h"
#include "DialogQueue.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Settings.h"
#include "UIChatBox.h"
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>

/// @brief Data structure to hold key configurables that aid in setting up ship enter and exit positions.
struct FormationShip
{
    /// @brief Key to the sprite texture to be used by this FormationShip.
    sf::Sprite sprite;

    /// @brief Velocity aspect of how fast this FormationShip will travel during scene.
    sf::Vector2f velocity;

    /// @brief Vector location where this FormationShip is bound to Enter to.
    sf::Vector2f targetPosition;

    /// @brief Vector location where this FormationShip is bound to Exit to.
    sf::Vector2f exitPosition;

    /// @brief Has this FormationShip arrived to its targetPosition, or later; exit position.
    bool arrived = false;
};

/// @brief Simple enumeration for supported Phases of the IntroductionScene.
enum class IntroPhase
{
    /// @brief Initial phase pertaining to ships entering the scene.
    ShipEntry,

    /// @brief Dialog phase after the ships have entered scene position.
    Dialog,

    /// @brief Post dialog phase, where ships begin to leave the scene bounds.
    ShipExit,

    /// @brief Scene flag indicating that the IntroductionScene should be concluded.
    Done
};

// ============================================================================
//  Class       : IntroductionScene
//  Purpose     : Leaf node class inheriting interface to define a Scene.
//                Cutscene to preceed game playing.
//
//  Responsibilities:
//      - Initializes and shuts down
//      - OnExit to call scene specific logic attributes
//      - Init to accept input and event logic
//      - Handles the render loop and time delta
//
// ============================================================================
class IntroductionScene : public Scene
{
  public:
    IntroductionScene(std::shared_ptr<Settings> settings);
    ~IntroductionScene() override = default;

    IntroductionScene(const IntroductionScene &) = delete;
    IntroductionScene &operator=(const IntroductionScene &) = delete;

    void Init() override;
    void LoadRequiredAssets() override;
    void Shutdown() override;
    void OnExit() override;
    void Update(float dt) override;
    void HandleEvent(const sf::Event &event) override;
    void OnResize(const sf::Vector2u &newSize) override;
    void Render() override;

  private:
    void CheckActionsPressed();
    void LoadBackground();
    void BindInputKeys();
    void InitializeDialogContexts();
    void AdvanceChatBox();
    void StartNextDialog();
    void SetupFormation();
    void UpdateFormationEntry(float dt);
    void UpdateShipExit(float dt);

  private:
    std::shared_ptr<Settings> m_settings;
    std::unique_ptr<Background> m_background;
    std::shared_ptr<UIChatBox> m_chatBox;

    std::vector<FormationShip> m_ships;
    DialogQueue m_dialogQueue;
    bool m_dialogStarted = false;

    SceneID m_requestedScene;
    IntroPhase m_phase = IntroPhase::ShipEntry;
};
