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

#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <functional>

/// @brief Data structure holding internal configurations useful for ArrowConfig construction.
/// @param position Vector2f position for Arrow.
/// @param size Vector2f size for Arrow.
/// @param textureKey string texture key for Arrow.
/// @param onClick function callback to trigger for Arrow.
struct ArrowConfig
{
    sf::Vector2f position;
    sf::Vector2f size;
    std::string textureKey;
    std::function<void()> onClick;
};

// ============================================================================
//  Class       : UIArrow
//  Purpose     : Manages this UIArrow logic at the ui level.
//
//  Responsibilities:
//      - Set arrow position
//      - Perform logic during onClick
//      - Display arrow specifics during render
//
// ============================================================================
class UIArrow : public UIElement
{
  public:
    UIArrow(const sf::Vector2f &position, const sf::Vector2f &size, const std::string &textureKey);
    ~UIArrow() override = default;

    // Disable copy
    UIArrow(const UIArrow &) = delete;
    UIArrow &operator=(const UIArrow &) = delete;

    // Allow move
    UIArrow(UIArrow &&) noexcept = default;
    UIArrow &operator=(UIArrow &&) noexcept = default;

    void Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt) override;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetTextureSkin(const std::string &texture);

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

    void SetOnClick(std::function<void()> callback);

  protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    void UpdateSprite();

  private:
    sf::Sprite m_sprite;
    sf::String m_texture;
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::Vector2f m_baseScale;

    float m_opacity = 0.f;
    float m_scale = 1.0f;
    float m_animationTime = 0.f;

    bool m_hovered = false;
    bool m_pressedLastFrame = false;

    std::function<void()> m_onClick;
};
