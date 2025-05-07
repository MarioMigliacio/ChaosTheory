// ============================================================================
//  File        : UISelectableButton.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-27
//  Description : Represents a clickable UI button where only one option can
//                be selected in a group option setting.
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
//  Class       : UISelectableButton
//  Purpose     : Manages this Radio Button logic at the ui level.
//
//  Responsibilities:
//      - Set button position
//      - Perform logic during onClick
//      - Display button specifics during render
//
// ============================================================================
class UISelectableButton : public UIElement
{
  public:
    UISelectableButton(const sf::Vector2f &position, const sf::Vector2f &size);
    ~UISelectableButton() override = default;

    // Disable copy
    UISelectableButton(const UISelectableButton &) = delete;
    UISelectableButton &operator=(const UISelectableButton &) = delete;

    // Allow move
    UISelectableButton(UISelectableButton &&) noexcept = default;
    UISelectableButton &operator=(UISelectableButton &&) noexcept = default;

    void SetSelected(bool selected);
    bool IsSelected() const;

    void SetText(const std::string &text, const sf::Font &font, unsigned int size = 24);
    const std::string GetLabel() const;

    void SetTextColor(const sf::Color &color);
    void SetSelectedColor(const sf::Color &fillColor, const sf::Color &textColor);
    void SetHoverColor(const sf::Color &hoverColor);
    void SetFontSize(unsigned int size);
    void SetCallback(std::function<void()> onSelect);

    void Update(const sf::Vector2i &mousePosition, bool isMousePressed) override;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

  private:
    void CenterLabel();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void HandleClickLogic(bool isMousePressed);
    void UpdateVisualState();

  private:
    sf::RectangleShape m_shape;
    sf::Text m_label;

    sf::Color m_idleColor = BUTTON_DEFAULT_IDLE_COLOR;
    sf::Color m_hoverColor = BUTTON_DEFAULT_HOVER_COLOR;
    sf::Color m_selectedFillColor = BUTTON_DEFAULT_SELECTED_COLOR;
    sf::Color m_selectedTextColor = BUTTON_DEFAULT_SELECTED_TEXT_COLOR;
    sf::Color m_textColor = BUTTON_DEFAULT_TEXT_COLOR;

    unsigned int m_fontSize = BUTTON_DEFAULT_FONT_SIZE;

    bool m_isSelected = false;
    bool m_isHovered = false;

    std::function<void()> m_onSelect;
};
