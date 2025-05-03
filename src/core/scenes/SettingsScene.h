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
#include "SceneManager.h"
#include "Settings.h"
#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <string>

enum class SettingsPage
{
    Audio,
    Video,
    KeyBindings
};

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
    explicit SettingsScene(std::shared_ptr<Settings> settings);

    void Init() override;
    void Shutdown() override;
    void OnExit() override;

    void Update(float dt) override;
    void HandleEvent(const sf::Event &event) override;
    void OnResize(const sf::Vector2u &newSize) override;
    void Render() override;

  private:
    void CreateSettingsPage(SettingsPage page);
    void CreateUI(SettingsPage page);
    void CreateTitle(SettingsPage page);
    void CreateArrows(SettingsPage page);
    void CreateButtons();

    void LoadDefaultSFXFile();

    void CheckForUnsavedChanges();
    void ShowToast(const std::string &message);

    void SwitchToPage(SettingsPage page);

  private:
    std::shared_ptr<Settings> m_settings;
    Settings m_backupSettings;

    SettingsPage m_currentPage = SettingsPage::Audio;
    std::optional<SettingsPage> m_pendingPageChange;

    std::shared_ptr<UIElement> m_applyButton;
    SceneID m_requestedScene = SceneID::MainMenu;

    bool m_hasUnsavedChanges = false;
    bool m_showToast = false;
    float m_toastTimer = 0.f;

    sf::Text m_toastText;
    sf::Text m_title;
};
