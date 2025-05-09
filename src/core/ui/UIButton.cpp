// ============================================================================
//  File        : UIButton.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-18
//  Description : Represents a clickable UI button with hover and press states.
//                Can be used in menus and interactive scenes.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIButton.h"
#include "Macros.h"

UIButton::UIButton(const sf::Vector2f &position, const sf::Vector2f &size)
{
    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(m_idleColor);
}

// Sets the internal text related members for this UIButton.
void UIButton::SetText(const std::string &text, const sf::Font &font, unsigned int size)
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

// Sets the internal callback function for this UIButton, which responds to onClick.
void UIButton::SetCallback(std::function<void()> callback)
{
    m_onClick = std::move(callback);
}

// Sets the color for this button during non-hover time.
void UIButton::SetIdleColor(const sf::Color &color)
{
    m_idleColor = color;
    m_shape.setFillColor(m_idleColor);
}

// Sets the color for this button during hover event.
void UIButton::SetHoverColor(const sf::Color &color)
{
    m_hoverColor = color;
}

// While button is in focus and active, set the color.
void UIButton::SetActiveColor(const sf::Color &color)
{
    m_activeColor = color;
}

// Sets the color for this buttons text.
void UIButton::SetTextColor(const sf::Color &color)
{
    m_textColor = color;
    m_label.setFillColor(m_textColor);
}

// Sets the fontsize for this button.
void UIButton::SetFontSize(unsigned int size)
{
    m_fontSize = size;
    m_label.setCharacterSize(m_fontSize);
    CenterLabel();
}

// Updates the scale size for this button, during hover and active focus.
void UIButton::SetHoverScale(float scale)
{
    m_hoverScale = scale;
}

// Update logic for this UIButton includes, isHovered, isPressed, scale, color and text.
void UIButton::Update(const sf::Vector2i &mousePosition, bool isMousePressed, float dt)
{
    sf::Vector2f mouse(mousePosition);
    bool wasHovered = m_isHovered;

    m_isHovered = m_shape.getGlobalBounds().contains(mouse);

    if (m_isHovered && !wasHovered)
    {
        CT_LOG_DEBUG("UIButton hovered.");
    }

    else if (!m_isHovered && wasHovered)
    {
        CT_LOG_DEBUG("UIButton unhovered.");
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

// Returns whether or not the point is within the bounds of this UIButton.
bool UIButton::Contains(const sf::Vector2i &point) const
{
    return m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

// Sets the position for this UIButton.
void UIButton::SetPosition(const sf::Vector2f &position)
{
    m_shape.setPosition(position);
    CenterLabel();
}

// Returns the position for this UIButton.
sf::Vector2f UIButton::GetPosition() const
{
    return m_shape.getPosition();
}

// Sets the size for this button and adjusts the label.
void UIButton::SetSize(const sf::Vector2f &size)
{
    m_shape.setSize(size);
    CenterLabel();
}

// Returns the size for this UIButton.
sf::Vector2f UIButton::GetSize() const
{
    return m_shape.getSize();
}

// Fix the label to be centered in this UIButton.
void UIButton::CenterLabel()
{
    sf::FloatRect textRect = m_label.getLocalBounds();
    m_label.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

    m_label.setPosition(m_shape.getPosition().x + m_shape.getSize().x / 2.f,
                        m_shape.getPosition().y + m_shape.getSize().y / 2.f);
}

// Draw this UIButton to the Renderable Target.
void UIButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
    target.draw(m_label, states);
}

// While hover and enable events, adjust the focus size of this button.
void UIButton::UpdateScale()
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

// Adjust the color of this button during enabled and hover events.
void UIButton::UpdateFillColor(bool isMousePressed)
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
            m_shape.setFillColor(BUTTON_DEFAULT_DISABLED_HOVER_COLOR);
        }

        else
        {
            m_shape.setFillColor(BUTTON_DEFAULT_DISABLED_IDLE_COLOR);
        }
    }
}

// Adjust the color for this UIButton based on enabled states.
void UIButton::UpdateTextColor()
{
    if (m_enabled)
    {
        m_label.setFillColor(m_textColor);
    }

    else
    {
        m_label.setFillColor(BUTTON_DEFAULT_DISABLED_TEXT_COLOR);
    }
}

// Adjust the click states for this UIButton.
void UIButton::HandleClickLogic(bool isMousePressed)
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
