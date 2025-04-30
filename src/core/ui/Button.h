// ============================================================================
//  File        : Button.h
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

//
// Default Colors for Button States
//
const sf::Color DEFAULT_IDLE_COLOR(200, 200, 200);
const sf::Color DEFAULT_HOVER_COLOR(160, 160, 255);
const sf::Color DEFAULT_ACTIVE_COLOR(100, 100, 255);
const sf::Color DEFAULT_DISABLED_IDLE_COLOR(100, 100, 100);
const sf::Color DEFAULT_DISABLED_HOVER_COLOR(100, 100, 100);
const sf::Color DEFAULT_TEXT_COLOR(0, 0, 0);
const sf::Color DEFAULT_SELECTED_COLOR(100, 180, 255);
const sf::Color DEFAULT_SELECTED_TEXT_COLOR(255, 255, 255);
const sf::Color DEFAULT_DISABLED_TEXT_COLOR(200, 200, 200);

// ============================================================================
//  Class       : Button
//  Purpose     : Manages this Button logic at the ui level.
//
//  Responsibilities:
//      - Set button position
//      - Perform logic during onClick
//      - Display button specifics during render
//
// ============================================================================
class Button : public UIElement
{
  public:
    Button(const sf::Vector2f &position, const sf::Vector2f &size);
    ~Button() override = default;

    // Disable copy
    Button(const Button &) = delete;
    Button &operator=(const Button &) = delete;

    // Allow move
    Button(Button &&) noexcept = default;
    Button &operator=(Button &&) noexcept = default;

    void SetText(const std::string &text, const sf::Font &font, unsigned int size = 24);
    void SetCallback(std::function<void()> callback);

    void SetIdleColor(const sf::Color &color);
    void SetHoverColor(const sf::Color &color);
    void SetActiveColor(const sf::Color &color);
    void SetTextColor(const sf::Color &color);
    void SetFontSize(unsigned int size);
    void SetHoverScale(float scale);

    void Update(const sf::Vector2i &mousePosition, bool isMousePressed) override;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    void SetSize(const sf::Vector2f &size) override;

  private:
    void CenterLabel();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void UpdateScale();
    void UpdateFillColor(bool isMousePressed);
    void UpdateTextColor();
    void HandleClickLogic(bool isMousePressed);

  private:
    sf::RectangleShape m_shape;
    sf::Text m_label;

    sf::Color m_idleColor = DEFAULT_IDLE_COLOR;
    sf::Color m_hoverColor = DEFAULT_HOVER_COLOR;
    sf::Color m_activeColor = DEFAULT_ACTIVE_COLOR;
    sf::Color m_textColor = DEFAULT_TEXT_COLOR;

    unsigned int m_fontSize = 24;
    float m_hoverScale = 1.05f;

    bool m_isHovered = false;
    bool m_isPressed = false;

    std::function<void()> m_onClick;
};