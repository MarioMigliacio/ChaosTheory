// ============================================================================
//  File        : UIArrow.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-01
//  Description : Represents a clickable UI arrow that will perform an action
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIArrow.h"
#include "AssetManager.h"
#include "WindowManager.h"

UIArrow::UIArrow(const sf::Vector2f &position, ArrowDirection direction)
    : m_direction(direction), m_position(position), m_size(64.f, 64.f)
{
    LoadTexture();
    UpdateSprite();
}

void UIArrow::Update(const sf::Vector2i &mousePos, bool isMousePressed)
{
    m_hovered = m_sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));

    const float fadeSpeed = 100.f;
    const float scaleSpeed = 2.f;
    const float maxOpacity = 255.f;
    const float maxScale = 1.2f;
    const float baseScale = 1.0f;

    if (m_opacity < maxOpacity)
    {
        m_opacity = std::min(maxOpacity, m_opacity + fadeSpeed * 0.016f);
    }

    float targetScale = m_hovered ? maxScale : baseScale;
    if (m_scale < targetScale)
    {
        m_scale = std::min(targetScale, m_scale + scaleSpeed * 0.016f);
    }

    else if (m_scale > targetScale)
    {
        m_scale = std::max(targetScale, m_scale - scaleSpeed * 0.016f);
    }

    m_sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(m_opacity)));
    m_sprite.setScale(m_scale, m_scale);

    if (m_hovered && isMousePressed && !m_pressedLastFrame && m_onClick)
    {
        m_onClick();
    }

    m_pressedLastFrame = isMousePressed;
}

// per-pixel hover detection:
bool UIArrow::Contains(const sf::Vector2i &point) const
{
    if (!m_texture)
    {
        return false;
    }

    // Convert from screen coordinates to local sprite coordinates
    sf::Vector2f local = m_sprite.getInverseTransform().transformPoint(static_cast<sf::Vector2f>(point));

    // Ensure the point is within the texture bounds
    const sf::IntRect texRect(0, 0, m_texture->getSize().x, m_texture->getSize().y);
    if (!texRect.contains(static_cast<sf::Vector2i>(local)))
    {
        return false;
    }

    // Get the pixel data
    const sf::Image &image = m_texture->copyToImage();
    sf::Color pixel = image.getPixel(static_cast<unsigned>(local.x), static_cast<unsigned>(local.y));

    return pixel.a > 32; // Consider hover only if mostly opaque
}

// Sets the position for this UI Arrow.
void UIArrow::SetPosition(const sf::Vector2f &position)
{
    m_position = position;
    UpdateSprite();
}

// Returns the position for this UI Arrow.
sf::Vector2f UIArrow::GetPosition() const
{
    return m_position;
}

void UIArrow::SetSize(const sf::Vector2f &size)
{
    m_size = size; // unused since texture controls sizing, but kept for API consistency
}

// Returns the size for this Button.
sf::Vector2f UIArrow::GetSize() const
{
    return m_size;
}

void UIArrow::SetOnClick(std::function<void()> callback)
{
    m_onClick = std::move(callback);
}

void UIArrow::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}

void UIArrow::LoadTexture()
{
    const std::string textureName = "arrow_texture";
    AssetManager::Instance().LoadTexture(textureName, "assets/ui/arrow_texture.png");
    m_texture = &AssetManager::Instance().GetTexture(textureName);
    m_sprite.setTexture(*m_texture);
}

void UIArrow::UpdateSprite()
{
    m_sprite.setPosition(m_position);
    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);

    // Rotate based on direction
    switch (m_direction)
    {
        case ArrowDirection::Left:
            m_sprite.setRotation(0.f);
            break;
        case ArrowDirection::Right:
            m_sprite.setRotation(180.f);
            break;
        case ArrowDirection::Up:
            m_sprite.setRotation(90.f);
            break;
        case ArrowDirection::Down:
            m_sprite.setRotation(270.f);
            break;
    }

    m_sprite.setScale(m_scale, m_scale);
}
