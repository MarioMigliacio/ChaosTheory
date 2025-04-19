// ============================================================================
//  File        : RoundedButton.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-18
//  Description : Represents a clickable UI button with hover and press states.
//                Can be used in menus and interactive scenes.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "RoundedButton.h"
#include "Macros.h"
#include <cmath>

RoundedButton::RoundedButton(const sf::Vector2f &position, const sf::Vector2f &size)
{
    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(m_idleColor); // Default color
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color::Black);

    m_label.setPosition(position.x + 10.f, position.y + 10.f);
}

// Sets the internal text related members for this Button.
void RoundedButton::SetText(const std::string &text, const sf::Font &font, unsigned int size)
{
    m_label.setFont(font);
    m_label.setString(text);
    m_label.setCharacterSize(size);
    m_label.setFillColor(sf::Color::White);

    // Center text within button
    sf::FloatRect textBounds = m_label.getLocalBounds();
    sf::Vector2f shapePos = m_shape.getPosition();
    sf::Vector2f shapeSize = m_shape.getSize();

    m_label.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);

    m_label.setPosition(shapePos.x + shapeSize.x / 2.0f, shapePos.y + shapeSize.y / 2.0f);
}

// Sets the internal callback function for this Button, which responds to onClick.
void RoundedButton::SetCallback(std::function<void()> callback)
{
    m_onClick = std::move(callback);
}

// Manages the colors for this Button based on input parameters.
void RoundedButton::SetColors(const sf::Color &idle, const sf::Color &hover, const sf::Color &active)
{
    m_idleColor = idle;
    m_hoverColor = hover;
    m_activeColor = active;

    m_shape.setFillColor(m_idleColor);
}

// Sets the corner radii for this Button, based on input radius.
void RoundedButton::SetCornerRadius(float radius)
{
    m_cornerRadius = radius;
}

// Update logic for this Button includes, isHovered, isPressed
void RoundedButton::Update(const sf::Vector2i &mousePosition, bool isMousePressed)
{
    m_isHovered = Contains(mousePosition);

    if (m_isHovered)
    {
        m_shape.setFillColor(isMousePressed ? m_activeColor : m_hoverColor);

        if (isMousePressed && !m_isPressed)
        {
            m_isPressed = true;
        }
        else if (!isMousePressed && m_isPressed)
        {
            m_isPressed = false;

            if (m_onClick)
            {
                CT_LOG_INFO("RoundedButton clicked: '{}'", m_label.getString().toAnsiString());
                m_onClick();
            }
        }
    }
    else
    {
        m_shape.setFillColor(m_idleColor);
        m_isPressed = false;
    }

    // Scale effect
    float scale = m_isHovered ? 1.05f : 1.0f;
    m_shape.setScale(scale, scale);
    m_label.setScale(scale, scale);
}

// Returns whether or not the point is within the bounds of this Button.
bool RoundedButton::Contains(const sf::Vector2i &point) const
{
    return m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

// Draw this Button to the Renderable Target.
void RoundedButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (!m_label.getFont())
    {
        CT_LOG_ERROR("RoundedButton label has no font set!");
        return;
    }
    target.draw(m_shape, states);
    target.draw(m_label, states);
}
