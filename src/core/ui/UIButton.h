// ============================================================================
//  File        : UIButton.h
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
#include "UIPresets.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

// ============================================================================
//  Class       : UIButton
//  Purpose     : Manages this UIButton logic at the ui level.
//
//  Responsibilities:
//      - Set button position
//      - Perform logic during onClick
//      - Display button specifics during render
//
// ============================================================================
class UIButton : public UIElement
{
  public:
    UIButton(const sf::Vector2f &position, const sf::Vector2f &size);
    ~UIButton() override = default;

    // Disable copy
    UIButton(const UIButton &) = delete;
    UIButton &operator=(const UIButton &) = delete;

    // Allow move
    UIButton(UIButton &&) noexcept = default;
    UIButton &operator=(UIButton &&) noexcept = default;

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
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

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

    sf::Color m_idleColor = BUTTON_DEFAULT_IDLE_COLOR;
    sf::Color m_hoverColor = BUTTON_DEFAULT_HOVER_COLOR;
    sf::Color m_activeColor = BUTTON_DEFAULT_ACTIVE_COLOR;
    sf::Color m_textColor = BUTTON_DEFAULT_TEXT_COLOR;

    unsigned int m_fontSize = BUTTON_DEFAULT_FONT_SIZE;
    float m_hoverScale = 1.05f;

    bool m_isHovered = false;
    bool m_isPressed = false;

    std::function<void()> m_onClick;
};