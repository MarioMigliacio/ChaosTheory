// ============================================================================
//  File        : UIGroupBox.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-07
//  Description : Represents a UI rect containter for many use cases
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIElement.h"
#include "UIPresets.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

/// @brief Simple enumeration for supported UIGroupBox Layout types.
enum class LayoutMode
{
    Vertical,
    Horizontal
};

// ============================================================================
//  Class       : UIGroupBox
//  Purpose     : Manages this UIGroupBox logic at the ui level.
//
//  Responsibilities:
//      - Set container position/size
//      - Adjust children layout
//      - Update and render all components
//
// ============================================================================
class UIGroupBox : public UIElement
{
  public:
    UIGroupBox(const sf::Vector2f &position, const sf::Vector2f &size);
    ~UIGroupBox() override = default;

    void SetTitle(const std::string &title, const sf::Font &font, unsigned int fontSize = 18);
    void AddElement(std::shared_ptr<UIElement> element);
    void RealignChildren();

    void Update(const sf::Vector2i &mousePosition, bool isMousePressed, bool isMouseJustPressed, float dt) override;
    bool Contains(const sf::Vector2i &point) const override;
    const std::vector<std::shared_ptr<UIElement>> &GetChildren() const;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;
    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

    void SetLayoutMode(LayoutMode mode);
    void SetCenterChildren(bool center);

    void SetFillColor(const sf::Color &color);
    void SetOutlineColor(const sf::Color &color);
    void SetOutlineThickness(float thickness);

    void SetInternalPadding(float padding);
    void SetEdgePadding(float padding);

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::RectangleShape m_background;
    sf::Text m_title;

    std::vector<std::shared_ptr<UIElement>> m_children;
    LayoutMode m_layoutMode = LayoutMode::Vertical;

    float m_internalPadding = DEFAULT_GROUPBOX_INTERNAL_PAD;
    float m_edgePadding = DEFAULT_GROUPBOX_EDGE_PAD;

    bool m_centerChildren = false;
};
