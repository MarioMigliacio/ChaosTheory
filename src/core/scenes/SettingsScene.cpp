// ============================================================================
//  File        : SettingsScene.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-23
//  Description : Hosts the definition for Settings Scene object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SettingsScene.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "Macros.h"
#include "SceneManager.h"
#include "SceneTransitionManager.h"
#include "SettingsManager.h"
#include "UIFactory.h"
#include "UIManager.h"
#include "WindowManager.h"

namespace
{
constexpr float SLIDER_WIDTH = 300.f;
constexpr float SLIDER_HEIGHT = 20.f;
constexpr float SLIDER_SPACING = 50.f;
constexpr float BUTTON_WIDTH = 160.f;
constexpr float BUTTON_HEIGHT = 30.f;
constexpr float BUTTON_SPACING = 20.f;
constexpr float TOAST_DURATION = 2.0f;
} // namespace

SettingsScene::SettingsScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

// Initialize the internal components for this SettingsScene.
void SettingsScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    UIManager::Instance().Clear();
    m_backupSettings = *SettingsManager::Instance().GetSettings();

    CreateTitleText();
    CreateSliders();
    CreateButtons();
    LoadDefaultSFXFile();

    SceneTransitionManager::Instance().StartFadeIn();

    m_isInitialized = true;

    CT_LOG_INFO("SettingsScene initialized.");
}

// Do any necessary logic for shutting this scene down.
void SettingsScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("SettingsScene", "Shutdown");

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("SettingsScene Shutdown.");
}

// Do any necessary logic upon exit for this scene.
void SettingsScene::OnExit()
{
    CT_LOG_INFO("SettingsScene OnExit.");
}

// Update internal members for this Settings Scene.
void SettingsScene::Update(float dt)
{
    const auto mousePos = InputManager::Instance().GetMousePosition();
    const bool isPressed = InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left);

    UIManager::Instance().Update(mousePos, isPressed);

    CheckForUnsavedChanges();

    if (m_applyButton)
    {
        m_applyButton->SetEnabled(m_hasUnsavedChanges);
    }

    if (m_showToast)
    {
        m_toastTimer -= dt;
        if (m_toastTimer <= 0.f)
        {
            m_showToast = false;
        }
    }

    if (m_hasPendingTransition)
    {
        CT_LOG_INFO("SettingsScene Requesting Scene Change to '{}'", ToString(m_requestedScene));
        m_hasPendingTransition = false;
        SceneTransitionManager::Instance().ForceFullyOpaque();
        SceneManager::Instance().RequestSceneChange(m_requestedScene);
    }
}

// Handle any internal logic that should be done relevant to this scene.
void SettingsScene::HandleEvent(const sf::Event &event)
{
    // Only if you want to catch window resize, close, etc.
}

// Update the dimensions for this Settings Scene.
void SettingsScene::OnResize(const sf::Vector2u &newSize)
{
    CreateTitleText();

    // Adjust UI elements
    UIManager::Instance().Clear();
    CreateButtons();
    CreateSliders();
}

// Draw this Settings Scene to the render target.
void SettingsScene::Render()
{
    auto &window = WindowManager::Instance().GetWindow();
    window.clear();

    window.draw(m_title);
    UIManager::Instance().Render(window);

    if (m_showToast)
    {
        window.draw(m_toastText);
    }
}

// Generate the text that is the Title for this Settings Scene.
void SettingsScene::CreateTitleText()
{
    m_title.setFont(AssetManager::Instance().GetFont("Default.ttf"));
    m_title.setString("Settings");
    m_title.setCharacterSize(48);
    m_title.setFillColor(sf::Color(255, 255, 200));
    m_title.setOutlineColor(sf::Color::Black);
    m_title.setOutlineThickness(2.f);

    const auto bounds = m_title.getLocalBounds();
    m_title.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);

    const auto windowSize = WindowManager::Instance().GetWindow().getSize();
    m_title.setPosition(windowSize.x / 2.f, windowSize.y * 0.15f);

    // Toast Text Setup
    m_toastText.setFont(AssetManager::Instance().GetFont("Default.ttf"));
    m_toastText.setCharacterSize(24);
    m_toastText.setFillColor(sf::Color(0, 255, 0));
    m_toastText.setOutlineColor(sf::Color::Black);
    m_toastText.setOutlineThickness(2.f);
}

// Generate the Sliders needed for this Settings Scene.
void SettingsScene::CreateSliders()
{
    auto windowSize = WindowManager::Instance().GetWindow().getSize();
    float startX = (windowSize.x - SLIDER_WIDTH) / 2.f;
    float startY = windowSize.y * 0.4f;

    UIManager::Instance().AddElement(
        MakeSlider("Master Volume", {startX, startY}, SettingsManager::Instance().GetSettings()->m_masterVolume,
                   [](float value) { SettingsManager::Instance().GetSettings()->m_masterVolume = value; }));

    startY += SLIDER_SPACING;

    UIManager::Instance().AddElement(
        MakeSlider("Music Volume", {startX, startY}, SettingsManager::Instance().GetSettings()->m_musicVolume,
                   [](float value) { SettingsManager::Instance().GetSettings()->m_musicVolume = value; }));

    startY += SLIDER_SPACING;

    UIManager::Instance().AddElement(MakeSlider("SFX Volume", {startX, startY},
                                                SettingsManager::Instance().GetSettings()->m_sfxVolume, [](float value)
                                                { SettingsManager::Instance().GetSettings()->m_sfxVolume = value; }));
}

// Generate the buttons needed for this Settings Scene.
void SettingsScene::CreateButtons()
{
    auto windowSize = WindowManager::Instance().GetWindow().getSize();
    float startX = (windowSize.x - BUTTON_WIDTH) / 2.f;
    float startY = windowSize.y * 0.7f;

    // Apply Changes Button
    m_applyButton = UIFactory::Instance().CreateButton(
        ButtonType::Classic, {startX, startY}, {BUTTON_WIDTH, BUTTON_HEIGHT}, "Apply Changes",
        [this]()
        {
            CT_LOG_INFO("SettingsScene: Apply Changes clicked.");
            SettingsManager::Instance().SaveToFile("config.json");
            SettingsManager::Instance().LoadFromFile("config.json");
            m_backupSettings = *SettingsManager::Instance().GetSettings();
            m_hasUnsavedChanges = false;
            ShowToast("Settings Applied");

            AudioManager::Instance().HotReload(SettingsManager::Instance().GetSettings());
            AudioManager::Instance().PlaySFX("PewPew");
        });

    UIManager::Instance().AddElement(m_applyButton);

    startY += BUTTON_HEIGHT + BUTTON_SPACING;

    // Cancel Changes Button
    UIManager::Instance().AddElement(UIFactory::Instance().CreateButton(
        ButtonType::Classic, {startX, startY}, {BUTTON_WIDTH, BUTTON_HEIGHT}, "Cancel Changes",
        [this]()
        {
            CT_LOG_INFO("SettingsScene: Cancel Changes clicked.");
            *SettingsManager::Instance().GetSettings() = m_backupSettings;
            m_requestedScene = SceneID::MainMenu;
            m_hasPendingTransition = true;
        }));
}

// Meant to be self contained within SettingsScene if this sound effect has yet to be loaded.
void SettingsScene::LoadDefaultSFXFile()
{
    AssetManager::Instance().LoadSound("PewPew", "assets/audio/PewPew.wav");
}

std::shared_ptr<UIElement> SettingsScene::MakeSlider(const std::string &label, const sf::Vector2f &position,
                                                     float initial, std::function<void(float)> onChange)
{
    return UIFactory::Instance().CreateSlider(label, position, {SLIDER_WIDTH, SLIDER_HEIGHT}, 0.f, 100.f, initial,
                                              std::move(onChange));
}

// Determines if there are any changes to the settings from the user.
void SettingsScene::CheckForUnsavedChanges()
{
    if (SettingsManager::Instance().GetSettings())
    {
        m_hasUnsavedChanges = SettingsManager::Instance().IsDifferentFrom(m_backupSettings);
    }
}

// Display a friendly toast message to indicate that settings are changed.
void SettingsScene::ShowToast(const std::string &message)
{
    m_toastText.setString(message);
    const auto bounds = m_toastText.getLocalBounds();
    m_toastText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);

    auto windowSize = WindowManager::Instance().GetWindow().getSize();
    m_toastText.setPosition(windowSize.x / 2.f, windowSize.y * 0.85f);

    m_toastTimer = TOAST_DURATION;
    m_showToast = true;
}
