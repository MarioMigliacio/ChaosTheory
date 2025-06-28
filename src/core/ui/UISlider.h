// ============================================================================
//  File        : UISlider.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-23
//  Description : Represents a UI slider with interactable elements.
//                Can be used in menus and interactive scenes.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Macros.h"
#include "UIElement.h"
#include "UITextLabel.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

/// @brief Data structure holding internal configurations useful for SliderConfig construction.
/// @param label string Label for Slider.
/// @param position Vector2f position for Slider.
/// @param size Vector2f size for Slider.
/// @param minValue flaot minimum value for Slider.
/// @param maxValue float maximum value for Slider.
/// @param initialValue float initial value for Slider.
/// @param onChange function callback to trigger for Slider.
struct SliderConfig
{
    std::string label;
    sf::Vector2f position;
    sf::Vector2f size;
    float minValue;
    float maxValue;
    float initialValue;
    std::function<void(float)> onChange;
};

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

    void SetupGraphics();

    void Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt) override;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

    void SetFont(const sf::Font &font);
    void SetFontSize(unsigned int size);
    void SetTitlePositionOffset(const sf::Vector2f &offset);

    void SetColor(const sf::Color &barColor, const sf::Color &knobColor);
    void SetValue(float value);
    float GetValue() const;

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    float ValueToPosition(float value) const;
    float PositionToValue(float x) const;
    float GetNormalizedValue() const;

  private:
    sf::RectangleShape m_barBackground;
    sf::RectangleShape m_barForeground;
    sf::CircleShape m_knob;
    std::string m_label;

    std::shared_ptr<UITextLabel> m_labelText;

    sf::Vector2f m_labelOffset;
    sf::Vector2f m_position;
    sf::Vector2f m_size;

    float m_min;
    float m_max;
    float m_value;
    bool m_dragging;

    std::function<void(float)> m_onChange;
};
