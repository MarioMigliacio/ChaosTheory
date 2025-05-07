// ============================================================================
//  File        : UISelectableButton.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-27
//  Description : Represents a clickable UI button where only one option can
//                be selected in a group option setting.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UISelectableButton.h"
#include "AssetManager.h"
#include "Macros.h"

UISelectableButton::UISelectableButton(const sf::Vector2f &position, const sf::Vector2f &size)
{
    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(m_idleColor);
}

// Sets the internal state for the Is Selected logic for this Radio Button.
void UISelectableButton::SetSelected(bool selected)
{
    m_isSelected = selected;
}

// Returns the state of whether or not this Radio Button is being selected.
bool UISelectableButton::IsSelected() const
{
    return m_isSelected;
}

// Sets the text fields for this Radio Button.
void UISelectableButton::SetText(const std::string &text, const sf::Font &font, unsigned int size)
{
    m_label.setFont(font);
    m_label.setString(text);

    // Start with requested size, but shrink to fit if necessary
    m_fontSize = size;
    m_label.setCharacterSize(m_fontSize);

    float maxWidth = m_shape.getSize().x - 32.f; // padding from circle and edge
    sf::FloatRect bounds = m_label.getLocalBounds();

    while (bounds.width > maxWidth && m_fontSize > 10)
    {
        --m_fontSize;
        m_label.setCharacterSize(m_fontSize);
        bounds = m_label.getLocalBounds();
    }

    m_label.setFillColor(m_textColor);

    CenterLabel();
}

// Return a reference to this Radio Button text label field.
const std::string UISelectableButton::GetLabel() const
{
    return m_label.getString();
}

// Sets the text color for this Radio Button.
void UISelectableButton::SetTextColor(const sf::Color &color)
{
    m_textColor = color;
    m_label.setFillColor(color);
}

// When selected, update the color for this Radio Button.
void UISelectableButton::SetSelectedColor(const sf::Color &fillColor, const sf::Color &textColor)
{
    m_selectedFillColor = fillColor;
    m_selectedTextColor = textColor;
}

// When hovered, update the color for this Radio Button.
void UISelectableButton::SetHoverColor(const sf::Color &hoverColor)
{
    m_hoverColor = hoverColor;
}

// Sets the font size for this Radio Button.
void UISelectableButton::SetFontSize(unsigned int size)
{
    m_fontSize = size;
    m_label.setCharacterSize(size);
    CenterLabel();
}

// Update the callback function set for this Radio Button when selected.
void UISelectableButton::SetCallback(std::function<void()> onSelect)
{
    m_onSelect = std::move(onSelect);
}

// Update logic for this Radio Button includes, isHovered, isPressed, scale, color and text.
void UISelectableButton::Update(const sf::Vector2i &mousePosition, bool isMousePressed)
{
    sf::Vector2f mouse(mousePosition);
    bool wasHovered = m_isHovered;

    m_isHovered = m_shape.getGlobalBounds().contains(mouse);

    if (m_isHovered && !wasHovered)
    {
        CT_LOG_DEBUG("UISelectableButton hovered.");
    }

    else if (!m_isHovered && wasHovered)
    {
        CT_LOG_DEBUG("UISelectableButton unhovered.");
    }

    HandleClickLogic(isMousePressed);
    UpdateVisualState();
}

// Returns whether or not the point is within the bounds of this Radio Button.
bool UISelectableButton::Contains(const sf::Vector2i &point) const
{
    return m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

// Sets the current position for this Radio Button.
void UISelectableButton::SetPosition(const sf::Vector2f &position)
{
    m_shape.setPosition(position);
    CenterLabel();
}

// Returns the position for this Button.
sf::Vector2f UISelectableButton::GetPosition() const
{
    return m_shape.getPosition();
}

// Sets the size for this Radio Button.
void UISelectableButton::SetSize(const sf::Vector2f &size)
{
    m_shape.setSize(size);
    CenterLabel();
}

// Returns the size for this Radio Button.
sf::Vector2f UISelectableButton::GetSize() const
{
    return m_shape.getSize();
}

// Adjusts the text label for this Radio Button.
void UISelectableButton::CenterLabel()
{
    sf::FloatRect bounds = m_label.getLocalBounds();

    m_label.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    m_label.setPosition(m_shape.getPosition().x + m_shape.getSize().x / 2.f,
                        m_shape.getPosition().y + m_shape.getSize().y / 2.f);
}

// Draw this Radio Button to the Render target.
void UISelectableButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Draw background rectangle
    target.draw(m_shape, states);

    // Draw label text
    target.draw(m_label, states);
}

// Updates the on click status and handling for this Radio Button.
void UISelectableButton::HandleClickLogic(bool isMousePressed)
{
    if (m_isHovered && isMousePressed)
    {
        CT_LOG_INFO("Radio Button clicked.");

        if (m_onSelect)
        {
            m_onSelect();
        }
    }
}

// Updates button color and appearance logic.
void UISelectableButton::UpdateVisualState()
{
    if (m_isSelected)
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