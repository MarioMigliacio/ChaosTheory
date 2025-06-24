// ============================================================================
//  File        : UIIcon.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-19
//  Description : Represents a square clickable icon with a sprite
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIIcon.h"
#include "AssetManager.h"
#include "Macros.h"
#include "WindowManager.h"

UIIcon::UIIcon(const sf::Vector2f &size, const sf::Vector2f &position) : m_size(size)
{
    SetPosition(position);
    m_sprite.setColor(sf::Color::White);
}

void UIIcon::SetTextureSkin(const std::string &textureKey)
{
    m_textureKey = textureKey;

    auto tex = AssetManager::Instance().GetTexture(textureKey);

    if (tex)
    {
        m_sprite.setTexture(*tex, true);
        ApplySpriteTransform();
    }

    else
    {
        CT_LOG_WARN("UIIcon: No texture found for {}.", textureKey);
    }
}

void UIIcon::SetIconType(IconType type)
{
    m_iconType = type;
}

IconType UIIcon::GetIconType() const
{
    return m_iconType;
}

void UIIcon::Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt)
{
    if (m_expired)
    {
        return;
    }

    m_timeAlive += dt;

    if (m_timeAlive >= m_fallDelay && m_driftEnabled)
    {
        m_falling = true;
        ApplyAlphaPulse();

        auto pos = m_sprite.getPosition();
        pos.y += m_driftSpeed * dt;
        m_sprite.setPosition(pos);

        const auto winSize = WindowManager::Instance().GetWindow().getSize();

        if (pos.y > winSize.y + 32.f)
        {
            m_expired = true;
        }
    }
}

void UIIcon::StartFalling(float delaySeconds)
{
    m_fallDelay = delaySeconds;
    m_timeAlive = 0.f;
    m_falling = false;
    m_expired = false;
    m_driftEnabled = true;
    m_sprite.setColor(sf::Color::White);

    // Set fall speed based on type
    switch (m_iconType)
    {
        case IconType::AtomicIcon:
            m_driftSpeed = 60.f;
            break;
        case IconType::FireRateIcon:
            m_driftSpeed = 70.f;
            break;
        case IconType::GasIcon:
            m_driftSpeed = 50.f;
            break;
        case IconType::LifeIcon:
            m_driftSpeed = 100.f;
            break;
        case IconType::PowerIcon:
            m_driftSpeed = 80.f;
            break;
        case IconType::UpgradeIcon:
            m_driftSpeed = 90.f;
            break;
        case IconType::WarpIcon:
            m_driftSpeed = 0.f;
            m_driftEnabled = false;
            break;
        default:
            m_driftSpeed = 50.f;
            break;
    }
}

bool UIIcon::IsExpired() const
{
    return m_expired;
}

sf::FloatRect UIIcon::GetGlobalBounds() const
{
    return m_sprite.getGlobalBounds();
}

bool UIIcon::Contains(const sf::Vector2i &point) const
{
    return m_sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

void UIIcon::SetPosition(const sf::Vector2f &position)
{
    m_sprite.setPosition(position);
    ApplySpriteTransform();
}

sf::Vector2f UIIcon::GetPosition() const
{
    return m_sprite.getPosition();
}

void UIIcon::SetSize(const sf::Vector2f &size)
{
    m_size = size;
    ApplySpriteTransform();
}

sf::Vector2f UIIcon::GetSize() const
{
    return m_size;
}

void UIIcon::ApplySpriteTransform()
{
    if (!m_sprite.getTexture())
    {
        return;
    }

    auto bounds = m_sprite.getLocalBounds();

    float scale = std::min(m_size.x / bounds.width, m_size.y / bounds.height);
    m_sprite.setScale(scale, scale);

    sf::Vector2f offset((m_size.x - bounds.width * scale) / 2.f, (m_size.y - bounds.height * scale) / 2.f);
    m_sprite.setPosition(m_sprite.getPosition() + offset);
}

void UIIcon::ApplyAlphaPulse()
{
    // oscillate between visible and invisible with sin logic
    float alpha = 128.f + 127.f * std::sin(m_timeAlive * 6.f);
    sf::Color color = m_sprite.getColor();
    color.a = static_cast<sf::Uint8>(alpha);
    m_sprite.setColor(color);
}

void UIIcon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (!m_expired && m_sprite.getTexture())
    {
        target.draw(m_sprite, states);
    }
}
