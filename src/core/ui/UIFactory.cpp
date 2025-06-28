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
/// @param cfg ButtonConfig.
/// @return safe pointer to a UIButton.
std::shared_ptr<UIButton> UIFactory::CreateButton(const ButtonConfig &cfg)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(cfg.size.x),
                            ResolutionScaleManager::Instance().ScaleY(cfg.size.y));
    unsigned int scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(BUTTON_DEFAULT_FONT_SIZE);

    auto btn = std::make_shared<UIButton>(cfg.position, scaledSize);

    btn->SetText(cfg.label, *AssetManager::Instance().GetFont(FontAssets::DefaultFontKey), scaledFontSize);
    btn->SetTextColor(BUTTON_DEFAULT_TEXT_COLOR);
    btn->SetCallback(std::move(cfg.onClick));
    btn->SetIdleColor(BUTTON_DEFAULT_IDLE_COLOR);
    btn->SetHoverColor(BUTTON_DEFAULT_HOVER_COLOR);
    btn->SetActiveColor(BUTTON_DEFAULT_ACTIVE_COLOR);

    btn->SetHoverScale(1.05f);

    return btn;
}

/// @brief Creates a UI Selectable Button element, given the custom input parameters.
/// @param cfg SelectableButtonConfig.
/// @return safe pointer to a UISelectableButton.
std::shared_ptr<UISelectableButton> UIFactory::CreateSelectableButton(const SelectableButtonConfig &cfg)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(cfg.size.x),
                            ResolutionScaleManager::Instance().ScaleY(cfg.size.y));
    unsigned int scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(SELECTABLE_BUTTON_DEFAULT_FONT_SIZE);

    auto btn = std::make_shared<UISelectableButton>(cfg.position, scaledSize);

    btn->SetText(cfg.label, *AssetManager::Instance().GetFont(FontAssets::DefaultFontKey), scaledFontSize);
    btn->SetTextColor(BUTTON_DEFAULT_TEXT_COLOR);
    btn->SetCallback(std::move(cfg.onClick));
    btn->SetHoverColor(BUTTON_DEFAULT_HOVER_COLOR);
    btn->SetSelectedColor(BUTTON_DEFAULT_SELECTED_COLOR, BUTTON_DEFAULT_SELECTED_TEXT_COLOR);

    return btn;
}

/// @brief Generates a SkinnableButton ui element.
/// @param cfg SkinnableButtonConfig.
/// @return Smart pointer to a UISkinnableButton.
std::shared_ptr<UISkinnableButton> UIFactory::CreateSkinnableButton(const SkinnableButtonConfig &cfg)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(cfg.size.x),
                            ResolutionScaleManager::Instance().ScaleY(cfg.size.y));
    unsigned int scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(SKINNABLE_BUTTON_DEFAULT_FONT_SIZE);

    auto btn = std::make_shared<UISkinnableButton>(cfg.position, scaledSize);

    btn->SetText(cfg.label, *AssetManager::Instance().GetFont(FontAssets::DefaultFontKey), scaledFontSize);
    btn->SetTextureSkins(cfg.idleTexture, cfg.hoverTexture);
    btn->ApplySkinnableButtonTextStyle(cfg.scheme);
    btn->SetCallback(cfg.onClick);

    return btn;
}

/// @brief Creates a UI Slider element, given the custom input parameters.
/// @param cfg SliderConfig.
/// @return safe pointer to a UISlider.
std::shared_ptr<UISlider> UIFactory::CreateSlider(const SliderConfig &cfg)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    // We explicitely do scale the slider UI elements position because it impacts the slider background and knob
    const sf::Vector2f scaledPos = {scaleMgr.ScaledReferenceX(cfg.position.x),
                                    scaleMgr.ScaledReferenceY(cfg.position.y)};

    const auto scaledSize = sf::Vector2f(scaleMgr.ScaledReferenceX(cfg.size.x), scaleMgr.ScaleY(cfg.size.y));
    const auto scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(BASE_SLIDER_TITLE_FONT_SIZE);

    auto slider = std::make_shared<UISlider>(cfg.label, cfg.minValue, cfg.maxValue, cfg.initialValue, scaledPos,
                                             scaledSize, cfg.onChange);
    slider->SetFont(*AssetManager::Instance().GetFont(FontAssets::DefaultFontKey));
    slider->SetFontSize(scaledFontSize);
    slider->SetTitlePositionOffset(sf::Vector2f(0.f, -ResolutionScaleManager::Instance().ScaleY(BASE_SLIDER_OFFSET_Y)));

    return slider;
}

/// @brief Creates a UI Arrow element, given the custom input parameters.
/// @param cfg ArrowConfig.
/// @return Safe pointer to a UIArrow.
std::shared_ptr<UIArrow> UIFactory::CreateArrow(const ArrowConfig &cfg)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(cfg.size.x),
                            ResolutionScaleManager::Instance().ScaleY(cfg.size.y));

    auto arrow = std::make_shared<UIArrow>(cfg.position, scaledSize, cfg.direction);
    arrow->SetOnClick(cfg.onClick);
    arrow->SetTextureSkin(cfg.textureKey);
    arrow->SetSize(scaledSize);

    return arrow;
}

/// @brief Creates a UI UIGroupBox element, given the custom input parameters.
/// @param cfg GroupBoxConfig.
/// @return safe pointer to a UIGroupBox.
std::shared_ptr<UIGroupBox> UIFactory::CreateGroupBox(const GroupBoxConfig &cfg)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const sf::Vector2f scaledPos{scaleMgr.ScaledReferenceX(cfg.position.x), scaleMgr.ScaledReferenceY(cfg.position.y)};
    const sf::Vector2f scaledSize{scaleMgr.ScaledReferenceX(cfg.size.x), scaleMgr.ScaledReferenceY(cfg.size.y)};

    const float internalPadding = scaleMgr.ScaledReferenceY(BASE_GROUPBOX_INTERNAL_PAD_RATIO);
    const float edgePadding = scaleMgr.ScaledReferenceY(BASE_GROUPBOX_EDGE_PAD_RATIO);

    auto groupBox = std::make_shared<UIGroupBox>(scaledPos, scaledSize);

    if (cfg.useTitle)
    {
        groupBox->SetTitle(cfg.title, *AssetManager::Instance().GetFont(FontAssets::DefaultFontKey),
                           scaleMgr.ScaleFont(BASE_GROUPBOX_FONT_SIZE), cfg.centerOrigin, cfg.scheme);
    }

    groupBox->SetLayoutMode(LayoutMode::Vertical);
    groupBox->SetCenterChildren(true);
    groupBox->SetInternalPadding(internalPadding);
    groupBox->SetEdgePadding(edgePadding);

    return groupBox;
}

/// @brief Creates a UI UITextLabel element, given the custom input parameters.
/// @param cfg TextLabelConfig.
/// @return Safe pointer to a UITextLabel.
std::shared_ptr<UITextLabel> UIFactory::CreateTextLabel(const TextLabelConfig &cfg)
{
    const float scaledOutline = ResolutionScaleManager::Instance().ScaleX(DEFAULT_TEXT_LABEL_BORDER_THICKNESS);

    auto label = std::make_shared<UITextLabel>(cfg.text, *AssetManager::Instance().GetFont(FontAssets::DefaultFontKey),
                                               cfg.fontSize, cfg.position, cfg.centerOrigin);
    label->ApplyTextLabelStyle(cfg.scheme, scaledOutline);

    return label;
}

/// @brief Creates a UI UIToastMessage element, given the custom input parameters.
/// @param cfg ToastConfig.
/// @return safe pointer to a UIToastMessage.
std::shared_ptr<UIToastMessage> UIFactory::CreateToastMessage(const ToastMessageConfig &cfg)
{
    const float scaledOutline = ResolutionScaleManager::Instance().ScaleX(DEFAULT_TEXT_LABEL_BORDER_THICKNESS);
    const unsigned int fontSize = ResolutionScaleManager::Instance().ScaleFont(cfg.baseFontSize);

    auto toast =
        std::make_shared<UIToastMessage>(cfg.text, cfg.position, fontSize, cfg.duration, cfg.centerOrigin, cfg.scheme);
    toast->ApplyStyle(cfg.scheme, scaledOutline);

    return toast;
}

/// @brief Creates a UI HUDPanel element, given the custom input parameters.
/// @param cfg HUDPanelConfig.
/// @return safe pointer to a UIHUDPanel.
std::shared_ptr<UIHUDPanel> UIFactory::CreateHUDPanel(const HUDPanelConfig &cfg)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const sf::Vector2f scaledPos{scaleMgr.ScaledReferenceX(cfg.position.x), scaleMgr.ScaledReferenceY(cfg.position.y)};
    const sf::Vector2f scaledSize{scaleMgr.ScaledReferenceX(cfg.size.x), scaleMgr.ScaledReferenceY(cfg.size.y)};

    auto panel = std::make_shared<UIHUDPanel>(scaledPos, scaledSize);
    panel->SetFillColor(cfg.fillColor);
    panel->SetOutlineColor(cfg.outlineColor);
    panel->SetOutlineThickness(cfg.outlineThickness);
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

    const sf::Vector2f scaledPos{scaleMgr.ScaledReferenceX(cfg.position.x), scaleMgr.ScaledReferenceY(cfg.position.y)};
    const sf::Vector2f scaledSize{scaleMgr.ScaledReferenceX(cfg.size.x), scaleMgr.ScaledReferenceY(cfg.size.y)};

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
/// @param cfg IconConfig.
/// @return safe pointer to a UIIcon.
std::shared_ptr<UIIcon> UIFactory::CreateIcon(const IconConfig &cfg)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(cfg.size.x),
                            ResolutionScaleManager::Instance().ScaleY(cfg.size.y));

    auto icon = std::make_shared<UIIcon>(scaledSize, cfg.position);
    icon->SetTextureSkin(cfg.textureKey);
    icon->SetIconType(cfg.type);
    icon->StartFalling();

    return icon;
}
