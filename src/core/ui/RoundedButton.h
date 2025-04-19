// ============================================================================
//  File        : RoundedButton.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-18
//  Description : Represents a clickable UI button with hover and press states.
//                Can be used in menus and interactive scenes.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

// ============================================================================
//  Class       : RoundedButton
//  Purpose     : Manages this Button logic at the ui level.
//
//  Responsibilities:
//      - Set button position
//      - Perform logic during onClick
//      - Display button specifics during render
//
// ============================================================================
class RoundedButton : public UIElement
{
  public:
    RoundedButton(const sf::Vector2f &position, const sf::Vector2f &size);
    ~RoundedButton() override = default;

    // Disable copy
    RoundedButton(const RoundedButton &) = delete;
    RoundedButton &operator=(const RoundedButton &) = delete;

    // Allow move
    RoundedButton(RoundedButton &&) noexcept = default;
    RoundedButton &operator=(RoundedButton &&) noexcept = default;

    void SetText(const std::string &text, const sf::Font &font, unsigned int size = 24);
    void SetCallback(std::function<void()> callback);
    void SetColors(const sf::Color &idle, const sf::Color &hover, const sf::Color &active);
    void SetCornerRadius(float radius);

    void Update(const sf::Vector2i &mousePos, bool isMousePressed) override;
    bool Contains(const sf::Vector2i &point) const override;

  private:
    // In SFML, the draw() method is intended to be overridden from sf::Drawable, which is a friend of sf::RenderTarget.
    // That’s why the draw method is typically marked private, and you don’t need to call it directly — SFML will handle
    // it via window.draw(...) when used
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::RectangleShape m_shape;
    sf::Text m_label;

    sf::Color m_idleColor;
    sf::Color m_hoverColor;
    sf::Color m_activeColor;

    bool m_isHovered = false;
    bool m_isPressed = false;

    float m_cornerRadius = 8.0f; // Rounded edge radius

    std::function<void()> m_onClick;
};
