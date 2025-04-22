// UIFactory.cpp

#include "UIFactory.h"
#include "AssetManager.h"
#include "Button.h"
#include "RoundedButton.h"

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
        case ButtonType::Rounded:
        {
            auto btn = std::make_shared<RoundedButton>(position, size);

            btn->SetText(label, font);
            btn->SetColors(sf::Color::Magenta, sf::Color::Yellow, sf::Color::Red);
            btn->SetCallback(onClick);

            button = btn;

            break;
        }
    }

    return button;
}