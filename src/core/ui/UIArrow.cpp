// ============================================================================
//  File        : UIArrow.cpp
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
#include "ResolutionScaleManager.h"

namespace
{
/// @brief Controls the speed at which this UIArrow fades.
constexpr float FADE_SPEED = 100.f;

/// @brief Controls the speed at which this UIArrow achieves its Scale size.
constexpr float SCALE_SPEED = 2.f;

/// @brief Controls the cap of visible opacity for this UIArrow.
constexpr float MAX_OPACITY = 255.f;

/// @brief Caps the scale for this UIArrow.
constexpr float MAX_SCALE = 1.5f;

/// @brief Base scale for this UIArrow.
constexpr float BASE_SCALE = 1.0f;
} // namespace

/// @brief Constructor for the UIArrow.
/// @param position Position to emplace.
/// @param direction Direction to face.
UIArrow::UIArrow(const sf::Vector2f &position, ArrowDirection direction)
    : m_direction(direction), m_position(position), m_size(64.f, 64.f)
{
    LoadTexture();
    UpdateSprite();
}

/// @brief Performs internal state management during a single frame.
/// @param mousePos Position for the mouse
/// @param isMousePressed IsMousePressed?
/// @param isMouseJustPressed IsMouseJustPressed?
/// @param dt delta time
void UIArrow::Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt)
{
    m_hovered = m_sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));

    if (m_opacity < MAX_OPACITY)
    {
        m_opacity = std::min(MAX_OPACITY, m_opacity + FADE_SPEED * 0.016f);
    }

    float targetScale = m_hovered ? MAX_SCALE : BASE_SCALE;

    if (m_scale < targetScale)
    {
        m_scale = std::min(targetScale, m_scale + SCALE_SPEED * 0.016f);
    }

    else if (m_scale > targetScale)
    {
        m_scale = std::max(targetScale, m_scale - SCALE_SPEED * 0.016f);
    }

    m_sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(m_opacity)));
    m_sprite.setScale(m_scale, m_scale);

    if (m_hovered && isMouseJustPressed)
    {
        m_onClick();
    }
}

/// @brief Determines if the point is bound in the texture.
/// @param point Point to compair against us.
/// @return true / false
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

/// @brief Sets the position for this UI Arrow.
/// @param position new m_position.
void UIArrow::SetPosition(const sf::Vector2f &position)
{
    m_position = position;
    UpdateSprite();
}

/// @brief Returns the position for this UIArrow.
/// @return m_position.
sf::Vector2f UIArrow::GetPosition() const
{
    return m_position;
}

/// @brief Sets the size for this UIArrow.
/// @param size new m_size.
void UIArrow::SetSize(const sf::Vector2f &size)
{
    m_size = size; // unused since texture controls sizing, but kept for API consistency
}

/// @brief Returns the size for this UIArrow.
/// @return m_size.
sf::Vector2f UIArrow::GetSize() const
{
    return m_size;
}

/// @brief Sets the function pointer to the callback.
/// @param callback new m_onClick.
void UIArrow::SetOnClick(std::function<void()> callback)
{
    m_onClick = std::move(callback);
}

/// @brief Gets the direction for this UIArrow.
/// @return m_direction.
const ArrowDirection UIArrow::GetDirection() const
{
    return m_direction;
}

/// @brief Draw this UIArrow to the render target.
/// @param target render target
/// @param states optional sf::RenderStates, unused.
void UIArrow::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}

/// @brief Load the texture into usable sprite for this UIArrow.
void UIArrow::LoadTexture()
{
    const std::string textureName = "arrow_texture";
    AssetManager::Instance().LoadTexture(textureName, "assets/ui/arrow_texture.png");
    m_texture = &AssetManager::Instance().GetTexture(textureName);
    m_sprite.setTexture(*m_texture);
}

/// @brief Correct the texture sprites position and orientation.
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
