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
#include <algorithm>
#include <sstream>

UISlider::UISlider(const std::string &label, float minValue, float maxValue, float initialValue,
                   const sf::Vector2f &position, const sf::Vector2f &size, std::function<void(float)> onChange)
    : m_label(label), m_min(minValue), m_max(maxValue), m_value(initialValue), m_position(position), m_size(size),
      m_dragging(false), m_onChange(onChange)
{
    m_barBackground.setSize(m_size);
    m_barBackground.setPosition(m_position);
    m_barBackground.setFillColor(sf::Color(100, 100, 100));

    m_barForeground.setSize({ValueToPosition(m_value) - m_position.x, m_size.y});
    m_barForeground.setPosition(m_position);
    m_barForeground.setFillColor(sf::Color(200, 200, 200));

    m_knob.setRadius(m_size.y / 2);
    m_knob.setOrigin(m_knob.getRadius(), m_knob.getRadius());
    m_knob.setPosition(ValueToPosition(m_value), m_position.y + m_size.y / 2);
    m_knob.setFillColor(sf::Color::White);

    m_labelText.setString(m_label + ": " + std::to_string(static_cast<int>(m_value)));
    m_labelText.setCharacterSize(14);
    m_labelText.setPosition(m_position.x, m_position.y - 20);
    m_labelText.setFillColor(sf::Color::White);
}

// Update the slider's state based on mouse input
void UISlider::Update(const sf::Vector2i &mousePos, bool isMousePressed)
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
        m_barForeground.setSize({newX - m_position.x, m_size.y});
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

// Returns true if the slider's knob or bar contains the point
bool UISlider::Contains(const sf::Vector2i &point) const
{
    return m_barBackground.getGlobalBounds().contains(static_cast<sf::Vector2f>(point)) ||
           m_knob.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

// Draws the slider's components
void UISlider::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_barBackground, states);
    target.draw(m_barForeground, states);
    target.draw(m_knob, states);
    target.draw(m_labelText, states);
}

// Set a new font for the label
void UISlider::SetFont(const sf::Font &font)
{
    m_labelText.setFont(font);
}

// Set custom colors for the bar and knob
void UISlider::SetColor(const sf::Color &barColor, const sf::Color &knobColor)
{
    m_barForeground.setFillColor(barColor);
    m_knob.setFillColor(knobColor);
}

// Set the slider value directly and update visuals
void UISlider::SetValue(float value)
{
    m_value = std::clamp(value, m_min, m_max);
    float posX = ValueToPosition(m_value);
    m_barForeground.setSize({posX - m_position.x, m_size.y});
    m_knob.setPosition(posX, m_position.y + m_size.y / 2);

    std::stringstream ss;
    ss << m_label << ": " << static_cast<int>(m_value);
    m_labelText.setString(ss.str());
}

// Get the current slider value
float UISlider::GetValue() const
{
    return m_value;
}

// Converts a value to its horizontal position on the slider
float UISlider::ValueToPosition(float value) const
{
    float range = m_max - m_min;
    float normalized = (value - m_min) / range;
    return m_position.x + normalized * m_size.x;
}

// Converts a horizontal position to its corresponding slider value
float UISlider::PositionToValue(float x) const
{
    float relative = (x - m_position.x) / m_size.x;
    return m_min + relative * (m_max - m_min);
}
