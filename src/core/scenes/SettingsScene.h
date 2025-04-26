// ============================================================================
//  File        : SettingsScene.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-23
//  Description : Hosts the definition for Settings Scene object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Scene.h"
#include "Settings.h"
#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <memory>

// ============================================================================
//  Class       : SettingsScene
//  Purpose     : Scene that displays the default configurable settings
//                and provides ability to save settings
//
//  Responsibilities:
//      - Initialize background and ui elements
//      - Display a stylized title with interactable
//      - Handle user interaction and route back to main menu
//
// ============================================================================
class SettingsScene : public Scene
{
  public:
    using SceneChangeCallback = std::function<void(std::unique_ptr<Scene>)>;

    SettingsScene(std::shared_ptr<Settings> settings);
    ~SettingsScene() override = default;

    void Init() override;
    void Shutdown() override;
    void OnExit() override;

    void HandleEvent(const sf::Event &event) override;
    void Update(float dt) override;
    void OnResize(const sf::Vector2u &newSize) override;
    void Render() override;

  private:
    void CreateTitleText();

    void CreateSliders();
    void CreateButtons();
    std::shared_ptr<UIElement> MakeSlider(const std::string &label, const sf::Vector2f &position, float initial,
                                          std::function<void(float)> onChange);

  private:
    std::shared_ptr<Settings> m_settings;

    std::unique_ptr<sf::Sprite> m_backgroundSprite;
    sf::Text m_title;
};
