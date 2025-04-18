// ============================================================================
//  File        : Button.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-17
//  Description : Represents a clickable UI button with hover and press states.
//                Can be used in menus and interactive scenes.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "Button.h"
#include "Macros.h"

Button::Button(const sf::Vector2f &position, const sf::Vector2f &size)
{
    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(sf::Color::White); // Default color

    m_idleColor = sf::Color(200, 200, 200);
    m_hoverColor = sf::Color(160, 160, 255);
    m_activeColor = sf::Color(100, 100, 255);
}

// Sets the internal text related members for this Button.
void Button::SetText(const std::string &text, const sf::Font &font, unsigned int size)
{
    m_label.setFont(font);
    m_label.setString(text);
    m_label.setCharacterSize(size);
    m_label.setFillColor(sf::Color::Black);

    // Center the label
    sf::FloatRect textRect = m_label.getLocalBounds();
    m_label.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

    sf::FloatRect shapeRect = m_shape.getGlobalBounds();
    m_label.setPosition(shapeRect.left + shapeRect.width / 2.f, shapeRect.top + shapeRect.height / 2.f);
}

// Sets the internal callback function for this Button, which responds to onClick.
void Button::SetCallback(std::function<void()> callback)
{
    m_onClick = std::move(callback);
}

// Manages the colors for this Button based on input parameters.
void Button::SetColors(const sf::Color &idle, const sf::Color &hover, const sf::Color &active)
{
    m_idleColor = idle;
    m_hoverColor = hover;
    m_activeColor = active;
    m_shape.setFillColor(idle);
}

// Update logic for this Button includes, isHovered, isPressed
void Button::Update(const sf::Vector2i &mousePosition, bool isMousePressed)
{
    sf::Vector2f mouse(mousePosition);
    bool wasHovered = m_isHovered;

    m_isHovered = m_shape.getGlobalBounds().contains(mouse);

    if (m_isHovered)
    {
        // Log on hover enter
        if (!wasHovered)
        {
            CT_LOG_DEBUG("Button hovered.");
        }

        // This gives a cool scaling effect on hovered buttons.
        m_shape.setScale(1.05f, 1.05f);
        m_label.setScale(1.05f, 1.05f);
        m_shape.setFillColor(isMousePressed ? m_activeColor : m_hoverColor);

        if (isMousePressed && !m_isPressed)
        {
            m_isPressed = true;

            CT_LOG_INFO("Button clicked.");
            if (m_onClick)
            {
                m_onClick();
            }
        }
        else if (!isMousePressed)
        {
            m_isPressed = false;
        }
    }
    else
    {
        if (wasHovered)
        {
            CT_LOG_DEBUG("Button unhovered.");
        }

        m_shape.setScale(1.f, 1.f);
        m_label.setScale(1.f, 1.f);
        m_shape.setFillColor(m_idleColor);
        m_isPressed = false;
    }
}

// Draw this Button to the Renderable Target.
void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
    target.draw(m_label, states);
}
