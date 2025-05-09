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
#include "ResolutionScaleManager.h"
#include "SceneManager.h"
#include "SceneTransitionManager.h"
#include "SettingsManager.h"
#include "UIFactory.h"
#include "UIGroupBox.h"
#include "UIManager.h"
#include "UIPresets.h"
#include "UISelectableButton.h"
#include "WindowManager.h"

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

    UIManager::Instance().Update(mousePos, isPressed, dt);

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

        if (m_pendingToast.has_value())
        {
            ShowToast(m_pendingToast.value());
            m_pendingToast.reset();
        }

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
    ResolutionScaleManager::Instance().SetCurrentResolution(newSize);

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

    UIManager::Instance().Render(window);

    if (m_showToast)
    {
        window.draw(m_toastText);
    }
}

// Generate the Entire Settings Page that is specific to the request.
void SettingsScene::CreateSettingsPage(SettingsPage page)
{
    CreateTitleText();
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
void SettingsScene::CreateTitleText()
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const std::string titleText = DEFAULT_SETTINGS_STR;
    const unsigned int fontSize = scaleMgr.ScaleFont(DEFAULT_TITLE_FONT_SIZE);
    const sf::Vector2f centerPos = {WindowManager::Instance().GetWindow().getSize().x / 2.f,
                                    scaleMgr.ScaledReferenceY(DEFAULT_TITLE_HEIGHT_PERCENT)};

    m_titleLabel = UIFactory::Instance().CreateTextLabel(titleText, centerPos, fontSize, true);
    m_titleLabel->SetColor(DEFAULT_TITLE_COLOR);
    UIManager::Instance().AddElement(m_titleLabel);
}

// Generate the relevent Arrow UI elements for the requested SettingsPage.
void SettingsScene::CreateArrows(SettingsPage page)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    const float centerY = winSize.y / 2.f;

    if (page == SettingsPage::Audio || page == SettingsPage::KeyBindings)
    {
        // LEFT ARROW (to previous page)
        const float arrow_x = scaleMgr.ScaledReferenceX(DEFAULT_ARROW_LEFT_CENTER_PERCENT);
        auto leftArrow = UIFactory::Instance().CreateArrow(arrow_x, centerY, ArrowDirection::Left);

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
        const float arrow_x = scaleMgr.ScaledReferenceX(DEFAULT_ARROW_RIGHT_CENTER_PERCENT);
        auto rightArrow = UIFactory::Instance().CreateArrow(arrow_x, centerY, ArrowDirection::Right);

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
    auto winSize = WindowManager::Instance().GetWindow().getSize();

    const float footerY = winSize.y * BASE_FOOTER_HEIGHT_85_PERCENT;
    ; // 15% away from absolute bottom

    const float buttonWidth = BASE_BUTTON_WIDTH_PIXEL;
    const float buttonHeight = BASE_BUTTON_HEIGHT_PIXEL;

    // Offset from center for symmetry
    const float spacingFromCenter =
        ResolutionScaleManager::Instance().ScaleX(BASE_BUTTON_WIDTH_PIXEL / 2 + BASE_BUTTON_SPACING_PIXEL);

    const sf::Vector2f applyPos{winSize.x / 2.f - spacingFromCenter - buttonWidth / 2.f, footerY};
    const sf::Vector2f backPos{winSize.x / 2.f + spacingFromCenter - buttonWidth / 2.f, footerY};

    // Apply Changes UIButton
    m_applyButton = UIFactory::Instance().CreateButton(
        ButtonType::Classic, applyPos, {BASE_BUTTON_WIDTH_PIXEL, BASE_BUTTON_HEIGHT_PIXEL}, "Apply Changes",
        [this]()
        {
            CT_LOG_INFO("SettingsScene: Apply Changes clicked.");

            auto currentSetting = m_backupSettings.m_resolution;

            SettingsManager::Instance().SaveToFile("config.json");
            SettingsManager::Instance().LoadFromFile("config.json");

            m_backupSettings = *SettingsManager::Instance().GetSettings();
            m_hasUnsavedChanges = false;

            AudioManager::Instance().HotReload(SettingsManager::Instance().GetSettings());
            AudioManager::Instance().PlaySFX("PewPew");

            auto targetSetting = SettingsManager::Instance().GetSettings()->m_resolution;

            if (currentSetting != targetSetting)
            {
                auto targetSize = WindowManager::Instance().GetResolutionSize(targetSetting);
                WindowManager::Instance().ApplyResolution(targetSetting);
                ResolutionScaleManager::Instance().SetCurrentResolution(targetSize);
                UIManager::Instance().BlockInputUntilMouseRelease();
                m_pendingPageChange = m_currentPage;
                m_pendingToast = "Settings Applied"; // defer toast display
            }

            else
            {
                ShowToast("Settings Applied"); // no resolution change, show immediately
            }
        });

    UIManager::Instance().AddElement(m_applyButton);

    // Go Back UIButton
    UIManager::Instance().AddElement(UIFactory::Instance().CreateButton(
        ButtonType::Classic, backPos, {BASE_BUTTON_WIDTH_PIXEL, BASE_BUTTON_HEIGHT_PIXEL}, "Go Back",
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
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const std::string title = "Audio Settings";
    const sf::Vector2f relativePos{0.25f, 0.25f};
    const sf::Vector2f relativeSize{0.5f, 0.5f};

    // Create a 50% screen width, 50% screen height UIGroupBox for UI elements as children.
    auto groupBox = UIFactory::Instance().CreateGroupBox(title, relativePos, relativeSize);

    const float referenceSliderWidth = BASE_SLIDER_WIDTH_PERCENT; // 45% of reference width
    const float sliderHeight = scaleMgr.ScaleY(BASE_SLIDER_HEIGHT_PIXEL);

    groupBox->AddElement(UIFactory::Instance().CreateSlider(
        "Master Volume", {0.f, 0.f}, {referenceSliderWidth, sliderHeight}, 0.f, 100.f, m_settings->m_masterVolume,
        [](float val) { SettingsManager::Instance().GetSettings()->m_masterVolume = val; }));

    groupBox->AddElement(UIFactory::Instance().CreateSlider(
        "Music Volume", {0.f, 0.f}, {referenceSliderWidth, sliderHeight}, 0.f, 100.f, m_settings->m_musicVolume,
        [](float val) { SettingsManager::Instance().GetSettings()->m_musicVolume = val; }));

    groupBox->AddElement(UIFactory::Instance().CreateSlider(
        "SFX Volume", {0.f, 0.f}, {referenceSliderWidth, sliderHeight}, 0.f, 100.f, m_settings->m_sfxVolume,
        [](float val) { SettingsManager::Instance().GetSettings()->m_sfxVolume = val; }));

    UIManager::Instance().AddElement(groupBox);
}

// Generate the buttons needed for this Settings Scene resolution settings page.
void SettingsScene::CreateResolutionControls()
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const std::string title = "Video Settings";
    const sf::Vector2f relativePos{0.375f, 0.33f};
    const sf::Vector2f relativeSize{0.25f, 0.33f};

    // Create the group box using the centralized UIFactory
    auto groupBox = UIFactory::Instance().CreateGroupBox(title, relativePos, relativeSize);
    groupBox->SetEdgePadding(scaleMgr.ScaledReferenceY(.01f));
    groupBox->SetInternalPadding(scaleMgr.ScaledReferenceY(.2f * relativeSize.y));

    // Define the resolution options
    std::vector<std::pair<std::string, ResolutionSetting>> options = {{"720p", ResolutionSetting::Res720p},
                                                                      {"1080p", ResolutionSetting::Res1080p},
                                                                      {"Fullscreen", ResolutionSetting::Fullscreen}};

    const sf::Vector2f buttonSize = {BASE_BUTTON_WIDTH_PIXEL, BASE_BUTTON_HEIGHT_PIXEL};

    const auto current = SettingsManager::Instance().GetSettings()->m_resolution;

    for (const auto &[label, resValue] : options)
    {
        auto radio = std::dynamic_pointer_cast<UISelectableButton>(
            UIFactory::Instance().CreateButton(ButtonType::Radio, {0.f, 0.f}, buttonSize, label,
                                               [this, resValue, label, groupBox]()
                                               {
                                                   for (auto &el : groupBox->GetChildren())
                                                   {
                                                       if (auto rb = std::dynamic_pointer_cast<UISelectableButton>(el))
                                                       {
                                                           rb->SetSelected(rb->GetLabel() == label);
                                                       }
                                                   }
                                                   SettingsManager::Instance().GetSettings()->m_resolution = resValue;
                                               }));

        radio->SetSelected(resValue == current);
        groupBox->AddElement(radio);
    }

    UIManager::Instance().AddElement(groupBox);
}

// Generate the ui elements needed for this Key Bindings settings page.
void SettingsScene::CreateKeyBindingControls()
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const std::string title = "Key Binding Settings";
    const sf::Vector2f relativePos{0.375f, 0.33f};
    const sf::Vector2f relativeSize{0.25f, 0.33f};

    // Create the group box using the centralized UIFactory
    auto groupBox = UIFactory::Instance().CreateGroupBox(title, relativePos, relativeSize);
    groupBox->SetEdgePadding(scaleMgr.ScaledReferenceY(.01f));
    groupBox->SetInternalPadding(scaleMgr.ScaledReferenceY(.2f * relativeSize.y));

    // Add some placeholder buttons for binding keys
    groupBox->AddElement(UIFactory::Instance().CreateButton(ButtonType::Classic, {0.f, 0.f},
                                                            {BASE_BUTTON_WIDTH_PIXEL, BASE_BUTTON_HEIGHT_PIXEL},
                                                            "Fire Bomb", []() { CT_LOG_INFO("Fire Bomb clicked"); }));

    groupBox->AddElement(UIFactory::Instance().CreateButton(ButtonType::Classic, {0.f, 0.f},
                                                            {BASE_BUTTON_WIDTH_PIXEL, BASE_BUTTON_HEIGHT_PIXEL},
                                                            "Shoot", []() { CT_LOG_INFO("Shoot clicked"); }));

    groupBox->AddElement(UIFactory::Instance().CreateButton(ButtonType::Classic, {0.f, 0.f},
                                                            {BASE_BUTTON_WIDTH_PIXEL, BASE_BUTTON_HEIGHT_PIXEL},
                                                            "Strafe", []() { CT_LOG_INFO("Strafe clicked"); }));

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

// Display a friendly toast message to indicate that settings are changed
void SettingsScene::ShowToast(const std::string &message)
{
    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    sf::Vector2f pos{winSize.x * BASE_FOOTER_WIDTH_75_PERCENT, winSize.y * BASE_FOOTER_HEIGHT_85_PERCENT};

    auto toast = UIFactory::Instance().CreateToastMessage(message, pos, TOAST_DEFAULT_DURATION);
    toast->SetColor(TOAST_DEFAULT_COLOR);
    UIManager::Instance().AddElement(toast);

    // ------
    // const auto bounds = m_toastText.getLocalBounds();

    // m_toastText.setFont(AssetManager::Instance().GetFont("Default.ttf"));
    // m_toastText.setString(message);

    // m_toastText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    // m_toastText.setFillColor(DEFAULT_TITLE_COLOR);
    // m_toastText.setCharacterSize(24);

    // auto windowSize = WindowManager::Instance().GetWindow().getSize();
    // m_toastText.setPosition(windowSize.x * BASE_FOOTER_WIDTH_75_PERCENT, windowSize.y *
    // BASE_FOOTER_HEIGHT_85_PERCENT);

    // m_toastTimer = TOAST_DEFAULT_DURATION;
    // m_showToast = true;
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
