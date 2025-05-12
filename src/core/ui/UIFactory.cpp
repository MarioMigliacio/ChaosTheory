// ============================================================================
//  File        : UIFactory.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-17
//  Description : UIFactory is responsible for creating different types of
//                UI elements such as buttons, sliders, chatboxes, etc.
// ============================================================================

#include "UIFactory.h"
#include "AssetManager.h"
#include "Macros.h"
#include "ResolutionScaleManager.h"
#include "SettingsManager.h"

UIFactory &UIFactory::Instance()
{
    static UIFactory instance;
    return instance;
}

std::shared_ptr<UIButton> UIFactory::CreateButton(const sf::Vector2f &position, const sf::Vector2f &size,
                                                  const std::string &label, std::function<void()> onClick)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(size.x),
                            ResolutionScaleManager::Instance().ScaleY(size.y));
    unsigned int scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(18);

    auto button = std::make_shared<UIButton>(position, scaledSize);

    button->SetText(label, AssetManager::Instance().GetFont("Default.ttf"), scaledFontSize);
    button->SetCallback(std::move(onClick));
    button->SetIdleColor(BUTTON_DEFAULT_IDLE_COLOR);
    button->SetHoverColor(BUTTON_DEFAULT_HOVER_COLOR);
    button->SetActiveColor(BUTTON_DEFAULT_ACTIVE_COLOR);
    button->SetTextColor(BUTTON_DEFAULT_TEXT_COLOR);
    button->SetHoverScale(1.05f);

    return button;
}

std::shared_ptr<UISelectableButton> UIFactory::CreateSelectableButton(const sf::Vector2f &position,
                                                                      const sf::Vector2f &size,
                                                                      const std::string &label,
                                                                      std::function<void()> onClick)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(size.x),
                            ResolutionScaleManager::Instance().ScaleY(size.y));
    unsigned int scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(18);

    auto selectableButton = std::make_shared<UISelectableButton>(position, scaledSize);

    selectableButton->SetText(label, AssetManager::Instance().GetFont("Default.ttf"), scaledFontSize);
    selectableButton->SetCallback(std::move(onClick));
    selectableButton->SetTextColor(BUTTON_DEFAULT_TEXT_COLOR);
    selectableButton->SetHoverColor(BUTTON_DEFAULT_HOVER_COLOR);
    selectableButton->SetSelectedColor(BUTTON_DEFAULT_SELECTED_COLOR, BUTTON_DEFAULT_SELECTED_TEXT_COLOR);

    return selectableButton;
}

// Manufactures a Slider ui element based on input parameters, returns a smart pointer.
std::shared_ptr<UISlider> UIFactory::CreateSlider(const std::string &label, const sf::Vector2f &position,
                                                  const sf::Vector2f &size, float minValue, float maxValue,
                                                  float initialValue, std::function<void(float)> onChange)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    // We explicitely do scale the slider UI elements position because it impacts the slider background and knob
    const sf::Vector2f scaledPos = {scaleMgr.ScaledReferenceX(position.x), scaleMgr.ScaledReferenceY(position.y)};

    const auto scaledSize = sf::Vector2f(scaleMgr.ScaledReferenceX(size.x), scaleMgr.ScaleY(size.y));
    const auto scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(14);

    auto slider = std::make_shared<UISlider>(label, minValue, maxValue, initialValue, scaledPos, scaledSize, onChange);
    slider->SetFont(AssetManager::Instance().GetFont("Default.ttf"));
    slider->SetFontSize(scaledFontSize);
    slider->SetTitlePositionOffset(sf::Vector2f(0.f, -ResolutionScaleManager::Instance().ScaleY(24.f)));

    return slider;
}

std::shared_ptr<UIArrow> UIFactory::CreateArrow(float x, float y, ArrowDirection direction,
                                                std::function<void()> onClick)
{
    auto arrow = std::make_shared<UIArrow>(sf::Vector2f{x, y}, direction);
    arrow->SetOnClick(onClick);

    return arrow;
}

// Fully configurable UIGroupBox with layout, alignment, padding, and font size.
std::shared_ptr<UIGroupBox> UIFactory::CreateGroupBox(const std::string &title, const sf::Vector2f &relativePosition,
                                                      const sf::Vector2f &relativeSize)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const sf::Vector2f scaledPos{scaleMgr.ScaledReferenceX(relativePosition.x),
                                 scaleMgr.ScaledReferenceY(relativePosition.y)};
    const sf::Vector2f scaledSize{scaleMgr.ScaledReferenceX(relativeSize.x), scaleMgr.ScaledReferenceY(relativeSize.y)};

    const float internalPadding = scaleMgr.ScaledReferenceY(BASE_GROUPBOX_INTERNAL_PAD_RATIO);
    const float edgePadding = scaleMgr.ScaledReferenceY(BASE_GROUPBOX_EDGE_PAD_RATIO);

    auto groupBox = std::make_shared<UIGroupBox>(scaledPos, scaledSize);
    groupBox->SetTitle(title, AssetManager::Instance().GetFont("Default.ttf"),
                       scaleMgr.ScaleFont(BASE_GROUPBOX_FONT_SIZE));
    groupBox->SetLayoutMode(LayoutMode::Vertical); // safe default state
    groupBox->SetCenterChildren(true);             // safe default state
    groupBox->SetInternalPadding(internalPadding);
    groupBox->SetEdgePadding(edgePadding);

    return groupBox;
}

std::shared_ptr<UITextLabel> UIFactory::CreateTextLabel(const std::string &text, const sf::Vector2f &position,
                                                        unsigned int baseFontSize, bool centerOrigin)
{
    auto scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(baseFontSize);
    auto label =
        std::make_shared<UITextLabel>(text, AssetManager::Instance().GetFont("Default.ttf"), scaledFontSize, position);

    if (!centerOrigin)
        label->SetPosition(position); // no auto-centering

    return label;
}

std::shared_ptr<UIToastMessage> UIFactory::CreateToastMessage(const std::string &text, const sf::Vector2f &position,
                                                              float duration)
{
    const auto &font = AssetManager::Instance().GetFont("Default.ttf");
    unsigned int fontSize = ResolutionScaleManager::Instance().ScaleFont(18);
    sf::Color color = sf::Color::White;
    bool centerOrigin = true;

    auto toast = std::make_shared<UIToastMessage>(text, position, duration, font, fontSize, color, centerOrigin);
    toast->SetSize({0.f, 0.f}); // still required by interface

    return toast;
}
