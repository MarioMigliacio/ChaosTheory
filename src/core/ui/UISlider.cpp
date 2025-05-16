// ============================================================================
//  File        : UISlider.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-22
//  Description : Implementation of UISlider element for interactive UI
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UISlider.h"
#include "Macros.h"
#include "UIPresets.h"
#include <algorithm>
#include <sstream>

/// @brief Constructor for the UISlider.
/// @param label String representation for this UISlider.
/// @param minValue minimum value.
/// @param maxValue maximum value.
/// @param initialValue starting value.
/// @param position starting position.
/// @param size starting size.
/// @param onChange function pointer to a callback, generally onChange for UISlider.
UISlider::UISlider(const std::string &label, float minValue, float maxValue, float initialValue,
                   const sf::Vector2f &position, const sf::Vector2f &size, std::function<void(float)> onChange)
    : m_label(label), m_min(minValue), m_max(maxValue), m_value(std::clamp(initialValue, minValue, maxValue)),
      m_position(position), m_size(size), m_dragging(false), m_onChange(std::move(onChange))
{
    // Setup visuals
    SetupGraphics();
}

/// @brief Establishes thhe necessary components that define a UISlider.
void UISlider::SetupGraphics()
{
    // Background bar
    m_barBackground.setSize(m_size);
    m_barBackground.setPosition(m_position);
    m_barBackground.setFillColor(BASE_SLIDER_BACK_COLOR);

    // Foreground bar (volume filled)
    float normalized = GetNormalizedValue();
    m_barForeground.setSize({m_size.x * normalized, m_size.y});
    m_barForeground.setPosition(m_position);
    m_barForeground.setFillColor(BASE_SLIDER_FILL_COLOR);

    // Knob
    float knobRadius = std::max(BASE_SLIDER_KNOB_RADIUS, m_size.y / 2.f);
    m_knob.setRadius(knobRadius);
    m_knob.setOrigin(knobRadius, knobRadius);
    m_knob.setPosition(ValueToPosition(m_value), m_position.y + m_size.y / 2);
    m_knob.setFillColor(BASE_SLIDER_KNOB_COLOR);

    // Label
    std::stringstream ss;
    ss << m_label << ": " << static_cast<int>(m_value);
    m_labelText.setString(ss.str());
    m_labelText.setCharacterSize(14);
    m_labelText.setFillColor(sf::Color::White);
    m_labelText.setPosition(m_position.x, m_position.y - 20);
}

/// @brief Returns a normalized value to be bound in min / max and value ratio.
/// @return normalized ratio between min, value, and max.
float UISlider::GetNormalizedValue() const
{
    return (m_value - m_min) / (m_max - m_min);
}

/// @brief Performs internal state management during a single frame.
/// @param mousePos Mouse Position.
/// @param isMousePressed IsMousePressed?
/// @param isMouseJustPressed IsMouseJustPressed?
/// @param dt delta time since last update.
void UISlider::Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt)
{
    sf::Vector2f mPos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if (isMousePressed && m_knob.getGlobalBounds().contains(mPos))
    {
        m_dragging = true;
    }
    else if (!isMousePressed)
    {
        m_dragging = false;
    }

    if (m_dragging)
    {
        float newX = std::clamp(mPos.x, m_position.x, m_position.x + m_size.x);
        m_value = PositionToValue(newX);

        float normalized = GetNormalizedValue();
        m_barForeground.setSize({m_size.x * normalized, m_size.y});
        m_knob.setPosition(newX, m_position.y + m_size.y / 2);

        std::stringstream ss;
        ss << m_label << ": " << static_cast<int>(m_value);
        m_labelText.setString(ss.str());

        if (m_onChange)
        {
            m_onChange(m_value);
        }
    }
}

/// @brief Returns whether or not the point is bound inside this UISlider.
/// @param point Point to compare against us.
/// @return true / false
bool UISlider::Contains(const sf::Vector2i &point) const
{
    return m_barBackground.getGlobalBounds().contains(static_cast<sf::Vector2f>(point)) ||
           m_knob.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

/// @brief Sets the internal position for this UISlider.
/// @param position new m_position.
void UISlider::SetPosition(const sf::Vector2f &position)
{
    m_position = position;

    m_barBackground.setPosition(m_position);
    m_barForeground.setPosition(m_position);

    float normalized = GetNormalizedValue();
    m_knob.setPosition(m_position.x + m_size.x * normalized, m_position.y + m_size.y / 2.f);

    m_labelText.setPosition(position + m_labelOffset);
}

/// @brief Returns the position of this UISlider.
/// @return m_position.
sf::Vector2f UISlider::GetPosition() const
{
    return m_position;
}

/// @brief Returns the size for this UISlider.
/// @param size m_size.
void UISlider::SetSize(const sf::Vector2f &size)
{
    m_size = size;

    m_barBackground.setSize(m_size);

    float normalized = GetNormalizedValue();
    m_barForeground.setSize({m_size.x * normalized, m_size.y});

    float knobRadius = std::max(6.f, m_size.y / 2.f);
    m_knob.setRadius(knobRadius);
    m_knob.setOrigin(knobRadius, knobRadius);
    m_knob.setPosition(ValueToPosition(m_value), m_position.y + m_size.y / 2);
}

/// @brief Returns the size of this UISlider.
/// @return m_size.
sf::Vector2f UISlider::GetSize() const
{
    return m_size;
}

/// @brief Draw this UISlider to the Renderable Target.
/// @param target render target.
/// @param states optional sf::RenderStates.
void UISlider::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_barBackground, states);
    target.draw(m_barForeground, states);
    target.draw(m_knob, states);
    target.draw(m_labelText, states);
}

/// @brief Sets the internal font for this UISlider.
/// @param font new m_labeltext.font.
void UISlider::SetFont(const sf::Font &font)
{
    m_labelText.setFont(font);
}

/// @brief Sets the internal font size for this UISlider.
/// @param size new m_labelText.size.
void UISlider::SetFontSize(unsigned int size)
{
    m_labelText.setCharacterSize(size);
}

/// @brief Sets the position for the Title on this UISlider.
/// @param offset position offset becomes new position + offset.
void UISlider::SetTitlePositionOffset(const sf::Vector2f &offset)
{
    m_labelOffset = offset;
    m_labelText.setPosition(m_position + m_labelOffset);
}

/// @brief Sets the foreground and knob color for  this UISlider.
/// @param barColor the new color for the bar foreground.
/// @param knobColor the new color for the knob.
void UISlider::SetColor(const sf::Color &barColor, const sf::Color &knobColor)
{
    m_barForeground.setFillColor(barColor);
    m_knob.setFillColor(knobColor);
}

/// @brief Sets the value for the UISlider.
/// @param value new m_value.
void UISlider::SetValue(float value)
{
    m_value = std::clamp(value, m_min, m_max);

    float normalized = GetNormalizedValue();
    m_barForeground.setSize({m_size.x * normalized, m_size.y});
    m_knob.setPosition(ValueToPosition(m_value), m_position.y + m_size.y / 2);

    std::stringstream ss;
    ss << m_label << ": " << static_cast<int>(m_value);
    m_labelText.setString(ss.str());
}

/// @brief Gets the value for this UISlider.
/// @return m_value.
float UISlider::GetValue() const
{
    return m_value;
}

/// @brief Returns the position value for this UISlider based on the input value.
/// @param value Value to normalize.
/// @return Normalized value to position.
float UISlider::ValueToPosition(float value) const
{
    float normalized = (value - m_min) / (m_max - m_min);
    return m_position.x + normalized * m_size.x;
}

/// @brief Returns the value for this UISlider based on the input position.
/// @param x X coordinate.
/// @return Value from position.
float UISlider::PositionToValue(float x) const
{
    float relative = (x - m_position.x) / m_size.x;
    return std::clamp(m_min + relative * (m_max - m_min), m_min, m_max);
}
