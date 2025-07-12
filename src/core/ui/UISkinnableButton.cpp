// ============================================================================
//  File        : UISkinnableButton.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-18
//  Description : Represents a clickable UI button with hover and press states.
//                Can be used in menus and interactive scenes. This button
//                supports texture skins, for more customizable appearances.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UISkinnableButton.h"
#include "AssetManager.h"
#include "Macros.h"

/// @brief Constructs a UISkinnableButton.
/// @param position Sets internal position.
/// @param size Sets internal size.
UISkinnableButton::UISkinnableButton(const sf::Vector2f &position, const sf::Vector2f &size)
    : m_size(size), m_position(position)
{
}

/// @brief Sets the internal idle and hover textures for this UISkinnableButton.
/// @param idle new m_textureIdle.
/// @param hover new m_textureHover.
void UISkinnableButton::SetTextureSkins(const std::string &idle, const std::string &hover)
{
    m_textureIdle = idle;
    m_textureHover = hover;

    auto tex = AssetManager::Instance().GetTexture(idle);

    if (tex)
    {
        m_sprite.setTexture(*tex, true);
        ApplySpriteTransform();
    }
}

/// @brief Update the callback function set for this UISkinnableButton when selected.
/// @param callback Function pointer to run typically when clicked.
void UISkinnableButton::SetCallback(std::function<void()> callback)
{
    m_onClick = std::move(callback);
}

/// @brief Sets the internal hoverscale for this UISkinnableButton.
/// @param scale new m_hoverScale.
void UISkinnableButton::SetHoverScale(float scale)
{
    m_hoverScale = scale;
}

/// @brief Sets the internal text related members for this UISkinnableButton.
/// @param text string contents of the text for UISkinnableButton.
/// @param font font to be used for UISkinnableButton.
/// @param size size of the text for the UISkinnableButton.
void UISkinnableButton::SetText(const std::string &text, const sf::Font &font, unsigned int size)
{
    m_label.setFont(font);
    m_label.setString(text);
    m_fontSize = size;
    m_label.setCharacterSize(m_fontSize);
    m_label.setFillColor(m_textColor);

    // Auto-fit text if too wide, nice to have!
    const float maxWidth = m_size.x * 0.9f; // Leave a little margin
    sf::FloatRect textRect = m_label.getLocalBounds();

    while (textRect.width > maxWidth && m_fontSize > 8) // Don't go below readable size
    {
        m_fontSize -= 1;
        m_label.setCharacterSize(m_fontSize);
        textRect = m_label.getLocalBounds();
    }

    CenterLabel();
}

/// @brief Return a reference to this UISkinnableButton text label field.
/// @return m_label as string.
const std::string UISkinnableButton::GetLabel() const
{
    return m_label.getString();
}

/// @brief Sets the text color for this UISkinnableButton.
/// @param color new m_textColor.
void UISkinnableButton::SetTextColor(const sf::Color &color)
{
    m_textColor = color;
    m_label.setFillColor(color);
}

/// @brief Sets the font size for this UISkinnableButton.
/// @param size new fontSize.
void UISkinnableButton::SetFontSize(unsigned int size)
{
    m_fontSize = size;
    m_label.setCharacterSize(size);
    CenterLabel();
}

/// @brief Allows the button to adjust its label text fill color, outline, and outline thickness.
/// @param textColor  new color for label text.
/// @param outlineColor new border color for label text.
/// @param outlineThickness new thickness in pixels for label text border.
void UISkinnableButton::SetTextStyle(const sf::Color &textColor, const sf::Color &outlineColor, float outlineThickness)
{
    m_textColor = textColor;
    m_label.setFillColor(textColor);
    m_label.setOutlineColor(outlineColor);
    m_label.setOutlineThickness(outlineThickness);
}

/// @brief Sets the internal position for this UISkinnableButton.
/// @param position new m_position.
void UISkinnableButton::SetPosition(const sf::Vector2f &position)
{
    m_position = position;

    if (m_sprite.getTexture())
    {
        ApplySpriteTransform();
    }
}

/// @brief Returns the internal position for this UISkinnableButton.
/// @return m_position.
sf::Vector2f UISkinnableButton::GetPosition() const
{
    return m_position;
}

/// @brief Sets the internal size for this UISkinnableButton.
/// @param size new m_size.
void UISkinnableButton::SetSize(const sf::Vector2f &size)
{
    m_size = size;

    if (m_sprite.getTexture())
    {
        ApplySpriteTransform();
    }
}

/// @brief Returns the internal size for this UISkinnableButton.
/// @return m_size.
sf::Vector2f UISkinnableButton::GetSize() const
{
    return m_size;
}

/// @brief Helper method to utilize color themes for a SkinnableButton combo.
/// @param scheme Enum field representing  the type of scheme.
void UISkinnableButton::ApplySkinnableButtonTextStyle(UISkinnableButtonColorScheme scheme)
{
    switch (scheme)
    {
        case UISkinnableButtonColorScheme::Blue:
            SetTextStyle(TEX_BTN_BLUE_LABEL_TEXT_COLOR, TEX_BTN_BLUE_TEXT_OUTLINE_COLOR, 2.0f);
            break;

        case UISkinnableButtonColorScheme::Green:
            SetTextStyle(TEX_BTN_GREEN_LABEL_TEXT_COLOR, TEX_BTN_GREEN_TEXT_OUTLINE_COLOR, 2.0f);
            break;

        case UISkinnableButtonColorScheme::Red:
            SetTextStyle(TEX_BTN_RED_LABEL_TEXT_COLOR, TEX_BTN_RED_TEXT_OUTLINE_COLOR, 2.0f);
            break;
    }
}

/// @brief Returns whether or not the point is within the bounds of this UISkinnableButton.
/// @param point Compare against us.
/// @return true / false
bool UISkinnableButton::Contains(const sf::Vector2i &point) const
{
    return m_sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

/// @brief Performs internal state management during a single frame.
/// @param mousePos current MousePosition.
/// @param isMousePressed IsMousePressed?
/// @param isMouseJustPressed IsMouseJustPressed?
/// @param dt delta time since last update.
void UISkinnableButton::Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt)
{
    if (!m_enabled)
    {
        m_sprite.setColor(m_disabledColorMask);
    }

    else
    {
        m_sprite.setColor(m_enabledColorMask);
    }

    const bool wasHovered = m_isHovered;
    m_isHovered = Contains(mousePos);

    if (m_isHovered && m_enabled)
    {
        m_sprite.setScale(m_hoverScale, m_hoverScale);

        auto tex = AssetManager::Instance().GetTexture(m_textureHover);

        if (tex)
        {
            m_sprite.setTexture(*tex, true);
            ApplySpriteTransform();
        }

        if (isMouseJustPressed && !m_isPressed)
        {
            m_isPressed = true;

            if (m_onClick)
            {
                CT_LOG_INFO("UISkinnableButton clicked.");

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
        m_sprite.setScale(1.0f, 1.0f);
        auto tex = AssetManager::Instance().GetTexture(m_textureIdle);

        if (tex)
        {
            m_sprite.setTexture(*tex, true);
            ApplySpriteTransform();
        }

        m_isPressed = false;
    }
}

/// @brief Scales the sprite for this UISkinnableButton to be be appropriate with the size currently set.
void UISkinnableButton::ApplySpriteTransform()
{
    if (!m_sprite.getTexture())
    {
        CT_LOG_WARN("UISkinnableButton: No texture bound to sprite.");

        return;
    }

    const sf::Vector2u texSize = m_sprite.getTexture()->getSize();

    if (texSize.x == 0 || texSize.y == 0)
    {
        return;
    }

    const float scaleX = m_size.x / static_cast<float>(texSize.x);
    const float scaleY = m_size.y / static_cast<float>(texSize.y);

    m_sprite.setScale(scaleX, scaleY);
    m_sprite.setPosition(m_position);
    CenterLabel();
}

/// @brief Adjusts the text label for this UISkinnableButton.
void UISkinnableButton::CenterLabel()
{
    sf::FloatRect bounds = m_label.getLocalBounds();

    m_label.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    m_label.setPosition(m_position.x + m_size.x / 2.f, m_position.y + m_size.y / 2.f);
}

/// @brief Adjust the color for this UISkinnableButton text, based on enabled states.
void UISkinnableButton::UpdateTextColor()
{
    if (m_enabled)
    {
        m_label.setFillColor(m_textColor);
    }

    else
    {
        m_label.setFillColor(m_disabledColorMask);
    }
}

/// @brief Draw this UISkinnableButton to the Render target.
/// @param target render target.
/// @param states optional sf::RenderState
void UISkinnableButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
    target.draw(m_label, states);
}
