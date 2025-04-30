// ============================================================================
//  File        : UISlider.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-23
//  Description : Represents a UI slider winteractable elements.
//                Can be used in menus and interactive scenes.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

// ============================================================================
//  Class       : UISlider
//  Purpose     : Manages this Slider logic at the ui level.
//
//  Responsibilities:
//      - Set button position
//      - Perform logic during onClick
//      - Display button specifics during render
//
// ============================================================================
class UISlider : public UIElement
{
  public:
    UISlider(const std::string &label, float minValue, float maxValue, float initialValue, const sf::Vector2f &position,
             const sf::Vector2f &size, std::function<void(float)> onChange);
    ~UISlider() = default;

    // Disable copy
    UISlider(const UISlider &) = delete;
    UISlider &operator=(const UISlider &) = delete;

    // Allow move
    UISlider(UISlider &&) noexcept = default;
    UISlider &operator=(UISlider &&) noexcept = default;

    void Update(const sf::Vector2i &mousePos, bool isMousePressed) override;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    void SetSize(const sf::Vector2f &size) override;

    void SetFont(const sf::Font &font);
    void SetColor(const sf::Color &barColor, const sf::Color &knobColor);
    void SetValue(float value);

    float GetValue() const;

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    float ValueToPosition(float value) const;
    float PositionToValue(float x) const;

  private:
    sf::RectangleShape m_barBackground;
    sf::RectangleShape m_barForeground;
    sf::CircleShape m_knob;

    sf::Text m_labelText;
    std::string m_label;

    sf::Vector2f m_position;
    sf::Vector2f m_size;

    float m_min;
    float m_max;
    float m_value;
    bool m_dragging;

        std::function<void(float)> m_onChange;
};
