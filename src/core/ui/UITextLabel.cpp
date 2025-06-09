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
#include "UIPresets.h"

/// @brief Constructor for the UITextLabel.
/// @param text String representation for this UITextLabel.
/// @param font Font to be used.
/// @param fontSize Font size for label.
/// @param position Position for label.
/// @param centerOrigin Whether or not to center the Text around the position or not.
UITextLabel::UITextLabel(const std::string &text, const sf::Font &font, unsigned int fontSize,
                         const sf::Vector2f &position, bool centerOrigin)
    : m_centerOrigin(centerOrigin)
{
    m_text.setFont(font);
    m_text.setString(text);
    m_text.setCharacterSize(fontSize);
    m_text.setFillColor(sf::Color::White);
    SetPosition(position); // ensures consistent logic with m_centerOrigin

    if (centerOrigin)
    {
        CenterOrigin();
    }
}

/// @brief Sets the text for this UITextLabel.
/// @param text new m_text.
void UITextLabel::SetText(const std::string &text)
{
    m_text.setString(text);
    CenterOrigin();
}

/// @brief Simple getter for this UITextLabels string content.
/// @return m_texts string representation.
const std::string UITextLabel::GetText() const
{
    return m_text.getString();
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

/// @brief Simple getter for this UITextLabels fill color.
/// @return m_texts fill color.
const sf::Color UITextLabel::GetFillColor() const
{
    return m_text.getFillColor();
}

/// @brief Sets the outline thickness for this UITextLabel.
/// @param thickness new outline thickness.
/// @param color new outline color.
void UITextLabel::SetOutline(float thickness, const sf::Color &color)
{
    m_text.setOutlineThickness(thickness);
    m_text.setOutlineColor(color);
}

/// @brief Simple getter for this UITextLabels outline color.
/// @return m_texts outline color.
const sf::Color UITextLabel::GetOutlineColor() const
{
    return m_text.getOutlineColor();
}

/// @brief Simple getter for this UITextLabels outline thickness.
/// @return m_texts outline thickness.
const float UITextLabel::GetOutlineThickness() const
{
    return m_text.getOutlineThickness();
}

/// @brief Useful helper for centering the UITextLabel on the localBounds.
void UITextLabel::CenterOrigin()
{
    if (m_centerOrigin)
    {
        auto bounds = m_text.getLocalBounds();
        m_text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    }
}

/// @brief Sets the position for this UITextLabel.
/// @param position new m_text.Position.
void UITextLabel::SetPosition(const sf::Vector2f &position)
{
    m_text.setPosition(position);

    if (m_centerOrigin)
    {
        sf::FloatRect bounds = m_text.getLocalBounds();
        m_text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }

    else
    {
        m_text.setOrigin(0.f, 0.f);
    }
}

/// @brief Returns the current position for this UITextLabel.
/// @return m_text.Position.
sf::Vector2f UITextLabel::GetPosition() const
{
    return m_text.getPosition();
}

/// @brief Helper method to utilize color themes for a TextLabel string combo.
/// @param scheme Enum field representing the type of shceme.
/// @param labelBorderSize Size to adjust text border witdth.
void UITextLabel::ApplyTextLabelStyle(UITextLabelScheme scheme, const float labelBorderSize)
{
    switch (scheme)
    {
        case UITextLabelScheme::DefaultScheme:
        default:
            // Default scheme is LimeGreen with Purple contrast border.
            m_text.setColor(TEXT_LABEL_COLOR_LIME_GREEN);
            m_text.setOutlineColor(TEXT_LABEL_COLOR_PURPLE_TINT);
            m_text.setOutlineThickness(labelBorderSize);
            break;

        case UITextLabelScheme::CougarScheme:
            // CougarScheme scheme is Crimson with Grey contrast border.
            m_text.setColor(TEXT_LABEL_COLOR_COUGAR_CRIMSON);
            m_text.setOutlineColor(TEXT_LABEL_COLOR_COUGAR_GREY);
            m_text.setOutlineThickness(labelBorderSize);
            break;

        case UITextLabelScheme::HuskyScheme:
            // HuskyScheme scheme is PurpleTint with MetallicGold contrast border.
            m_text.setColor(TEXT_LABEL_COLOR_PURPLE_TINT);
            m_text.setOutlineColor(TEXT_LABEL_COLOR_METALLIC_GOLD);
            m_text.setOutlineThickness(labelBorderSize);
            break;

        case UITextLabelScheme::BlueSteelScheme:
            // BlueSteelScheme scheme is BlueSteel with CoolGrey contrast border.
            m_text.setColor(TEXT_LABEL_COLOR_BLUE_STEEL);
            m_text.setOutlineColor(TEXT_LABEL_COLOR_COOL_GREY);
            m_text.setOutlineThickness(labelBorderSize);
            break;

        case UITextLabelScheme::MintyHerbScheme:
            // MintyHerbScheme scheme is TealMint with Dark Green contrast border.
            m_text.setColor(TEXT_LABEL_COLOR_TEAL_MINT);
            m_text.setOutlineColor(TEXT_LABEL_COLOR_MUTE_GREEN);
            m_text.setOutlineThickness(labelBorderSize);
            break;
    }
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

/// @brief Draw this UITextLabel to the Renderable Target.
/// @param target render target.
/// @param states optional sf::RenderStates.
void UITextLabel::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_text, states);
}
