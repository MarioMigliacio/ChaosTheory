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
#include "SceneManager.h"
#include "Settings.h"
#include "UIFactory.h"
#include "UITextLabel.h"
#include <SFML/Graphics.hpp>
#include <memory>

// ============================================================================
//  Class       : MainMenuScene
//  Purpose     : Scene that displays the interactive main menu.
//                Contains background, title, and Play/Exit buttons.
//
//  Responsibilities:
//      - Initialize background and music
//      - Display a stylized title and interactable buttons
//      - Handle user interaction and route scene transitions
//      - Scale dynamically with window resize
//
// ============================================================================
class MainMenuScene final : public Scene
{
  public:
    MainMenuScene(std::shared_ptr<Settings> settings);
    ~MainMenuScene() = default;

    MainMenuScene(const MainMenuScene &) = delete;
    MainMenuScene &operator=(const MainMenuScene &) = delete;

    void Init() override;
    void LoadRequiredAssets() override;
    void Shutdown() override;
    void OnExit() override;

    void Update(float dt) override;
    void HandleEvent(const sf::Event &event) override;
    void OnResize(const sf::Vector2u &newSize) override;
    void Render() override;

  private:
    // Setup helpers
    void SetupSceneComponents();
    void CreateTitleText();
    void CreateButtons();
    void LoadBackground();
    void PlayIntroMusic();

    std::shared_ptr<UIElement> MakeMenuButton(ButtonType type, const sf::Vector2f &pos, const std::string &label,
                                              std::function<void()> onClick);

  private:
    std::shared_ptr<Settings> m_settings;

    std::unique_ptr<sf::Sprite> m_backgroundSprite;
    std::shared_ptr<UITextLabel> m_titleLabel;
    SceneID m_requestedScene = SceneID::Splash;
};
