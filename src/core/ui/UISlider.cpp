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
#include <algorithm>
#include <sstream>

UISlider::UISlider(const std::string &label, float minValue, float maxValue, float initialValue,
                   const sf::Vector2f &position, const sf::Vector2f &size, std::function<void(float)> onChange)
    : m_label(label), m_min(minValue), m_max(maxValue), m_value(std::clamp(initialValue, minValue, maxValue)),
      m_position(position), m_size(size), m_dragging(false), m_onChange(std::move(onChange))
{
    // Setup visuals
    SetupGraphics();
}

void UISlider::SetupGraphics()
{
    // Background bar
    m_barBackground.setSize(m_size);
    m_barBackground.setPosition(m_position);
    m_barBackground.setFillColor(sf::Color(100, 100, 100));

    // Foreground bar (volume filled)
    float normalized = GetNormalizedValue();
    m_barForeground.setSize({m_size.x * normalized, m_size.y});
    m_barForeground.setPosition(m_position);
    m_barForeground.setFillColor(sf::Color(200, 200, 200));

    // Knob
    float knobRadius = std::max(6.f, m_size.y / 2.f);
    m_knob.setRadius(knobRadius);
    m_knob.setOrigin(knobRadius, knobRadius);
    m_knob.setPosition(ValueToPosition(m_value), m_position.y + m_size.y / 2);
    m_knob.setFillColor(sf::Color::Green);

    // Label
    std::stringstream ss;
    ss << m_label << ": " << static_cast<int>(m_value);
    m_labelText.setString(ss.str());
    m_labelText.setCharacterSize(14);
    m_labelText.setFillColor(sf::Color::White);
    m_labelText.setPosition(m_position.x, m_position.y - 20);
}

float UISlider::GetNormalizedValue() const
{
    return (m_value - m_min) / (m_max - m_min);
}

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

bool UISlider::Contains(const sf::Vector2i &point) const
{
    return m_barBackground.getGlobalBounds().contains(static_cast<sf::Vector2f>(point)) ||
           m_knob.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

void UISlider::SetPosition(const sf::Vector2f &position)
{
    m_position = position;

    m_barBackground.setPosition(m_position);
    m_barForeground.setPosition(m_position);

    float normalized = GetNormalizedValue();
    m_knob.setPosition(m_position.x + m_size.x * normalized, m_position.y + m_size.y / 2.f);

    m_labelText.setPosition(position + m_labelOffset);
}

sf::Vector2f UISlider::GetPosition() const
{
    return m_position;
}

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

sf::Vector2f UISlider::GetSize() const
{
    return m_size;
}

void UISlider::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_barBackground, states);
    target.draw(m_barForeground, states);
    target.draw(m_knob, states);
    target.draw(m_labelText, states);
}

void UISlider::SetFont(const sf::Font &font)
{
    m_labelText.setFont(font);
}

void UISlider::SetFontSize(unsigned int size)
{
    m_labelText.setCharacterSize(size);
}

void UISlider::SetTitlePositionOffset(const sf::Vector2f &offset)
{
    m_labelOffset = offset;
    m_labelText.setPosition(m_position + m_labelOffset);
}

void UISlider::SetColor(const sf::Color &barColor, const sf::Color &knobColor)
{
    m_barForeground.setFillColor(barColor);
    m_knob.setFillColor(knobColor);
}

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

float UISlider::GetValue() const
{
    return m_value;
}

float UISlider::ValueToPosition(float value) const
{
    float normalized = (value - m_min) / (m_max - m_min);
    return m_position.x + normalized * m_size.x;
}

float UISlider::PositionToValue(float x) const
{
    float relative = (x - m_position.x) / m_size.x;
    return std::clamp(m_min + relative * (m_max - m_min), m_min, m_max);
}
