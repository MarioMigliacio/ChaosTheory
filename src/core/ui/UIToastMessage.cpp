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

UIToastMessage::UIToastMessage(const std::string &text, const sf::Vector2f &position, float durationSeconds,
                               const sf::Font &font, unsigned int fontSize, const sf::Color &textColor,
                               bool centerOrigin)
    : m_duration(durationSeconds), m_centerOrigin(centerOrigin)
{
    m_text.setFont(font);
    m_text.setString(text);
    m_text.setCharacterSize(fontSize);
    m_text.setFillColor(textColor);

    SetPosition(position); // will auto-center if needed
}

void UIToastMessage::Update(const sf::Vector2i &, bool, float dt)
{
    if (!IsEnabled())
        return;

    m_elapsed += 1.0f / 60.0f; // Assume ~60 FPS; for true dt tracking, pass dt in your scene

    // Could add fade-out or slide animation here in future
}

bool UIToastMessage::Contains(const sf::Vector2i &point) const
{
    return m_text.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

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

    m_text.setPosition(position);
}

sf::Vector2f UIToastMessage::GetPosition() const
{
    return m_text.getPosition();
}

void UIToastMessage::SetSize(const sf::Vector2f &)
{
    // Not applicable for text-only toast, but required by interface.
}

sf::Vector2f UIToastMessage::GetSize() const
{
    const auto bounds = m_text.getGlobalBounds();
    return {bounds.width, bounds.height};
}

bool UIToastMessage::IsExpired() const
{
    return m_elapsed >= m_duration;
}

void UIToastMessage::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (IsEnabled() && !IsExpired())
    {
        target.draw(m_text, states);
    }
}
