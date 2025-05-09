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

enum class ArrowDirection
{
    Left,
    Right,
    Up,
    Down
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
    explicit UIArrow(const sf::Vector2f &position, ArrowDirection direction);
    ~UIArrow() override = default;

    UIArrow(const UIArrow &) = delete;
    UIArrow &operator=(const UIArrow &) = delete;
    UIArrow(UIArrow &&) = delete;
    UIArrow &operator=(UIArrow &&) = delete;

    void Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt) override;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

    void SetOnClick(std::function<void()> callback);
    const ArrowDirection GetDirection() const;

  protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    void LoadTexture();
    void UpdateSprite();

  private:
    ArrowDirection m_direction;
    sf::Sprite m_sprite;
    sf::Texture *m_texture = nullptr;
    sf::Vector2f m_position;
    sf::Vector2f m_size;

    float m_opacity = 0.f;
    float m_scale = 1.0f;
    bool m_hovered = false;
    bool m_pressedLastFrame = false;
    std::function<void()> m_onClick;
};
