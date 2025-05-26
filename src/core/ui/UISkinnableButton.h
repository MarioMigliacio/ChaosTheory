// ============================================================================
//  File        : UISkinnableButton.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-18
//  Description : Represents a clickable UI button with hover and press states.
//                Can be used in menus and interactive scenes. This button
//                supports texture skins, for more customizable appearances.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIElement.h"
#include "UIPresets.h"
#include <functional>
#include <string>

// ============================================================================
//  Class       : UISkinnableButton
//  Purpose     : Manages this UISkinnableButton logic at the ui level.
//
//  Responsibilities:
//      - Set button position
//      - Perform logic during onClick
//      - Display button specifics during render
//
// ============================================================================
class UISkinnableButton : public UIElement
{
  public:
    UISkinnableButton(const sf::Vector2f &position, const sf::Vector2f &size);
    ~UISkinnableButton() override = default;

    // Disable copy
    UISkinnableButton(const UISkinnableButton &) = delete;
    UISkinnableButton &operator=(const UISkinnableButton &) = delete;

    // Allow move
    UISkinnableButton(UISkinnableButton &&) noexcept = default;
    UISkinnableButton &operator=(UISkinnableButton &&) noexcept = default;

    void SetTextureSkins(const std::string &idle, const std::string &hover);
    void SetCallback(std::function<void()> callback);
    void SetHoverScale(float scale);

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;
    void ApplySpriteTransform();

    void Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt) override;
    bool Contains(const sf::Vector2i &point) const override;

  protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::Sprite m_sprite;
    std::string m_textureIdle;
    std::string m_textureHover;

    sf::Vector2f m_size;
    sf::Vector2f m_position;

    sf::Color m_enabledColorMask = SKINNABLE_BUTTON_ENABLED_COLOR_MASK;
    sf::Color m_disabledColorMask = SKINNABLE_BUTTON_DISABLED_COLOR_MASK;

    float m_hoverScale = BUTTON_DEFAULT_SCALE_SIZE;
    bool m_isHovered = false;
    bool m_isPressed = false;

    std::function<void()> m_onClick;
};
