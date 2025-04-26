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
    std::shared_ptr<UIElement> button;

    sf::Font &font = AssetManager::Instance().GetFont("Default.ttf");

    switch (type)
    {
        case ButtonType::Classic:
        default:
        {
            auto btn = std::make_shared<Button>(position, size);

            btn->SetText(label, font);
            btn->SetColors(sf::Color::Blue, sf::Color::Cyan, sf::Color::Green);
            btn->SetCallback(onClick);

            button = btn;

            break;
        }
    }

    return button;
}

std::shared_ptr<UIElement> UIFactory::CreateSlider(const std::string &label, const sf::Vector2f &position,
                                                   const sf::Vector2f &size, float minValue, float maxValue,
                                                   float initialValue, std::function<void(float)> onChange)
{
    auto slider = std::make_shared<UISlider>(label, minValue, maxValue, initialValue, position, size, onChange);

    sf::Font &font = AssetManager::Instance().GetFont("Default.ttf");

    slider->SetFont(font);

    return slider;
}