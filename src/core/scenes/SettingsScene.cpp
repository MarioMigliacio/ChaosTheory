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
#include "SettingsAssets.h"
#include "SettingsManager.h"
#include "UIFactory.h"
#include "UIGroupBox.h"
#include "UIManager.h"
#include "UIPresets.h"
#include "UISelectableButton.h"
#include "WindowManager.h"

/// @brief Constructor for the SettingsScene.
/// @param settings Internal settings to initialize with.
SettingsScene::SettingsScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

/// @brief Initialize the internal components for this SettingsScene.
void SettingsScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    UIManager::Instance().Clear();

    m_backupSettings = *SettingsManager::Instance().GetSettings();
    m_currentPage = SettingsPage::Audio;

    LoadRequiredAssets();
    SetupSceneComponents(m_currentPage);

    SceneTransitionManager::Instance().StartFadeIn();

    m_isInitialized = true;

    CT_LOG_INFO("SettingsScene initialized.");
}

/// @brief Load any required assets listed in the SettingsAssets namespace.
void SettingsScene::LoadRequiredAssets()
{
    auto &assets = AssetManager::Instance();

    for (const auto &[key, path] : SettingsAssets::Textures)
    {
        if (!assets.LoadTexture(key, path))
        {
            CT_LOG_ERROR("SettingsScene::LoadRequiredAssets::LoadTexture failed to load Asset: {}, {}", key, path);
        }
    }

    for (const auto &[key, path] : SettingsAssets::Sounds)
    {
        if (!assets.LoadSound(key, path))
        {
            CT_LOG_ERROR("SettingsScene::LoadRequiredAssets::LoadSound failed to load Asset: {}, {}", key, path);
        }
    }

    for (const auto &[key, path] : SettingsAssets::Fonts)
    {
        if (!assets.LoadFont(key, path))
        {
            CT_LOG_ERROR("SettingsScene::LoadRequiredAssets::LoadFont failed to load Asset: {}, {}", key, path);
        }
    }

    CT_LOG_INFO("SettingsScene finished LoadRequiredAssets.");
}

/// @brief Do any necessary logic for shutting this scene down.
void SettingsScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("SettingsScene", "Shutdown");

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("SettingsScene Shutdown.");
}

/// @brief Do any necessary logic upon exit for this scene.
void SettingsScene::OnExit()
{
    CT_LOG_INFO("SettingsScene OnExit.");
}

/// @brief Update internal members for this Settings Scene.
/// @param dt delta time since last update.
void SettingsScene::Update(float dt)
{
    const auto mousePos = InputManager::Instance().GetMousePosition();
    const bool isPressed = InputManager::Instance().IsMouseButtonPressed(sf::Mouse::Left);
    const bool isJustPressed = InputManager::Instance().IsMouseButtonJustPressed(sf::Mouse::Left);

    UIManager::Instance().Update(mousePos, isPressed, isJustPressed, dt);

    CheckForUnsavedChanges();

    if (m_background)
    {
        m_background->Update(dt);
    }

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
        CT_LOG_INFO("SettingsScene Requesting Scene Change to '{}'", SceneIDToString(m_requestedScene));
        m_hasPendingTransition = false;
        SceneTransitionManager::Instance().ForceFullyOpaque();
        SceneManager::Instance().RequestSceneChange(m_requestedScene);

        return;
    }

    if (m_pendingPageChange.has_value())
    {
        UIManager::Instance().Clear();
        SceneTransitionManager::Instance().StartFadeIn();
        SetupSceneComponents(m_pendingPageChange.value());

        if (m_pendingToast.has_value())
        {
            ShowToast(m_pendingToast.value());
            m_pendingToast.reset();
        }

        m_pendingPageChange.reset();
    }
}

/// @brief Not used in SettingsScene context.
/// @param event bubbled down from caller, not needed.
void SettingsScene::HandleEvent(const sf::Event &event)
{
}

/// @brief Not used in SettingsScene context. We manually call for adjustResolution.
/// @param newSize bubbled down from caller, not needed.
void SettingsScene::OnResize(const sf::Vector2u &newSize)
{
}

/// @brief Draw this SettingsScene to the render target.
void SettingsScene::Render()
{
    auto &window = WindowManager::Instance().GetWindow();
    window.clear();

    if (m_background)
    {
        m_background->Draw(window);
    }

    UIManager::Instance().Render(window);

    if (m_showToast)
    {
        window.draw(m_toastText);
    }
}

/// @brief Generate the Entire Settings Page that is specific to the request.
/// @param page Enumeration corresponding to the setting page type.
void SettingsScene::SetupSceneComponents(SettingsPage page)
{
    CreateTitleText();
    LoadBackground();
    CreateUI(page);
    CreateArrows(page);
    CreateButtonControls();
}

/// @brief Generate the UI elements needed for specific SettingsPage.
/// @param page Enumeration corresponding to the setting page type.
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
            CreateDifficultyControls();

            break;
        }
    }
}

/// @brief Generate the text that is the Title for this SettingsScene.
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

/// @brief Loads the main background image for this SettingsScene.
void SettingsScene::LoadBackground()
{
    m_background = std::make_unique<Background>();
    m_background->InitParallax({{"GasPattern3", 2.f}, {"PlainStarBackground", 1.f}});

    m_background->SetLayerMotion("PlainStarBackground", {1.f, -0.33f});
    m_background->SetLayerMotion("GasPattern3", {1.f, 0.f});

    CT_LOG_INFO("SettingsScene background loaded and scaled.");
}

/// @brief Generate the relevent Arrow UI elements for the requested SettingsPage.
/// @param page Enumeration corresponding to the setting page type.
void SettingsScene::CreateArrows(SettingsPage page)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    const float centerY = winSize.y / 2.f;

    if (page == SettingsPage::Audio || page == SettingsPage::KeyBindings)
    {
        // LEFT ARROW
        const float arrow_x = scaleMgr.ScaledReferenceX(DEFAULT_ARROW_LEFT_CENTER_PERCENT);
        auto leftArrow = UIFactory::Instance().CreateArrow(arrow_x, centerY, ArrowDirection::Left,
                                                           [this, page]()
                                                           {
                                                               if (page == SettingsPage::KeyBindings)
                                                               {
                                                                   SwitchToPage(SettingsPage::Audio);
                                                               }

                                                               else if (page == SettingsPage::Audio)
                                                               {
                                                                   SwitchToPage(SettingsPage::Video);
                                                               }
                                                           });

        UIManager::Instance().AddElement(leftArrow);
    }

    if (page == SettingsPage::Audio || page == SettingsPage::Video)
    {
        // RIGHT ARROW
        const float arrow_x = scaleMgr.ScaledReferenceX(DEFAULT_ARROW_RIGHT_CENTER_PERCENT);
        auto rightArrow = UIFactory::Instance().CreateArrow(arrow_x, centerY, ArrowDirection::Right,
                                                            [this, page]()
                                                            {
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

/// @brief Generate the buttons needed for this Settings Scene Page.
void SettingsScene::CreateButtonControls()
{
    auto winSize = WindowManager::Instance().GetWindow().getSize();

    const float footerY = winSize.y * BASE_FOOTER_HEIGHT_85_PERCENT;

    const float buttonWidth = BASE_BUTTON_WIDTH_PIXEL;
    const float buttonHeight = BASE_BUTTON_HEIGHT_PIXEL;

    // Offset from center for symmetry
    const float spacingFromCenter =
        ResolutionScaleManager::Instance().ScaleX(BASE_BUTTON_WIDTH_PIXEL / 2 + BASE_BUTTON_SPACING_PIXEL);

    const sf::Vector2f applyPos{winSize.x / 2.f - spacingFromCenter - buttonWidth / 2.f, footerY};
    const sf::Vector2f backPos{winSize.x / 2.f + spacingFromCenter - buttonWidth / 2.f, footerY};

    // Apply Changes UIButton
    m_applyButton = UIFactory::Instance().CreateButton(
        applyPos, {BASE_BUTTON_WIDTH_PIXEL, BASE_BUTTON_HEIGHT_PIXEL}, "Apply Changes",
        [this]()
        {
            CT_LOG_INFO("SettingsScene: Apply Changes clicked.");

            auto currentSetting = m_backupSettings.m_resolution;

            SettingsManager::Instance().SaveToFile("config.json");
            SettingsManager::Instance().LoadFromFile("config.json");

            m_backupSettings = *SettingsManager::Instance().GetSettings();
            m_hasUnsavedChanges = false;

            AudioManager::Instance().HotReload(SettingsManager::Instance().GetSettings());
            AudioManager::Instance().PlaySFX(SettingsAssets::SettingsSound);

            auto targetSetting = SettingsManager::Instance().GetSettings()->m_resolution;

            if (currentSetting != targetSetting)
            {
                auto targetSize = WindowManager::Instance().GetResolutionSize(targetSetting);
                WindowManager::Instance().ApplyResolution(targetSetting);
                ResolutionScaleManager::Instance().SetCurrentResolution(targetSize);
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
    UIManager::Instance().AddElement(
        UIFactory::Instance().CreateButton(backPos, {BASE_BUTTON_WIDTH_PIXEL, BASE_BUTTON_HEIGHT_PIXEL}, "Go Back",
                                           [this]()
                                           {
                                               CT_LOG_INFO("SettingsScene: Go Back clicked.");
                                               *SettingsManager::Instance().GetSettings() = m_backupSettings;
                                               m_requestedScene = SceneID::MainMenu;
                                               m_hasPendingTransition = true;
                                           }));
}

/// @brief Generate the Audio Sliders needed for the Audio Settings Scene page.
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

/// @brief Generate the buttons needed for the Resolution Settings page.
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
        auto selectableButton = UIFactory::Instance().CreateSelectableButton(
            {0.f, 0.f}, buttonSize, label,
            [this, resValue, label, groupBox]()
            {
                for (auto &el : groupBox->GetChildren())
                {
                    if (auto sb = std::dynamic_pointer_cast<UISelectableButton>(el))
                    {
                        sb->SetSelected(sb->GetLabel() == label);
                    }
                }
                SettingsManager::Instance().GetSettings()->m_resolution = resValue;
            });

        selectableButton->SetSelected(resValue == current);
        groupBox->AddElement(selectableButton);
    }

    UIManager::Instance().AddElement(groupBox);
}

/// @brief Generate the ui elements needed for the Difficulty Settings page.
void SettingsScene::CreateDifficultyControls()
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const std::string title = "Difficulty Settings";
    const sf::Vector2f relativePos{0.375f, 0.33f};
    const sf::Vector2f relativeSize{0.25f, 0.33f};

    // Create the group box using the centralized UIFactory
    auto groupBox = UIFactory::Instance().CreateGroupBox(title, relativePos, relativeSize);
    groupBox->SetEdgePadding(scaleMgr.ScaledReferenceY(.01f));
    groupBox->SetInternalPadding(scaleMgr.ScaledReferenceY(.2f * relativeSize.y));

    // Define the resolution options
    std::vector<std::pair<std::string, GameDifficultySetting>> options = {{"Easy", GameDifficultySetting::Easy},
                                                                          {"Normal", GameDifficultySetting::Normal},
                                                                          {"Hard", GameDifficultySetting::Hard}};

    const sf::Vector2f buttonSize = {BASE_BUTTON_WIDTH_PIXEL, BASE_BUTTON_HEIGHT_PIXEL};

    const auto current = SettingsManager::Instance().GetSettings()->m_gameDifficulty;

    for (const auto &[label, modeValue] : options)
    {
        auto selectableButton = UIFactory::Instance().CreateSelectableButton(
            {0.f, 0.f}, buttonSize, label,
            [this, modeValue, label, groupBox]()
            {
                for (auto &el : groupBox->GetChildren())
                {
                    if (auto sb = std::dynamic_pointer_cast<UISelectableButton>(el))
                    {
                        sb->SetSelected(sb->GetLabel() == label);
                    }
                }
                SettingsManager::Instance().GetSettings()->m_gameDifficulty = modeValue;
            });

        selectableButton->SetSelected(modeValue == current);
        groupBox->AddElement(selectableButton);
    }

    UIManager::Instance().AddElement(groupBox);
}

/// @brief Determines if there are any changes to the settings from the user.
void SettingsScene::CheckForUnsavedChanges()
{
    if (SettingsManager::Instance().GetSettings())
    {
        m_hasUnsavedChanges = SettingsManager::Instance().IsDifferentFrom(m_backupSettings);
    }
}

/// @brief Display a friendly toast message specific to this SettingsScene, that settings are changed.
/// @param message The toast message to render.
void SettingsScene::ShowToast(const std::string &message)
{
    const auto winSize = WindowManager::Instance().GetWindow().getSize();
    sf::Vector2f pos{winSize.x * BASE_FOOTER_WIDTH_75_PERCENT, winSize.y * BASE_FOOTER_HEIGHT_85_PERCENT};

    auto toast = UIFactory::Instance().CreateToastMessage(message, pos, TOAST_DEFAULT_DURATION);
    toast->SetColor(TOAST_DEFAULT_COLOR);
    UIManager::Instance().AddElement(toast);
}

/// @brief Upon clicking a navigation arrow, change the current Settings page to display.
/// @param page Enumeration corresponding to the setting page type.
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
