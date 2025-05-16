// ============================================================================
//  File        : UITextLabel.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-23
//  Description : Represents a UI string element.
//                Can be used in menus and interactive scenes.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <string>

// ============================================================================
//  Class       : UITextLabel
//  Purpose     : Manages this UITextLabel logic at the ui level.
//
//  Responsibilities:
//      - Set string position, color, size, outline.
//      - Display string specifics during render.
//
// ============================================================================
class UITextLabel : public UIElement
{
  public:
    UITextLabel(const std::string &text, const sf::Font &font, unsigned int fontSize,
                const sf::Vector2f &position = {0.f, 0.f});
    ~UITextLabel() = default;

    // Disable copy
    UITextLabel(const UITextLabel &) = delete;
    UITextLabel &operator=(const UITextLabel &) = delete;

    // Allow move
    UITextLabel(UITextLabel &&) noexcept = default;
    UITextLabel &operator=(UITextLabel &&) noexcept = default;

    void SetText(const std::string &text);
    void SetFont(const sf::Font &font);
    void SetFontSize(unsigned int size);
    void SetColor(const sf::Color &color);
    void SetOutline(float thickness, const sf::Color &color);

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

  public:
    // common.
    virtual void Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt)
    {
    }
    bool Contains(const sf::Vector2i &) const override
    {
        return false;
    }

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    void CenterOrigin();

    sf::Text m_text;
};
