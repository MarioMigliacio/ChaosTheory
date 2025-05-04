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
#include "GroupBox.h"
#include "InputManager.h"
#include "Macros.h"
#include "RadioButton.h"
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

    UIManager::Instance().BlockInputUntilMouseRelease();
    UIManager::Instance().Clear();

    m_backupSettings = *SettingsManager::Instance().GetSettings();
    m_currentPage = SettingsPage::Audio; // by default

    CreateSettingsPage(m_currentPage);
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

        return;
    }

    if (m_pendingPageChange.has_value())
    {
        UIManager::Instance().Clear();
        SceneTransitionManager::Instance().StartFadeIn();
        CreateSettingsPage(m_pendingPageChange.value());
        m_pendingPageChange.reset();
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
    if (m_settings->m_resolution != ResolutionSetting::Fullscreen)
    {
        // Ignore manual resizes when not in fullscreen
        const auto intendedSize = WindowManager::Instance().GetWindow().getSize();

        if (newSize != intendedSize)
        {
            WindowManager::Instance().ApplyResolution(m_settings->m_resolution);

            return;
        }
    }

    // Proceed with layout reflow
    UIManager::Instance().Clear();
    CreateSettingsPage(m_currentPage);
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

// Generate the Entire Settings Page that is specific to the request.
void SettingsScene::CreateSettingsPage(SettingsPage page)
{
    CreateTitle(page);
    CreateUI(page);
    CreateArrows(page);
    CreateButtonControls();
}

// Generate the UI elements needed for specific SettingsPage.
void SettingsScene::CreateUI(SettingsPage page)
{
    m_currentPage = page;

    switch (page)
    {
        case SettingsPage::Audio:
        default:
        {
            CreateAudioControls();

            break;
        }

        case SettingsPage::Video:
        {
            CreateResolutionControls();

            break;
        }

        case SettingsPage::KeyBindings:
        {
            CreateKeyBindingControls();

            break;
        }
    }
}

// Generate the text that is the Title for the requested SettingsPage.
void SettingsScene::CreateTitle(SettingsPage page)
{
    AssetManager::Instance().LoadFont("Default.ttf", "assets/fonts/Default.ttf");
    m_title.setFont(AssetManager::Instance().GetFont("Default.ttf"));

    m_title.setCharacterSize(48);
    m_title.setFillColor(sf::Color(255, 255, 200));
    m_title.setOutlineColor(sf::Color::Black);
    m_title.setOutlineThickness(2.f);

    switch (page)
    {
        case SettingsPage::Audio:
            m_title.setString("Audio Settings");
            break;

        case SettingsPage::Video:
            m_title.setString("Video Settings");
            break;

        case SettingsPage::KeyBindings:
            m_title.setString("Key Binding Settings");
            break;
    }

    const auto bounds = m_title.getLocalBounds();
    m_title.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);

    const auto windowSize = WindowManager::Instance().GetWindow().getSize();
    m_title.setPosition(windowSize.x / 2.f, windowSize.y * 0.15f);
}

// Generate the relevent Arrow UI elements for the requested SettingsPage.
void SettingsScene::CreateArrows(SettingsPage page)
{
    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    const float centerY = winSize.y / 2.f;

    if (page == SettingsPage::Audio || page == SettingsPage::KeyBindings)
    {
        // LEFT ARROW (to previous page)
        auto leftArrow = UIFactory::Instance().CreateArrow(40.f, centerY, ArrowDirection::Left);

        leftArrow->SetOnClick(
            [this, page]()
            {
                UIManager::Instance().BlockInputUntilMouseRelease();

                if (page == SettingsPage::KeyBindings)
                {
                    SwitchToPage(SettingsPage::Audio);
                }

                else if (page == SettingsPage::Audio)
                {
                    SwitchToPage(SettingsPage::Video); // wrap or lock based on preference
                }
            });

        UIManager::Instance().AddElement(leftArrow);
    }

    if (page == SettingsPage::Audio || page == SettingsPage::Video)
    {
        // RIGHT ARROW (to next page)
        auto rightArrow = UIFactory::Instance().CreateArrow(winSize.x - 60.f, centerY, ArrowDirection::Right);

        rightArrow->SetOnClick(
            [this, page]()
            {
                UIManager::Instance().BlockInputUntilMouseRelease();

                if (page == SettingsPage::Audio)
                {
                    SwitchToPage(SettingsPage::KeyBindings);
                }

                else if (page == SettingsPage::Video)
                {
                    SwitchToPage(SettingsPage::Audio);
                }
            });

        UIManager::Instance().AddElement(rightArrow);
    }
}

// Meant to be self contained within SettingsScene if this sound effect has yet to be loaded.
void SettingsScene::LoadDefaultSFXFile()
{
    AssetManager::Instance().LoadSound("PewPew", "assets/audio/PewPew.wav");
    AssetManager::Instance().LoadFont("Default.ttf", "assets/fonts/Default.ttf");
}

// Generate the buttons needed for this Settings Scene Page.
void SettingsScene::CreateButtonControls()
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

            // Only apply resolution if size actually changed
            auto &window = WindowManager::Instance().GetWindow();
            auto currentSize = window.getSize();
            auto targetSetting = SettingsManager::Instance().GetSettings()->m_resolution;
            auto targetSize = WindowManager::Instance().GetResolutionSize(targetSetting);

            if (currentSize != targetSize)
            {
                WindowManager::Instance().ApplyResolution(targetSetting);
                UIManager::Instance().BlockInputUntilMouseRelease();
                m_pendingPageChange = m_currentPage;
            }
        });

    UIManager::Instance().AddElement(m_applyButton);

    startY += BUTTON_HEIGHT + BUTTON_SPACING;

    // Go Back Button
    UIManager::Instance().AddElement(UIFactory::Instance().CreateButton(
        ButtonType::Classic, {startX, startY}, {BUTTON_WIDTH, BUTTON_HEIGHT}, "Go Back",
        [this]()
        {
            CT_LOG_INFO("SettingsScene: Go Back clicked.");
            *SettingsManager::Instance().GetSettings() = m_backupSettings;
            m_requestedScene = SceneID::MainMenu;
            m_hasPendingTransition = true;
        }));
}

// Generate the Audio Sliders needed for this Settings Scene page.
void SettingsScene::CreateAudioControls()
{
    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    float startX = (winSize.x - SLIDER_WIDTH) / 2.f;
    float startY = winSize.y * 0.4f;

    UIManager::Instance().AddElement(UIFactory::Instance().CreateSlider(
        "Master Volume", {startX, startY}, {SLIDER_WIDTH, SLIDER_HEIGHT}, 0.f, 100.f, m_settings->m_masterVolume,
        [](float val) { SettingsManager::Instance().GetSettings()->m_masterVolume = val; }));

    startY += SLIDER_SPACING;

    UIManager::Instance().AddElement(UIFactory::Instance().CreateSlider(
        "Music Volume", {startX, startY}, {SLIDER_WIDTH, SLIDER_HEIGHT}, 0.f, 100.f, m_settings->m_musicVolume,
        [](float val) { SettingsManager::Instance().GetSettings()->m_musicVolume = val; }));

    startY += SLIDER_SPACING;

    UIManager::Instance().AddElement(UIFactory::Instance().CreateSlider(
        "SFX Volume", {startX, startY}, {SLIDER_WIDTH, SLIDER_HEIGHT}, 0.f, 100.f, m_settings->m_sfxVolume,
        [](float val) { SettingsManager::Instance().GetSettings()->m_sfxVolume = val; }));
}

// Generate the buttons needed for this Settings Scene resolution settings page.
void SettingsScene::CreateResolutionControls()
{
    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    float startX = (winSize.x - 320.f) / 2.f;
    float startY = winSize.y * 0.4f;

    auto currentRes = SettingsManager::Instance().GetSettings()->m_resolution;

    std::vector<std::pair<std::string, ResolutionSetting>> options = {{"480p", ResolutionSetting::Res480p},
                                                                      {"720p", ResolutionSetting::Res720p},
                                                                      {"1080p", ResolutionSetting::Res1080p},
                                                                      {"Fullscreen", ResolutionSetting::Fullscreen}};

    // Track radio buttons as base UIElement for standardization
    std::vector<std::shared_ptr<UIElement>> elements;

    for (size_t i = 0; i < options.size(); ++i)
    {
        float offsetX = (i % 2 == 0) ? 0.f : 160.f;
        float offsetY = (i < 2) ? 0.f : 50.f;
        sf::Vector2f pos = {startX + offsetX, startY + offsetY};

        auto label = options[i].first;
        auto resValue = options[i].second;

        auto radio = std::dynamic_pointer_cast<RadioButton>(
            UIFactory::Instance().CreateButton(ButtonType::Radio, pos, {140.f, 30.f}, label,
                                               [this, resValue, label]()
                                               {
                                                   // Deselect all radios
                                                   for (auto &el : UIManager::Instance().GetElements())
                                                   {
                                                       if (auto rb = std::dynamic_pointer_cast<RadioButton>(el))
                                                           rb->SetSelected(false);
                                                   }

                                                   // Set selected resolution in the settings
                                                   SettingsManager::Instance().GetSettings()->m_resolution = resValue;

                                                   // Select the radio that matches the clicked label
                                                   for (auto &el : UIManager::Instance().GetElements())
                                                   {
                                                       if (auto rb = std::dynamic_pointer_cast<RadioButton>(el))
                                                       {
                                                           if (rb->GetLabel() == label)
                                                           {
                                                               rb->SetSelected(true);
                                                               break;
                                                           }
                                                       }
                                                   }
                                               }));

        // Initial selection
        if (currentRes == resValue)
            radio->SetSelected(true);

        UIManager::Instance().AddElement(radio);
    }
}

// Generate the ui elements needed for this Key Bindings settings page.
void SettingsScene::CreateKeyBindingControls()
{
    // merely a placeholder for the time being.
    const auto winSize = WindowManager::Instance().GetWindow().getSize();

    // Create the GroupBox panel
    auto groupBox = std::make_shared<GroupBox>(sf::Vector2f((winSize.x - 360.f) / 2.f, winSize.y * 0.4f),
                                               sf::Vector2f(360.f, 160.f));
    groupBox->SetTitle("Key Binding Settings", AssetManager::Instance().GetFont("Default.ttf"));
    groupBox->SetLayoutMode(LayoutMode::Vertical);
    groupBox->SetCenterChildren(true); // Try false if you want left-aligned buttons

    // Add some placeholder buttons for binding keys
    groupBox->AddElement(UIFactory::Instance().CreateButton(ButtonType::Classic, {0.f, 0.f}, {200.f, 30.f}, "Fire Bomb",
                                                            []() { CT_LOG_INFO("Fire Bomb clicked"); }));

    groupBox->AddElement(UIFactory::Instance().CreateButton(ButtonType::Classic, {0.f, 0.f}, {200.f, 30.f}, "Shoot",
                                                            []() { CT_LOG_INFO("Shoot clicked"); }));

    groupBox->AddElement(UIFactory::Instance().CreateButton(ButtonType::Classic, {0.f, 0.f}, {200.f, 30.f}, "Strafe",
                                                            []() { CT_LOG_INFO("Strafe clicked"); }));

    // Add group box to the UI system
    UIManager::Instance().AddElement(groupBox);
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

void SettingsScene::SwitchToPage(SettingsPage page)
{
    if (page == m_currentPage)
    {
        return;
    }

    // Revert any local changes before leaving page
    *SettingsManager::Instance().GetSettings() = m_backupSettings;

    // Don't switch immediately — set for safe deferred swap
    m_pendingPageChange = page;
}
