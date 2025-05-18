// ============================================================================
//  File        : Background.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-16
//  Description : Provides simplified ResolutionScale with background images
//                and supports multiple layers, as well as fully controlable
//                directional updates. Implements parallax scrolling effect.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "Background.h"
#include "AssetManager.h"
#include "ResolutionScaleManager.h"
#include "WindowManager.h"

namespace
{
/// @brief Determines how fast the default scroll speed is.
constexpr float SCROLL_SPEED = 20.f;
} // namespace

/// @brief Constructor for the Background class.
Background::Background() : m_isParallax(false)
{
}

/// @brief Destructor for the Background class.
Background::~Background()
{
    Clear();
}

/// @brief Initialize a single Background texture, with no movement.
/// @param textureId Key value for texture to load.
void Background::InitStatic(const std::string &textureId)
{
    Clear();

    auto texture = AssetManager::Instance().GetTexture(textureId);

    if (!texture)
    {
        return;
    }

    ParallaxLayer layer;
    layer.textureId = textureId;
    layer.parallaxFactor = 0.0f;
    layer.sprite.setTexture(*texture);
    layer.offset = {0.f, 0.f};

    m_layers.push_back(layer);
    m_isParallax = false;
}

/// @brief Initialize a parallax Background, supporting multiple layers of textures with movement.
/// @param layerData A collection of <key, value> pairs consisting of TextureID, and Movement speed factor.
void Background::InitParallax(const std::vector<std::pair<std::string, float>> &layerData)
{
    Clear();

    for (const auto &[textureId, factor] : layerData)
    {
        auto texture = AssetManager::Instance().GetTexture(textureId);

        if (!texture)
        {
            continue;
        }

        ParallaxLayer layer;
        layer.textureId = textureId;
        layer.parallaxFactor = factor;
        layer.sprite.setTexture(*texture);
        layer.offset = {0.f, 0.f};

        m_layers.push_back(layer);
    }

    m_isParallax = true;
}

/// @brief Performs interal state management during a single frame.
/// @param dt delta time since last update.
void Background::Update(float dt)
{
    if (!m_isParallax)
    {
        return;
    }

    for (auto &layer : m_layers)
    {
        // increase the scroll offset per frame.
        layer.offset.x += SCROLL_SPEED * layer.parallaxFactor * dt * layer.motion.x;
        layer.offset.y += SCROLL_SPEED * layer.parallaxFactor * dt * layer.motion.y;

        // Wrap horizontal offset
        float texWidth = static_cast<float>(layer.sprite.getTexture()->getSize().x);

        if (layer.offset.x >= texWidth)
        {
            layer.offset.x -= texWidth;
        }

        if (layer.offset.x < 0.f)
        {
            layer.offset.x += texWidth;
        }

        // Wrap vertical offset
        float texHeight = static_cast<float>(layer.sprite.getTexture()->getSize().y);

        if (layer.offset.y >= texHeight)
        {
            layer.offset.y -= texHeight;
        }

        if (layer.offset.y < 0.f)
        {
            layer.offset.y += texHeight;
        }
    }
}

/// @brief Draw this Background to the Renderable Target.
/// @param window Render Target.
void Background::Draw(sf::RenderWindow &window)
{
    const auto winSize = window.getSize();
    auto &scaleMgr = ResolutionScaleManager::Instance();

    // Determine true scale based on window vs reference resolution
    const float scaleX = static_cast<float>(winSize.x) / static_cast<float>(scaleMgr.ReferenceResolutionX());
    const float scaleY = static_cast<float>(winSize.y) / static_cast<float>(scaleMgr.ReferenceResolutionY());

    for (auto &layer : m_layers)
    {
        const sf::Texture *texture = layer.sprite.getTexture();

        if (!texture)
        {
            continue;
        }

        // Calculate the scaled size of one full texture tile
        const float texWidth = static_cast<float>(texture->getSize().x);
        const float texHeight = static_cast<float>(texture->getSize().y);

        const float finalWidth = texWidth * scaleX;
        const float finalHeight = texHeight * scaleY;

        // Calculate where to start drawing based on offset (ensures smooth wrap)
        const float startX = -layer.offset.x * scaleX;
        const float startY = -layer.offset.y * scaleY;

        // Draw enough tiles to fully cover the screen
        for (float xPos = startX; xPos < static_cast<float>(winSize.x); xPos += finalWidth)
        {
            for (float yPos = startY; yPos < static_cast<float>(winSize.y); yPos += finalHeight)
            {
                layer.sprite.setPosition(xPos, yPos);
                layer.sprite.setScale(scaleX, scaleY);
                window.draw(layer.sprite);
            }
        }
    }
}

/// @brief Exposes a vector of motion to the TextureID, allowing it to update in any direction.
/// @param textureId Key for texture.
/// @param motion <x, y> spatial vector quantity allowing for range of motion.
void Background::SetLayerMotion(const std::string &textureId, const sf::Vector2f &motion)
{
    for (auto &layer : m_layers)
    {
        if (layer.textureId == textureId)
        {
            layer.motion = motion;
            break;
        }
    }
}

/// @brief Exposes the size of the collection of layers for this Background.
/// @return m_layers.size
size_t Background::GetLayerCount() const
{
    return m_layers.size();
}

/// @brief Returns the offset for the underlying Texture for this Background.
/// @param textureId Key texture to query.
/// @return layer.offset
sf::Vector2f Background::GetLayerOffset(const std::string &textureId) const
{
    for (const auto &layer : m_layers)
    {
        if (layer.textureId == textureId)
        {
            return layer.offset;
        }
    }

    return {0.f, 0.f};
}

/// @brief Clears the Background collection of any existing layers.
void Background::Clear()
{
    m_layers.clear();
    m_isParallax = false;
}
