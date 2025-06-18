// ============================================================================
//  File        : UIFillableGauge.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-13
//  Description : Visual bar UI element that fills based on a value
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIFillableGauge.h"

UIFillableGauge::UIFillableGauge(const sf::Vector2f &position, const sf::Vector2f &size)
    : m_position(position), m_size(size)
{
    m_backgroundBar.setPosition(position);
    m_backgroundBar.setSize(size);
    m_backgroundBar.setFillColor(m_barBackgroundColor);

    m_fillBar.setPosition(position);
    m_fillBar.setSize({size.x * m_value, size.y});
    m_fillBar.setFillColor(m_barFillColor);
}

void UIFillableGauge::Update(const sf::Vector2i &, bool, bool, float)
{
}

bool UIFillableGauge::Contains(const sf::Vector2i &point) const
{
    return m_backgroundBar.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

void UIFillableGauge::SetPosition(const sf::Vector2f &position)
{
    m_position = position;
    m_backgroundBar.setPosition(position);
    m_fillBar.setPosition(position);
    m_border.setPosition(m_position);
}

sf::Vector2f UIFillableGauge::GetPosition() const
{
    return m_position;
}

void UIFillableGauge::SetSize(const sf::Vector2f &size)
{
    m_size = size;
    m_backgroundBar.setSize(size);
    m_fillBar.setSize({size.x * m_value, size.y});
    m_border.setSize(m_size);
}

sf::Vector2f UIFillableGauge::GetSize() const
{
    return m_size;
}

void UIFillableGauge::SetValue(float percentage)
{
    m_value = std::clamp(percentage, 0.f, 1.f);
    m_fillBar.setSize({m_size.x * m_value, m_size.y});
}

void UIFillableGauge::SetScheme(GaugeColorScheme scheme)
{
    switch (scheme)
    {
        case GaugeColorScheme::Health:
            m_barBackgroundColor = sf::Color(60, 0, 0); // Dark red
            m_barFillColor = sf::Color(220, 30, 30);    // Bright red
            break;

        case GaugeColorScheme::Gas:
            m_barBackgroundColor = sf::Color(20, 20, 60); // Dark blue
            m_barFillColor = sf::Color(60, 150, 255);     // Light gas blue
            break;

        case GaugeColorScheme::Mana:
            m_barBackgroundColor = sf::Color(10, 10, 30); // Navy
            m_barFillColor = sf::Color(90, 90, 250);      // Mana blue
            break;

        case GaugeColorScheme::Default:
        default:
            m_barBackgroundColor = sf::Color(40, 40, 40); // Neutral dark
            m_barFillColor = sf::Color(200, 200, 200);    // Neutral light
            break;
    }

    m_fillBar.setFillColor(m_barFillColor);
    m_backgroundBar.setFillColor(m_barBackgroundColor);
}

void UIFillableGauge::SetOrientation(LayoutMode orientation)
{
    m_orientation = orientation;
    UpdateFillVisual();
}

void UIFillableGauge::SetBorder(float thickness, const sf::Color &color)
{
    m_drawBorder = thickness > 0.f;
    m_border.setSize(m_size);
    m_border.setPosition(m_position);
    m_border.setFillColor(sf::Color::Transparent);
    m_border.setOutlineThickness(thickness);
    m_border.setOutlineColor(color);
}

void UIFillableGauge::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_backgroundBar, states);
    target.draw(m_fillBar, states);

    if (m_drawBorder)
    {
        target.draw(m_border, states);
    }
}

void UIFillableGauge::UpdateFillVisual()
{
    sf::Vector2f fillSize = m_size;

    if (m_orientation == LayoutMode::Horizontal)
    {
        fillSize.x *= m_value;
    }

    else // Vertical
    {
        fillSize.y *= m_value;
        fillSize.y = std::max(1.f, fillSize.y); // avoid 0 height
    }

    m_fillBar.setSize(fillSize);
}
