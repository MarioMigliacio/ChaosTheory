// ============================================================================
//  File        : Button.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-17
//  Description : Represents a clickable UI button with hover and press states.
//                Can be used in menus and interactive scenes.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

// ============================================================================
//  Class       : Button
//  Purpose     : Manages the Button logic at the ui level.
//
//  Responsibilities:
//      - Set button position
//      - Perform logic during onClick
//      - Display button specifics during render
//
// ============================================================================
class Button : public sf::Drawable
{
  public:
    Button(const sf::Vector2f &position, const sf::Vector2f &size);

    void SetText(const std::string &text, const sf::Font &font, unsigned int size = 24);
    void SetCallback(std::function<void()> callback);
    void SetColors(const sf::Color &idle, const sf::Color &hover, const sf::Color &active);

    void Update(const sf::Vector2i &mousePosition, bool isMousePressed);

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::RectangleShape m_shape;
    sf::Text m_label;

    sf::Color m_idleColor;
    sf::Color m_hoverColor;
    sf::Color m_activeColor;

    bool m_isHovered = false;
    bool m_isPressed = false;
    std::function<void()> m_onClick;
};