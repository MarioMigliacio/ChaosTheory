// ============================================================================
//  File        : Button.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-18
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
    m_shape.setFillColor(m_idleColor);
}

void Button::SetText(const std::string &text, const sf::Font &font, unsigned int size)
{
    m_label.setFont(font);
    m_label.setString(text);
    m_fontSize = size;
    m_label.setCharacterSize(m_fontSize);
    m_label.setFillColor(m_textColor);

    // Auto-fit text if too wide, nice to have!
    const float maxWidth = m_shape.getSize().x * 0.9f; // Leave a little margin
    sf::FloatRect textRect = m_label.getLocalBounds();

    while (textRect.width > maxWidth && m_fontSize > 8) // Don't go below readable size
    {
        m_fontSize -= 1;
        m_label.setCharacterSize(m_fontSize);
        textRect = m_label.getLocalBounds();
    }

    CenterLabel();
}

void Button::SetCallback(std::function<void()> callback)
{
    m_onClick = std::move(callback);
}

void Button::SetIdleColor(const sf::Color &color)
{
    m_idleColor = color;
    m_shape.setFillColor(m_idleColor);
}

void Button::SetHoverColor(const sf::Color &color)
{
    m_hoverColor = color;
}

void Button::SetActiveColor(const sf::Color &color)
{
    m_activeColor = color;
}

void Button::SetTextColor(const sf::Color &color)
{
    m_textColor = color;
    m_label.setFillColor(m_textColor);
}

void Button::SetFontSize(unsigned int size)
{
    m_fontSize = size;
    m_label.setCharacterSize(m_fontSize);
    CenterLabel();
}

void Button::SetHoverScale(float scale)
{
    m_hoverScale = scale;
}

void Button::Update(const sf::Vector2i &mousePosition, bool isMousePressed)
{
    sf::Vector2f mouse(mousePosition);
    bool wasHovered = m_isHovered;

    m_isHovered = m_shape.getGlobalBounds().contains(mouse);

    if (m_isHovered && !wasHovered)
    {
        CT_LOG_DEBUG("Button hovered.");
    }

    else if (!m_isHovered && wasHovered)
    {
        CT_LOG_DEBUG("Button unhovered.");
    }

    UpdateScale();
    UpdateFillColor(isMousePressed);
    UpdateTextColor();

    if (m_isHovered && m_enabled)
    {
        HandleClickLogic(isMousePressed);
    }

    else
    {
        m_isPressed = false;
    }
}

bool Button::Contains(const sf::Vector2i &point) const
{
    return m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

void Button::SetPosition(const sf::Vector2f &position)
{
    m_shape.setPosition(position);
    CenterLabel();
}

void Button::SetSize(const sf::Vector2f &size)
{
    m_shape.setSize(size);
    CenterLabel();
}

void Button::CenterLabel()
{
    sf::FloatRect textRect = m_label.getLocalBounds();
    m_label.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

    m_label.setPosition(m_shape.getPosition().x + m_shape.getSize().x / 2.f,
                        m_shape.getPosition().y + m_shape.getSize().y / 2.f);
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
    target.draw(m_label, states);
}

void Button::UpdateScale()
{
    if (m_isHovered && m_enabled)
    {
        m_shape.setScale(m_hoverScale, m_hoverScale);
        m_label.setScale(m_hoverScale, m_hoverScale);
    }

    else
    {
        m_shape.setScale(1.f, 1.f);
        m_label.setScale(1.f, 1.f);
    }
}

void Button::UpdateFillColor(bool isMousePressed)
{
    if (m_enabled)
    {
        if (m_isHovered)
        {
            m_shape.setFillColor(isMousePressed ? m_activeColor : m_hoverColor);
        }

        else
        {
            m_shape.setFillColor(m_idleColor);
        }
    }

    else
    {
        if (m_isHovered)
        {
            m_shape.setFillColor(DEFAULT_DISABLED_HOVER_COLOR);
        }

        else
        {
            m_shape.setFillColor(DEFAULT_DISABLED_IDLE_COLOR);
        }
    }
}

void Button::UpdateTextColor()
{
    if (m_enabled)
    {
        m_label.setFillColor(m_textColor);
    }

    else
    {
        m_label.setFillColor(DEFAULT_DISABLED_TEXT_COLOR);
    }
}

void Button::HandleClickLogic(bool isMousePressed)
{
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
