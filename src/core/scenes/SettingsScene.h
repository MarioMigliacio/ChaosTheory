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

#include "Background.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Settings.h"
#include "UIElement.h"
#include "UITextLabel.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <optional>
#include <string>

/// @brief Simple enumeration for supported SettingsPage types.
enum class SettingsPage
{
    Audio,
    Video,
    Difficulty
};

// ============================================================================
//  Class       : SettingsScene
//  Purpose     : Scene that displays the default configurable settings
//                and provides ability to save settings.
//
//  Responsibilities:
//      - Initialize background and ui elements
//      - Display a stylized title with interactable ui elements
//      - Handle user interaction and route back to main menu
//
// ============================================================================
class SettingsScene : public Scene
{
  public:
    using ReturnCallback = std::function<void()>;

    SettingsScene(std::shared_ptr<Settings> settings, bool includeDifficulty = true, ReturnCallback onReturn = nullptr);
    ~SettingsScene() = default;

    SettingsScene(const SettingsScene &) = delete;
    SettingsScene &operator=(const SettingsScene &) = delete;

    void Init() override;
    void LoadRequiredAssets() override;
    void Shutdown() override;
    void OnExit() override;

    void Update(float dt) override;
    void HandleEvent(const sf::Event &event) override;
    void OnResize(const sf::Vector2u &newSize) override;
    void Render() override;

  private:
    void SetupSceneComponents(SettingsPage page);
    void CreateUI(SettingsPage page);
    void CreateTitleText();
    void LoadBackground();
    void CreateArrows(SettingsPage page);

    void CreateButtonControls();
    void CreateAudioControls();
    void CreateResolutionControls();
    void CreateDifficultyControls();

    void CheckForUnsavedChanges();
    void ShowToast(const std::string &message);

    void SwitchToPage(SettingsPage page);
    void OnGoBack();

  private:
    std::shared_ptr<Settings> m_settings;
    ReturnCallback m_onReturn;
    Settings m_backupSettings;

    SettingsPage m_currentPage;
    std::optional<SettingsPage> m_pendingPageChange;
    std::optional<std::string> m_pendingToast;

    std::shared_ptr<UIElement> m_applyButton;
    SceneID m_requestedScene;

    bool m_includeDifficultyPage = true;
    bool m_hasUnsavedChanges = false;
    bool m_showToast = false;
    float m_toastTimer = 0.f;

    std::shared_ptr<UITextLabel> m_titleLabel;
    std::unique_ptr<Background> m_background;
    sf::Text m_toastText;
};
