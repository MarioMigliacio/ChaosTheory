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
#include "Assets.h"
#include "ResolutionScaleManager.h"
#include <cmath>

/// @brief Constants that can be adjusted throughout the UIArrow.
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

/// @brief Frequency in oscillations per second
constexpr float PULSE_SPEED = 4.0f;

/// @brief How much bigger/smaller than base
constexpr float PULSE_AMPLITUDE = 0.5f;
} // namespace

/// @brief Constructor for the UIArrow.
/// @param position Position to emplace.
/// @param size Size to initialize with.
/// @param direction Direction to face.
UIArrow::UIArrow(const sf::Vector2f &position, const sf::Vector2f &size, const std::string &textureKey)
    : m_position(position), m_size(size)
{
    SetTextureSkin(textureKey);
}

/// @brief Performs internal state management during a single frame.
/// @param mousePos Position for the mouse
/// @param isMousePressed IsMousePressed?
/// @param isMouseJustPressed IsMouseJustPressed?
/// @param dt delta time
void UIArrow::Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt)
{
    m_hovered = Contains(mousePos);

    if (m_opacity < MAX_OPACITY)
    {
        m_opacity = std::min(MAX_OPACITY, m_opacity + FADE_SPEED * dt);
    }

    if (!m_hovered)
    {
        m_animationTime += dt;

        float pulse = std::sin(m_animationTime * PULSE_SPEED);
        float dynamicScale = BASE_SCALE + PULSE_AMPLITUDE * pulse;

        m_sprite.setScale(m_baseScale.x * dynamicScale, m_baseScale.y * dynamicScale);
    }

    else
    {
        // Smoothly return to BASE_SCALE multiplier
        float targetScale = BASE_SCALE;
        float delta = SCALE_SPEED * dt;

        if (m_scale < targetScale)
        {
            m_scale = std::min(targetScale, m_scale + delta);
        }

        else if (m_scale > targetScale)
        {
            m_scale = std::max(targetScale, m_scale - delta);
        }

        m_sprite.setScale(m_baseScale.x * m_scale, m_baseScale.y * m_scale);
    }

    m_sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(m_opacity)));

    if (m_hovered && isMouseJustPressed)
    {
        m_onClick();
    }
}

/// @brief Determines if the point is bound in the texture. Using a more complex algorithm for a per-pixel computation.
/// @param point Point to compair against us.
/// @return true / false
bool UIArrow::Contains(const sf::Vector2i &point) const
{
    if (!m_sprite.getTexture())
    {
        return false;
    }

    // Step 1: Check bounding box
    if (!m_sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(point)))
    {
        return false;
    }

    // Step 2: Convert to local coordinates
    sf::Vector2f local = m_sprite.getInverseTransform().transformPoint(static_cast<sf::Vector2f>(point));

    const sf::Texture *tex = m_sprite.getTexture();

    if (!tex)
    {
        return false;
    }

    // Step 3: Get texture image
    const sf::Image &image = tex->copyToImage();

    // Step 4: Convert to pixel coordinates in image space
    unsigned int x = static_cast<unsigned int>(local.x);
    unsigned int y = static_cast<unsigned int>(local.y);

    if (x >= image.getSize().x || y >= image.getSize().y)
    {
        return false;
    }

    // Step 5: Check alpha
    const sf::Color pixel = image.getPixel(x, y);

    return pixel.a > 32; // Only consider "solid" pixels as interactive
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

/// @brief Sets the internal sprite image for this UIArrow.
/// @param texture Path to the texture asset.
void UIArrow::SetTextureSkin(const std::string &texture)
{
    m_texture = texture;

    auto tex = AssetManager::Instance().GetTexture(texture);

    if (tex)
    {
        m_sprite.setTexture(*tex, true);
        UpdateSprite();
    }
}

/// @brief Sets the size for this UIArrow.
/// @param size new m_size.
void UIArrow::SetSize(const sf::Vector2f &size)
{
    m_size = size;
    UpdateSprite();
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

/// @brief Draw this UIArrow to the render target.
/// @param target render target
/// @param states optional sf::RenderStates, unused.
void UIArrow::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}

/// @brief Correct the texture sprites position and orientation.
void UIArrow::UpdateSprite()
{
    if (m_sprite.getTexture())
    {
        const sf::Vector2u texSize = m_sprite.getTexture()->getSize();

        m_baseScale.x = m_size.x / static_cast<float>(texSize.x);
        m_baseScale.y = m_size.y / static_cast<float>(texSize.y);

        m_sprite.setPosition(m_position);
        m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);
        m_sprite.setScale(m_baseScale.x, m_baseScale.y);
    }
}
