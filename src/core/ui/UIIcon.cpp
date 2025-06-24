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

UIIcon::UIIcon(const sf::Vector2f &size, const sf::Vector2f &position) : m_size(size)
{
    SetPosition(position);
    m_sprite.setColor(m_normalTint);
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

void UIIcon::SetOnClick(const std::function<void()> &callback)
{
    m_onClick = callback;
}

void UIIcon::SetHoverTint(const sf::Color &color)
{
    m_hoverTint = color;
}

void UIIcon::SetDisabledTint(const sf::Color &color)
{
    m_disabledTint = color;
}

void UIIcon::SetNormalTint(const sf::Color &color)
{
    m_normalTint = color;

    if (m_enabled)
    {
        m_sprite.setColor(m_normalTint);
    }
}

void UIIcon::Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt)
{
    if (!m_enabled)
    {
        m_sprite.setColor(m_disabledTint);
        return;
    }

    m_isHovered = Contains(mousePos);
    m_sprite.setColor(m_isHovered ? m_hoverTint : m_normalTint);

    if (m_isHovered && isMouseJustPressed && m_onClick)
    {
        m_onClick();
    }
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

void UIIcon::SetIconType(IconType type)
{
    m_iconType = type;
}

IconType UIIcon::GetIconType() const
{
    return m_iconType;
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

void UIIcon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (!m_enabled || !m_sprite.getTexture())
    {
        return;
    }

    target.draw(m_sprite, states);
}
