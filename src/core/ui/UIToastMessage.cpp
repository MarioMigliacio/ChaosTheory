// ============================================================================
//  File        : UIToastMessage.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-08
//  Description : Implementation of UIToastMessage - brief on-screen messages
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIToastMessage.h"
#include "ResolutionScaleManager.h"
#include "WindowManager.h"

/// @brief Constructor for the UIToastMessage.
/// @param text String representation for this UIToastMessage.
/// @param position Position for this UIToastMessage.
/// @param durationSeconds Lifespan in delta time for this UIToastMessage.
/// @param font Font to be used.
/// @param fontSize Font size.
/// @param textColor Color for the text fill color.
/// @param centerOrigin Positional origin for the UIToastMessage.
UIToastMessage::UIToastMessage(const std::string &text, const sf::Vector2f &position, float durationSeconds,
                               const sf::Font &font, unsigned int fontSize, const sf::Color &textColor,
                               bool centerOrigin)
    : m_duration(durationSeconds), m_centerOrigin(centerOrigin)
{
    m_text.setFont(font);
    m_text.setString(text);
    m_text.setCharacterSize(fontSize);
    m_text.setFillColor(textColor);

    SetPosition(position);
}

/// @brief Performs internal state management during a single frame. Note the default update signature is in line with
/// interface, but only delta time is relevant for this UIToastMessage.
/// @param none
/// @param none
/// @param none
/// @param dt delta time since last update.
void UIToastMessage::Update(const sf::Vector2i &, bool, bool, float dt)
{
    if (!IsEnabled())
    {
        return;
    }

    m_elapsed += dt;

    // Slide animation: interpolate Y position upward (toast rise)
    float t = std::min(1.f, m_elapsed / m_duration);
    float newY = m_startY + (m_targetY - m_startY) * t;
    auto pos = m_text.getPosition();
    m_text.setPosition(pos.x, newY);

    // Fade out near end
    if (m_elapsed >= m_duration - m_fadeOutDuration)
    {
        float fadeT = (m_duration - m_elapsed) / m_fadeOutDuration;
        m_alpha = 255.f * std::clamp(fadeT, 0.f, 1.f);
        auto color = m_text.getFillColor();
        color.a = static_cast<sf::Uint8>(m_alpha);
        m_text.setFillColor(color);
    }
}

/// @brief Returns whether or not the point is within the bounds of this UIToastMessage.
/// @param point Point to compair against us.
/// @return true / false
bool UIToastMessage::Contains(const sf::Vector2i &point) const
{
    return m_text.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

/// @brief Sets the position for this UIToastMessage.
/// @param position new text.position.
void UIToastMessage::SetPosition(const sf::Vector2f &position)
{
    if (m_centerOrigin)
    {
        const auto bounds = m_text.getLocalBounds();
        m_text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    }

    else
    {
        m_text.setOrigin(0.f, 0.f);
    }

    // Calculate Y drift
    float drift = ResolutionScaleManager::Instance().ScaledReferenceY(TOAST_DEFAULT_DRIFT_PERCENTAGE);
    const auto winSize = WindowManager::Instance().GetWindow().getSize();

    // Clamp start and target Y within screen bounds
    m_targetY = std::min(position.y, winSize.y - drift);        // target must remain on-screen
    m_startY = std::min(position.y + drift, winSize.y - drift); // drift downward but within screen

    m_text.setPosition(position.x, m_startY);
}

/// @brief Returns the current position for this UIToastMessage.
/// @return m_text.position.
sf::Vector2f UIToastMessage::GetPosition() const
{
    return m_text.getPosition();
}

/// @brief Not relevant for this UIToastMessage, but consistent with interface.
/// @param none
void UIToastMessage::SetSize(const sf::Vector2f &)
{
    // Not applicable for text-only toast, but required by interface.
}

/// @brief Returns the size bounds for this UIToastMessage.
/// @return size.
sf::Vector2f UIToastMessage::GetSize() const
{
    const auto bounds = m_text.getGlobalBounds();
    return {bounds.width, bounds.height};
}

/// @brief Sets the font for this UIToastMessage.
/// @param font new m_text.font.
void UIToastMessage::SetFont(const sf::Font &font)
{
    m_text.setFont(font);
}

/// @brief Sets the font for this UIToastMessage.
/// @param size new m_text.size.
void UIToastMessage::SetFontSize(unsigned int size)
{
    m_text.setCharacterSize(size);
    // Optional: Re-center origin if needed
    sf::FloatRect bounds = m_text.getLocalBounds();
    m_text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

/// @brief Sets the text fill color for this UIToastMessage.
/// @param color new m_text.color.
void UIToastMessage::SetColor(const sf::Color &color)
{
    m_text.setFillColor(color);
}

/// @brief Returns whether or not this UIToastMessage lifespan is expired.
/// @return true / false.
bool UIToastMessage::IsExpired() const
{
    return m_elapsed >= m_duration;
}

/// @brief Draw this UIToastMessage to the Renderable Target.
/// @param target render target.
/// @param states optional sf::RenderStates.
void UIToastMessage::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (IsEnabled() && !IsExpired())
    {
        target.draw(m_text, states);
    }
}
