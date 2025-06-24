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
    void SetOnClick(const std::function<void()> &callback);

    void SetHoverTint(const sf::Color &color);
    void SetDisabledTint(const sf::Color &color);
    void SetNormalTint(const sf::Color &color);

    void Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt) override;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

    void SetIconType(IconType type);
    IconType GetIconType() const;

  private:
    void ApplySpriteTransform();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    IconType m_iconType = IconType::None;

    sf::Vector2f m_size;
    sf::Sprite m_sprite;
    std::string m_textureKey;

    sf::Color m_normalTint = sf::Color::White;
    sf::Color m_hoverTint = sf::Color(200, 200, 255);
    sf::Color m_disabledTint = sf::Color(128, 128, 128, 150);
};
