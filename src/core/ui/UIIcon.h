// ============================================================================
//  File        : UIIcon.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-19
//  Description : Represents a square icon with a sprite.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>

/// @brief Enumeration type representing supported Icon type UI objects.
enum class IconType
{
    /// @brief Default nothing entity.
    None,

    /// @brief Atomic Icon grants a bomb item.
    AtomicIcon,

    /// @brief FireRate Icon grants faster shooting capability.
    FireRateIcon,

    /// @brief Gas Icon grants replenished Fuel meter.
    GasIcon,

    /// @brief Life Icon grants an additional life.
    LifeIcon,

    /// @brief Power Icon grants increased damage per shot potential.
    PowerIcon,

    /// @brief Upgrade Icon grants enhanced damage signature pattern.
    UpgradeIcon,

    /// @brief Warp Icon allows for teleportation to matching warp icon.
    WarpIcon,

    /// @brief Represents an icon for a Speaker embedded in a ChatBox or other UI entity.
    SpeakerIcon,

    /// @brief Represents a Next indicator for a dialog ChatBox or other UI entity.
    DialogNextIcon,
};

/// @brief Data structure holding internal configurations useful for IconConfig construction.
/// @param position Vector2f position for Icon Config.
/// @param size Vector2f size for Icon Config.
/// @param textureKey String texture key for Icon Config.
/// @param type IconType type for Icon Config.
struct IconConfig
{
    sf::Vector2f position;
    sf::Vector2f size;
    std::string textureKey;
    IconType type = IconType::None;
};

// ============================================================================
//  Class       : UIIcon
//  Purpose     : Handles the logic for ui Icon elements providing robust
//                Enhancements during gameplay.
//
//  Responsibilities:
//      - Initializes and expires.
//      - Generate texture with bounding sprite.
//      - Update based on time.
//
// ============================================================================
class UIIcon : public UIElement
{
  public:
    UIIcon(const sf::Vector2f &size, const sf::Vector2f &position);
    ~UIIcon() override = default;

    // Disable copy
    UIIcon(const UIIcon &) = delete;
    UIIcon &operator=(const UIIcon &) = delete;

    // Allow move
    UIIcon(UIIcon &&) noexcept = default;
    UIIcon &operator=(UIIcon &&) noexcept = default;

    void SetTextureSkin(const std::string &textureKey);
    void SetIconType(IconType type);
    IconType GetIconType() const;

    void Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt) override;
    void StartFalling();
    bool IsExpired() const;

    sf::FloatRect GetGlobalBounds() const;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

  private:
    void ApplySpriteTransform();
    void ApplyAlphaPulse();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::Sprite m_sprite;
    sf::Vector2f m_size;
    std::string m_textureKey;

    IconType m_iconType = IconType::None;

    float m_fallDelay;
    float m_timeAlive;
    float m_driftSpeed;

    bool m_falling = false;
    bool m_expired = false;
    bool m_driftEnabled = false;
};
