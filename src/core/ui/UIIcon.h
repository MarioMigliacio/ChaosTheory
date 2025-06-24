// ============================================================================
//  File        : UIIcon.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-19
//  Description : Represents a square clickable icon with a sprite
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>

enum class IconType
{
    None,
    AtomicIcon,
    FireRateIcon,
    GasIcon,
    LifeIcon,
    PowerIcon,
    UpgradeIcon,
    WarpIcon
};

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
    void StartFalling(float delaySeconds = 5.f);
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
    float m_timeAlive = 0.f;
    float m_driftSpeed = 40.f;

    bool m_falling = false;
    bool m_expired = false;
    bool m_driftEnabled = false;
};
