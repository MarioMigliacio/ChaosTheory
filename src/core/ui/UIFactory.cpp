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
#include "Button.h"
#include "RadioButton.h"
#include "ResolutionScaleManager.h"
#include "SettingsManager.h"
#include "UISlider.h"
#include "UITextLabel.h"

UIFactory &UIFactory::Instance()
{
    static UIFactory instance;
    return instance;
}

// Manufactures a 'type' of Button based on input parameters, returns a smart pointer.
std::shared_ptr<UIElement> UIFactory::CreateButton(ButtonType type, const sf::Vector2f &position,
                                                   const sf::Vector2f &size, const std::string &label,
                                                   std::function<void()> onClick)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(size.x),
                            ResolutionScaleManager::Instance().ScaleY(size.y));
    unsigned int scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(18);

    switch (type)
    {
        case ButtonType::Classic:
        default:
        {
            auto button = std::make_shared<Button>(position, scaledSize);

            button->SetText(label, AssetManager::Instance().GetFont("Default.ttf"), scaledFontSize);
            button->SetCallback(std::move(onClick));
            button->SetIdleColor(DEFAULT_IDLE_COLOR);
            button->SetHoverColor(DEFAULT_HOVER_COLOR);
            button->SetActiveColor(DEFAULT_ACTIVE_COLOR);
            button->SetTextColor(DEFAULT_TEXT_COLOR);
            button->SetHoverScale(1.05f);

            return button;
        }

        case ButtonType::Radio:
        {
            auto radio = std::make_shared<RadioButton>(position, scaledSize);

            radio->SetText(label, AssetManager::Instance().GetFont("Default.ttf"), scaledFontSize);
            radio->SetCallback(std::move(onClick));
            radio->SetTextColor(DEFAULT_TEXT_COLOR);
            radio->SetHoverColor(DEFAULT_HOVER_COLOR);
            radio->SetSelectedColor(DEFAULT_SELECTED_COLOR, DEFAULT_SELECTED_TEXT_COLOR);

            return radio;
        }
    }
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

std::shared_ptr<UIArrow> UIFactory::CreateArrow(float x, float y, ArrowDirection direction)
{
    return std::make_shared<UIArrow>(sf::Vector2f{x, y}, direction);
}

// Creates a standard vertical GroupBox occupying relative screen space with automatic scaling.
std::shared_ptr<GroupBox> UIFactory::CreateGroupBox(const std::string &title, const sf::Vector2f &relativePos,
                                                    const sf::Vector2f &relativeSize)
{
    return CreateGroupBox(title, relativePos, relativeSize,
                          LayoutMode::Vertical, // vertical layout is nice for controls
                          true,                 // center children
                          0.1f,                 // use a default provided internal padding
                          0.075f,               // use a defaut provided edge padding
                          0                     // use default provided font size.
    );
}

// Fully configurable GroupBox with layout, alignment, padding, and font size.
std::shared_ptr<GroupBox> UIFactory::CreateGroupBox(const std::string &title, const sf::Vector2f &relativePosition,
                                                    const sf::Vector2f &relativeSize, LayoutMode layoutMode,
                                                    bool centerChildren, float internalPadRatio, float edgePadRatio,
                                                    unsigned int fontSize)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const sf::Vector2f scaledPos{scaleMgr.ScaledReferenceX(relativePosition.x),
                                 scaleMgr.ScaledReferenceY(relativePosition.y)};
    const sf::Vector2f scaledSize{scaleMgr.ScaledReferenceX(relativeSize.x), scaleMgr.ScaledReferenceY(relativeSize.y)};

    const float internalPadding = scaleMgr.ScaledReferenceY(internalPadRatio);
    const float edgePadding = scaleMgr.ScaledReferenceY(edgePadRatio);

    auto groupBox = std::make_shared<GroupBox>(scaledPos, scaledSize);
    groupBox->SetTitle(title, AssetManager::Instance().GetFont("Default.ttf"),
                       fontSize > 0 ? fontSize : scaleMgr.ScaleFont(24));
    groupBox->SetLayoutMode(layoutMode);
    groupBox->SetCenterChildren(centerChildren);
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
