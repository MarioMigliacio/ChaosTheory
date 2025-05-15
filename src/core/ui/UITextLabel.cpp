// ============================================================================
//  File        : UITextLabel.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-23
//  Description : Represents a UI string element.
//                Can be used in menus and interactive scenes.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UITextLabel.h"

/// @brief Constructor for the UITextLabel.
/// @param text String representation for this UITextLabel.
/// @param font Font to be used.
/// @param fontSize Font size for label.
/// @param position Position for label.
UITextLabel::UITextLabel(const std::string &text, const sf::Font &font, unsigned int fontSize,
                         const sf::Vector2f &position)
{
    m_text.setFont(font);
    m_text.setString(text);
    m_text.setCharacterSize(fontSize);
    m_text.setFillColor(sf::Color::White);
    m_text.setPosition(position);
    CenterOrigin();
}

/// @brief Sets the text for this UITextLabel.
/// @param text new m_text.
void UITextLabel::SetText(const std::string &text)
{
    m_text.setString(text);
    CenterOrigin();
}

/// @brief Sets the font for this UITextLabel.
/// @param font new m_text.font.
void UITextLabel::SetFont(const sf::Font &font)
{
    m_text.setFont(font);
    CenterOrigin();
}

/// @brief Sets the font size for this UITextLabel
/// @param size new m_text.CharacterSzie.
void UITextLabel::SetFontSize(unsigned int size)
{
    m_text.setCharacterSize(size);
    CenterOrigin();
}

/// @brief Sets the text fill color for this UITextLabel.
/// @param color new m_text.Color.
void UITextLabel::SetColor(const sf::Color &color)
{
    m_text.setFillColor(color);
}

/// @brief Sets the outline thickness for this UITextLabel.
/// @param thickness new outline thickness.
/// @param color new outline color.
void UITextLabel::SetOutline(float thickness, const sf::Color &color)
{
    m_text.setOutlineThickness(thickness);
    m_text.setOutlineColor(color);
}

/// @brief Sets the position for this UITextLabel.
/// @param position new m_text.Position.
void UITextLabel::SetPosition(const sf::Vector2f &position)
{
    m_text.setPosition(position);
}

/// @brief Returns the current position for this UITextLabel.
/// @return m_text.Position.
sf::Vector2f UITextLabel::GetPosition() const
{
    return m_text.getPosition();
}

/// @brief Unused for this UITextLabel currently, just for interface consistency.
/// @param None
void UITextLabel::SetSize(const sf::Vector2f &)
{
}

/// @brief Gets the size in localBounds for this UITextLabel.
/// @return Vector2f of size.
sf::Vector2f UITextLabel::GetSize() const
{
    auto bounds = m_text.getLocalBounds();
    return {bounds.width, bounds.height};
}

/// @brief Useful helper for centering the UITextLabel on the localBounds.
void UITextLabel::CenterOrigin()
{
    auto bounds = m_text.getLocalBounds();
    m_text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

/// @brief Draw this UITextLabel to the Renderable Target.
/// @param target render target.
/// @param states optional sf::RenderStates.
void UITextLabel::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_text, states);
}
