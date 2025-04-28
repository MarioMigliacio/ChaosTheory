#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class RadioButton : public UIElement
{
  public:
    RadioButton(const sf::Vector2f &position, const sf::Vector2f &size, const std::string &label,
                std::function<void()> onSelect);
    ~RadioButton() override = default;

    RadioButton(const RadioButton &) = delete;
    RadioButton &operator=(const RadioButton &) = delete;
    RadioButton(RadioButton &&) noexcept = default;
    RadioButton &operator=(RadioButton &&) noexcept = default;

    void SetSelected(bool selected);
    bool IsSelected() const;

    void Update(const sf::Vector2i &mousePosition, bool isMousePressed) override;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    void SetSize(const sf::Vector2f &size) override;

  private:
    void CenterLabel();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void HandleClickLogic(bool isMousePressed);
    void UpdateVisualState();

  private:
    sf::RectangleShape m_shape;
    sf::Text m_label;
    std::function<void()> m_onSelect;

    sf::Color m_currentFillColor;
    sf::Color m_currentTextColor;

    bool m_isSelected = false;
    bool m_isHovered = false;
};
