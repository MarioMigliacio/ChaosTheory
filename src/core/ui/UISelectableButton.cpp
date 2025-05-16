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

/// @brief Constructs a UISelectableButton.
/// @param position sets internal position.
/// @param size sets internal size.
UISelectableButton::UISelectableButton(const sf::Vector2f &position, const sf::Vector2f &size)
{
    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(m_idleColor);
}

/// @brief Sets the internal state for the Is Selected logic for this SelectableButton.
/// @param selected new m_isSelected.
void UISelectableButton::SetSelected(bool selected)
{
    m_isSelected = selected;
}

/// @brief Returns the state of whether or not this SelectableButton is being selected.
/// @return m_isSelected.
bool UISelectableButton::IsSelected() const
{
    return m_isSelected;
}

/// @brief Sets the text fields for this SelectableButton.
/// @param text String representation for the SelectableButton.
/// @param font Font used.
/// @param fontSize Font size.
void UISelectableButton::SetText(const std::string &text, const sf::Font &font, unsigned int fontSize)
{
    m_label.setFont(font);
    m_label.setString(text);

    // Start with requested size, but shrink to fit if necessary
    m_fontSize = fontSize;
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

/// @brief Return a reference to this SelectableButton text label field.
/// @return m_label as string.
const std::string UISelectableButton::GetLabel() const
{
    return m_label.getString();
}

/// @brief Sets the text color for this SelectableButton.
/// @param color new m_textColor.
void UISelectableButton::SetTextColor(const sf::Color &color)
{
    m_textColor = color;
    m_label.setFillColor(color);
}

/// @brief When selected, update the color for this SelectableButton.
/// @param fillColor new Fill color.
/// @param textColor new Text color.
void UISelectableButton::SetSelectedColor(const sf::Color &fillColor, const sf::Color &textColor)
{
    m_selectedFillColor = fillColor;
    m_selectedTextColor = textColor;
}

/// @brief When hovered, update the color for this SelectableButton.
/// @param hoverColor new m_hoverColor.
void UISelectableButton::SetHoverColor(const sf::Color &hoverColor)
{
    m_hoverColor = hoverColor;
}

/// @brief Sets the font size for this SelectableButton.
/// @param size new fontSize.
void UISelectableButton::SetFontSize(unsigned int size)
{
    m_fontSize = size;
    m_label.setCharacterSize(size);
    CenterLabel();
}

/// @brief Update the callback function set for this SelectableButton when selected.
/// @param onSelect Function pointer to run typically when clicked.
void UISelectableButton::SetCallback(std::function<void()> onSelect)
{
    m_onSelect = std::move(onSelect);
}

/// @brief Performs internal state management during a single frame.
/// @param mousePosition current MousePosition.
/// @param isMousePressed IsMousePressed?
/// @param isMouseJustPressed IsMouseJustPressed?
/// @param dt delta time since last update.
void UISelectableButton::Update(const sf::Vector2i &mousePosition, bool isMousePressed, bool isMouseJustPressed,
                                float dt)
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

    HandleClickLogic(isMouseJustPressed);
    UpdateVisualState();
}

/// @brief Returns whether or not the point is within the bounds of this SelectableButton.
/// @param point Compare against us.
/// @return true / false
bool UISelectableButton::Contains(const sf::Vector2i &point) const
{
    return m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

/// @brief Sets the current position for this SelectableButton.
/// @param position new m_position.
void UISelectableButton::SetPosition(const sf::Vector2f &position)
{
    m_shape.setPosition(position);
    CenterLabel();
}

/// @brief Returns the position for this SelectableButton.
/// @return m_position.
sf::Vector2f UISelectableButton::GetPosition() const
{
    return m_shape.getPosition();
}

/// @brief Sets the size for this SelectableButton.
/// @param size new m_size.
void UISelectableButton::SetSize(const sf::Vector2f &size)
{
    m_shape.setSize(size);
    CenterLabel();
}

/// @brief Returns the size for this SelectableButton.
/// @return m_size.
sf::Vector2f UISelectableButton::GetSize() const
{
    return m_shape.getSize();
}

/// @brief Adjusts the text label for this SelectableButton.
void UISelectableButton::CenterLabel()
{
    sf::FloatRect bounds = m_label.getLocalBounds();

    m_label.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    m_label.setPosition(m_shape.getPosition().x + m_shape.getSize().x / 2.f,
                        m_shape.getPosition().y + m_shape.getSize().y / 2.f);
}

/// @brief Draw this SelectableButton to the Render target.
/// @param target render target.
/// @param states optional sf::RenderStates.
void UISelectableButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
    target.draw(m_label, states);
}

/// @brief Updates the on click status and handling for this Selectable Button.
/// @param isMousePressed Whether or not the Mouse was registered as Pressed.
void UISelectableButton::HandleClickLogic(bool isMousePressed)
{
    if (m_isHovered && isMousePressed)
    {
        CT_LOG_INFO("Selectable Button clicked.");

        if (m_onSelect)
        {
            m_onSelect();
        }
    }
}

/// @brief Updates button color and appearance logic.
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