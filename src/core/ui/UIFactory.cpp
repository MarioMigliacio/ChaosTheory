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
#include "SettingsManager.h"
#include "UISlider.h"

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
    switch (type)
    {
        case ButtonType::Classic:
        default:
        {
            auto button = std::make_shared<Button>(position, size);

            button->SetText(label, AssetManager::Instance().GetFont("Default.ttf"), 24);
            button->SetCallback(std::move(onClick));
            button->SetIdleColor(DEFAULT_IDLE_COLOR);
            button->SetHoverColor(DEFAULT_HOVER_COLOR);
            button->SetActiveColor(DEFAULT_ACTIVE_COLOR);
            button->SetTextColor(DEFAULT_TEXT_COLOR);
            button->SetHoverScale(1.05f); // Normal hover grow effect

            return button;

            break;
        }

        case ButtonType::Radio:
        {
            auto radio = std::make_shared<RadioButton>(position, size, label, std::move(onClick));
            radio->SetText(label, AssetManager::Instance().GetFont("Default.ttf"), 24);
            radio->SetCallback(std::move(onClick));
            radio->SetTextColor(DEFAULT_TEXT_COLOR);
            radio->SetHoverColor(DEFAULT_HOVER_COLOR);
            radio->SetSelectedColor(DEFAULT_SELECTED_COLOR, DEFAULT_SELECTED_TEXT_COLOR);

            return radio;

            break;
        }
    }
}

// Manufactures a Slider ui element based on input parameters, returns a smart pointer.
std::shared_ptr<UIElement> UIFactory::CreateSlider(const std::string &label, const sf::Vector2f &position,
                                                   const sf::Vector2f &size, float minValue, float maxValue,
                                                   float initialValue, std::function<void(float)> onChange)
{
    auto slider = std::make_shared<UISlider>(label, minValue, maxValue, initialValue, position, size, onChange);

    sf::Font &font = AssetManager::Instance().GetFont("Default.ttf");

    slider->SetFont(font);

    return slider;
}

std::shared_ptr<UIArrow> UIFactory::CreateArrow(float x, float y, ArrowDirection direction)
{
    return std::make_shared<UIArrow>(sf::Vector2f{x, y}, direction);
}
