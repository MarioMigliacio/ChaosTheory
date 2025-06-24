// ============================================================================
//  File        : UIFactory.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-17
//  Description : UIFactory is responsible for creating different types of
//                UI elements such as buttons, sliders, chatboxes, etc.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIFactory.h"
#include "AssetManager.h"
#include "Assets.h"
#include "Macros.h"
#include "ResolutionScaleManager.h"
#include "SettingsManager.h"

/// @brief Get the current Instance for this UIFactory singleton.
/// @return reference to existing UIFactory interface.
UIFactory &UIFactory::Instance()
{
    static UIFactory instance;
    return instance;
}

/// @brief Creates a UI Button element, given the custom input parameters.
/// @param position Button position.
/// @param size Button size.
/// @param label Button text with font and font size.
/// @param onClick Pointer to callback function, when clicked.
/// @return safe pointer to a UIButton.
std::shared_ptr<UIButton> UIFactory::CreateButton(const sf::Vector2f &position, const sf::Vector2f &size,
                                                  const std::string &label, std::function<void()> onClick)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(size.x),
                            ResolutionScaleManager::Instance().ScaleY(size.y));
    unsigned int scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(BUTTON_DEFAULT_FONT_SIZE);

    auto btn = std::make_shared<UIButton>(position, scaledSize);

    btn->SetText(label, *AssetManager::Instance().GetFont(FontAssets::DefaultFontKey), scaledFontSize);
    btn->SetTextColor(BUTTON_DEFAULT_TEXT_COLOR);
    btn->SetCallback(std::move(onClick));
    btn->SetIdleColor(BUTTON_DEFAULT_IDLE_COLOR);
    btn->SetHoverColor(BUTTON_DEFAULT_HOVER_COLOR);
    btn->SetActiveColor(BUTTON_DEFAULT_ACTIVE_COLOR);

    btn->SetHoverScale(1.05f);

    return btn;
}

/// @brief Creates a UI Selectable Button element, given the custom input parameters.
/// @param position Button position.
/// @param size Button size.
/// @param label Button text with font and font size.
/// @param onClick Pointer to callback function, when clicked.
/// @return safe pointer to a UISelectableButton.
std::shared_ptr<UISelectableButton> UIFactory::CreateSelectableButton(const sf::Vector2f &position,
                                                                      const sf::Vector2f &size,
                                                                      const std::string &label,
                                                                      std::function<void()> onClick)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(size.x),
                            ResolutionScaleManager::Instance().ScaleY(size.y));
    unsigned int scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(SELECTABLE_BUTTON_DEFAULT_FONT_SIZE);

    auto btn = std::make_shared<UISelectableButton>(position, scaledSize);

    btn->SetText(label, *AssetManager::Instance().GetFont(FontAssets::DefaultFontKey), scaledFontSize);
    btn->SetTextColor(BUTTON_DEFAULT_TEXT_COLOR);
    btn->SetCallback(std::move(onClick));
    btn->SetHoverColor(BUTTON_DEFAULT_HOVER_COLOR);
    btn->SetSelectedColor(BUTTON_DEFAULT_SELECTED_COLOR, BUTTON_DEFAULT_SELECTED_TEXT_COLOR);

    return btn;
}

/// @brief Generates a SkinnableButton ui element.
/// @param pos Position to set button at.
/// @param size Size to initialize button with.
/// @param label Text to initialize button with.
/// @param idle Path to Idle texture to be used for SkinnableButton
/// @param hover Path to Hover texture to be used for SkinnableButton
/// @param scheme Enum field representing the scheme for Button text layouts.
/// @param onClick Pointer to callback function, when clicked.
/// @return Smart pointer to a UISkinnableButton.
std::shared_ptr<UISkinnableButton> UIFactory::CreateSkinnableButton(const sf::Vector2f &pos, const sf::Vector2f &size,
                                                                    const std::string &label, const std::string &idle,
                                                                    const std::string &hover,
                                                                    UIButtonColorScheme scheme,
                                                                    std::function<void()> onClick)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(size.x),
                            ResolutionScaleManager::Instance().ScaleY(size.y));
    unsigned int scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(SKINNABLE_BUTTON_DEFAULT_FONT_SIZE);

    auto btn = std::make_shared<UISkinnableButton>(pos, scaledSize);

    btn->SetText(label, *AssetManager::Instance().GetFont(FontAssets::DefaultFontKey), scaledFontSize);
    btn->SetTextureSkins(idle, hover);
    btn->ApplySkinnableButtonTextStyle(scheme);
    btn->SetCallback(onClick);

    return btn;
}

/// @brief Creates a UI Slider element, given the custom input parameters.
/// @param label String representation for slider content.
/// @param position Slider position.
/// @param size Slider size.
/// @param minValue Slider minimum value.
/// @param maxValue Slider maximum value.
/// @param initialValue Slider default value.
/// @param onChange Pointer to callback function, when value changed.
/// @return safe pointer to a UISlider.
std::shared_ptr<UISlider> UIFactory::CreateSlider(const std::string &label, const sf::Vector2f &position,
                                                  const sf::Vector2f &size, float minValue, float maxValue,
                                                  float initialValue, std::function<void(float)> onChange)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    // We explicitely do scale the slider UI elements position because it impacts the slider background and knob
    const sf::Vector2f scaledPos = {scaleMgr.ScaledReferenceX(position.x), scaleMgr.ScaledReferenceY(position.y)};

    const auto scaledSize = sf::Vector2f(scaleMgr.ScaledReferenceX(size.x), scaleMgr.ScaleY(size.y));
    const auto scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(BASE_SLIDER_TITLE_FONT_SIZE);

    auto slider = std::make_shared<UISlider>(label, minValue, maxValue, initialValue, scaledPos, scaledSize, onChange);
    slider->SetFont(*AssetManager::Instance().GetFont(FontAssets::DefaultFontKey));
    slider->SetFontSize(scaledFontSize);
    slider->SetTitlePositionOffset(sf::Vector2f(0.f, -ResolutionScaleManager::Instance().ScaleY(BASE_SLIDER_OFFSET_Y)));

    return slider;
}

/// @brief Creates a UI Arrow element, given the custom input parameters.
/// @param position Position to emplace.
/// @param size Size to initialize with.
/// @param texture Loadable texture path.
/// @param direction Arrow direction L, R, U, D.
/// @param onClick Pointer to callback function, when clicked.
/// @return Safe pointer to a UIArrow.
std::shared_ptr<UIArrow> UIFactory::CreateArrow(const sf::Vector2f &position, const sf::Vector2f &size,
                                                const std::string &texture, ArrowDirection direction,
                                                std::function<void()> onClick)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(size.x),
                            ResolutionScaleManager::Instance().ScaleY(size.y));

    auto arrow = std::make_shared<UIArrow>(position, scaledSize, direction);
    arrow->SetOnClick(onClick);
    arrow->SetTextureSkin(texture);
    arrow->SetSize(scaledSize);

    return arrow;
}

/// @brief Creates a UI UIGroupBox element, given the custom input parameters.

/// @param position Screen relative position to be centered around.
/// @param size Screen relative size to occupy.
/// @param title String representation for GroupBox.
/// @param centerOrigin Whether or not to center title text for groupbox around position.
/// @param mode Relative or Absolute position based.
/// @param scheme Type of ui text label color themes.
/// @return safe pointer to a UIGroupBox.
std::shared_ptr<UIGroupBox> UIFactory::CreateGroupBox(const sf::Vector2f &position, const sf::Vector2f &size,
                                                      const std::string &title, bool centerOrigin, CoordinateMode mode,
                                                      UITextLabelScheme scheme)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    sf::Vector2f finalPos = (mode == CoordinateMode::Relative) ? sf::Vector2f{scaleMgr.ScaledReferenceX(position.x),
                                                                              scaleMgr.ScaledReferenceY(position.y)}
                                                               : position;

    sf::Vector2f finalSize = (mode == CoordinateMode::Relative)
                                 ? sf::Vector2f{scaleMgr.ScaledReferenceX(size.x), scaleMgr.ScaledReferenceY(size.y)}
                                 : size;

    const float internalPadding = scaleMgr.ScaledReferenceY(BASE_GROUPBOX_INTERNAL_PAD_RATIO);
    const float edgePadding = scaleMgr.ScaledReferenceY(BASE_GROUPBOX_EDGE_PAD_RATIO);

    auto groupBox = std::make_shared<UIGroupBox>(finalPos, finalSize);

    if (title != "")
    {
        groupBox->SetTitle(title, *AssetManager::Instance().GetFont(FontAssets::DefaultFontKey),
                           scaleMgr.ScaleFont(BASE_GROUPBOX_FONT_SIZE), centerOrigin, scheme);
    }

    groupBox->SetLayoutMode(LayoutMode::Vertical);
    groupBox->SetCenterChildren(true);
    groupBox->SetInternalPadding(internalPadding);
    groupBox->SetEdgePadding(edgePadding);

    return groupBox;
}

/// @brief Creates a UI UITextLabel element, given the custom input parameters.
/// @param text String representation for the element.
/// @param position Position for text label.
/// @param fontSize Size of the font for the Text.
/// @param centerOrigin Used for centering the string contents around the position during construction.
/// @param scheme Enum field representing the type of shceme.
/// @return Safe pointer to a UITextLabel.
std::shared_ptr<UITextLabel> UIFactory::CreateTextLabel(const std::string &text, const sf::Vector2f &position,
                                                        unsigned int fontSize, bool centerOrigin,
                                                        UITextLabelScheme scheme)
{
    const float scaledOutline = ResolutionScaleManager::Instance().ScaleX(DEFAULT_TEXT_LABEL_BORDER_THICKNESS);

    auto label = std::make_shared<UITextLabel>(text, *AssetManager::Instance().GetFont(FontAssets::DefaultFontKey),
                                               fontSize, position, centerOrigin);
    label->ApplyTextLabelStyle(scheme, scaledOutline);

    return label;
}

/// @brief Creates a UI UIToastMessage element, given the custom input parameters.
/// @param text String representation for the toast message.
/// @param position Position for the toast message.
/// @param duration Duration for how long the toast message will exist.
/// @param baseFontSize Font size to initialize with.
/// @param centerOrigin Whether or not the center text around origin.
/// @param scheme Enum field representing the type of shceme.
/// @return safe pointer to a UIToastMessage.
std::shared_ptr<UIToastMessage> UIFactory::CreateToastMessage(const std::string &text, const sf::Vector2f &position,
                                                              float duration, unsigned int baseFontSize,
                                                              bool centerOrigin, UITextLabelScheme scheme)
{
    const float scaledOutline = ResolutionScaleManager::Instance().ScaleX(DEFAULT_TEXT_LABEL_BORDER_THICKNESS);
    const unsigned int fontSize = ResolutionScaleManager::Instance().ScaleFont(baseFontSize);

    auto toast = std::make_shared<UIToastMessage>(text, position, fontSize, duration, centerOrigin, scheme);
    toast->ApplyStyle(scheme, scaledOutline);

    return toast;
}

/// @brief Creates a UI HUDPanel element, given the custom input parameters.
/// @param relativePosition Position for the HUD Panel.
/// @param relativeSize Size for HUD panel.
/// @param fillColor Fill color for HUD panel.
/// @param outlineColor Outline color for HUD panel.
/// @param outlineThickness Outline thickenss for HUD panel.
/// @param mode Relative or Absolute position based.
/// @return safe pointer to a UIHUDPanel.
std::shared_ptr<UIHUDPanel> UIFactory::CreateHUDPanel(const sf::Vector2f &position, const sf::Vector2f &size,
                                                      const sf::Color &fillColor, const sf::Color &outlineColor,
                                                      float outlineThickness, CoordinateMode mode)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    sf::Vector2f finalPos = (mode == CoordinateMode::Relative) ? sf::Vector2f{scaleMgr.ScaledReferenceX(position.x),
                                                                              scaleMgr.ScaledReferenceY(position.y)}
                                                               : position;

    sf::Vector2f finalSize = (mode == CoordinateMode::Relative)
                                 ? sf::Vector2f{scaleMgr.ScaledReferenceX(size.x), scaleMgr.ScaledReferenceY(size.y)}
                                 : size;

    auto panel = std::make_shared<UIHUDPanel>(finalPos, finalSize);
    panel->SetFillColor(fillColor);
    panel->SetOutlineColor(outlineColor);
    panel->SetOutlineThickness(outlineThickness);
    panel->SetLayoutMode(LayoutMode::Horizontal);
    panel->SetInternalPadding(scaleMgr.ScaledReferenceX(BASE_GROUPBOX_INTERNAL_PAD_RATIO));
    panel->SetEdgePadding(scaleMgr.ScaledReferenceX(BASE_GROUPBOX_EDGE_PAD_RATIO));
    panel->SetCenterChildren(false);

    return panel;
}

/// @brief Creates a UI FillableGauge element, given the custom input structure config.
/// @param cfg Configuration structure holding relevent data for a FillableGauge
/// @return safe pointer to a UIFillableGauge
std::shared_ptr<UIFillableGauge> UIFactory::CreateFillableGauge(const FillableGaugeConfig &cfg)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const sf::Vector2f scaledPos{scaleMgr.ScaledReferenceX(cfg.relativePosition.x),
                                 scaleMgr.ScaledReferenceY(cfg.relativePosition.y)};
    const sf::Vector2f scaledSize{scaleMgr.ScaledReferenceX(cfg.relativeSize.x),
                                  scaleMgr.ScaledReferenceY(cfg.relativeSize.y)};

    auto gauge = std::make_shared<UIFillableGauge>(scaledPos, scaledSize);
    gauge->SetScheme(cfg.colorScheme);
    gauge->SetOrientation(cfg.orientation);
    gauge->SetBorder(cfg.borderThickness, cfg.borderColor);
    gauge->SetValue(cfg.initialValue);

    if (cfg.showPercentage)
    {
        gauge->SetShowPercentage(true);
    }

    if (cfg.showTitle)
    {
        gauge->SetShowTitleLabel(cfg.titleText, scaleMgr.ScaleFont(cfg.titleFontSize),
                                 scaleMgr.ScaleX(cfg.titlePadding), cfg.titleScheme, cfg.titlePosition);
    }

    gauge->SetSize({scaledSize});

    return gauge;
}

/// @brief Creates a UI Icon element, given the custom input parameters.
/// @param textureKey Asset path to sprite.
/// @param position Location to instantiate at.
/// @param size Size adjustment for icon.
/// @param onClick Callback function if relevent, often onClick.
/// @return safe pointer to a UIIcon.
std::shared_ptr<UIIcon> UIFactory::CreateIcon(const std::string &textureKey, const sf::Vector2f &position,
                                              const sf::Vector2f &size, std::function<void()> onClick)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(size.x),
                            ResolutionScaleManager::Instance().ScaleY(size.y));

    auto icon = std::make_shared<UIIcon>(scaledSize, position);
    icon->SetTextureSkin(textureKey);

    if (onClick)
    {
        icon->SetOnClick(onClick);
    }

    return icon;
}
