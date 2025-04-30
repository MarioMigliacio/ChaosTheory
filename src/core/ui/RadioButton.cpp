// ============================================================================
//  File        : RadioButton.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-27
//  Description : Represents a clickable UI button where only one option can
//                be selected in a group option setting.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "RadioButton.h"
#include "AssetManager.h"
#include "Macros.h"

RadioButton::RadioButton(const sf::Vector2f &position, const sf::Vector2f &size, const std::string &label,
                         std::function<void()> onSelect)
    : m_onSelect(std::move(onSelect))
{
    m_shape.setPosition(position);
    m_shape.setSize(size);

    m_label.setFont(AssetManager::Instance().GetFont("Default.ttf"));
    m_label.setString(label);
    m_label.setCharacterSize(24);
    m_label.setFillColor(DEFAULT_TEXT_COLOR);

    m_selectedFillColor = DEFAULT_IDLE_COLOR;
    m_textColor = DEFAULT_TEXT_COLOR;

    CenterLabel();
}

// Sets the internal state for the Is Selected logic for this Radio Button.
void RadioButton::SetSelected(bool selected)
{
    m_isSelected = selected;
}

// Returns the state of whether or not this Radio Button is being selected.
bool RadioButton::IsSelected() const
{
    return m_isSelected;
}

// Sets the text fields for this Radio Button.
void RadioButton::SetText(const std::string &text, const sf::Font &font, unsigned int size)
{
    m_fontSize = size;
    m_label.setFont(font);
    m_label.setString(text);
    m_label.setCharacterSize(size);
    m_label.setFillColor(m_textColor);
    CenterLabel();
}

// Sets the text color for this Radio Button.
void RadioButton::SetTextColor(const sf::Color &color)
{
    m_textColor = color;
    m_label.setFillColor(color);
}

// When selected, update the color for this Radio Button.
void RadioButton::SetSelectedColor(const sf::Color &fillColor, const sf::Color &textColor)
{
    m_selectedFillColor = fillColor;
    m_selectedTextColor = textColor;
}

// When hovered, update the color for this Radio Button.
void RadioButton::SetHoverColor(const sf::Color &hoverColor)
{
    m_hoverColor = hoverColor;
}

// Sets the font size for this Radio Button.
void RadioButton::SetFontSize(unsigned int size)
{
    m_fontSize = size;
    m_label.setCharacterSize(size);
    CenterLabel();
}

// Update the callback function set for this Radio Button when selected.
void RadioButton::SetCallback(std::function<void()> onSelect)
{
    m_onSelect = std::move(onSelect);
}

// Update logic for this Radio Button includes, isHovered, isPressed, scale, color and text.
void RadioButton::Update(const sf::Vector2i &mousePosition, bool isMousePressed)
{
    sf::Vector2f mouse(mousePosition);
    bool wasHovered = m_isHovered;

    m_isHovered = m_shape.getGlobalBounds().contains(mouse);

    if (m_isHovered && !wasHovered)
    {
        CT_LOG_DEBUG("RadioButton hovered.");
    }

    else if (!m_isHovered && wasHovered)
    {
        CT_LOG_DEBUG("RadioButton unhovered.");
    }

    HandleClickLogic(isMousePressed);
    UpdateVisualState();
}

// Returns whether or not the point is within the bounds of this Radio Button.
bool RadioButton::Contains(const sf::Vector2i &point) const
{
    return m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

// Sets the current position for this Radio Button.
void RadioButton::SetPosition(const sf::Vector2f &position)
{
    m_shape.setPosition(position);
    CenterLabel();
}

// Sets the size for this Radio Button.
void RadioButton::SetSize(const sf::Vector2f &size)
{
    m_shape.setSize(size);
    CenterLabel();
}

// Adjusts the text label for this Radio Button.
void RadioButton::CenterLabel()
{
    sf::FloatRect textRect = m_label.getLocalBounds();
    m_label.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

    m_label.setPosition(m_shape.getPosition().x + m_shape.getSize().x / 2.f,
                        m_shape.getPosition().y + m_shape.getSize().y / 2.f);
}

// Draw this Radio Button to the Render target.
void RadioButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape drawableShape = m_shape;
    drawableShape.setFillColor(m_selectedFillColor);

    sf::Text drawableText = m_label;
    drawableText.setFillColor(m_textColor);

    target.draw(drawableShape, states);
    target.draw(drawableText, states);
}

// Updates the on click status and handling for this Radio Button.
void RadioButton::HandleClickLogic(bool isMousePressed)
{
    if (m_isHovered && isMousePressed)
    {
        CT_LOG_INFO("Button clicked.");

        if (m_onSelect)
        {
            m_onSelect();
        }
    }
}

// Updates button color and appearance logic.
void RadioButton::UpdateVisualState()
{
    if (!m_enabled)
    {
        m_shape.setFillColor(DEFAULT_DISABLED_IDLE_COLOR);
        m_label.setFillColor(DEFAULT_DISABLED_TEXT_COLOR);
    }

    else if (m_isSelected)
    {
        m_shape.setFillColor(m_selectedFillColor);
        m_label.setFillColor(m_selectedTextColor);
    }

    else if (m_isHovered)
    {
        m_shape.setFillColor(m_hoverColor);
        m_label.setFillColor(m_textColor);
    }

    else
    {
        m_shape.setFillColor(m_idleColor);
        m_label.setFillColor(m_textColor);
    }
}