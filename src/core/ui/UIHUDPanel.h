// ============================================================================
//  File        : UIHUDPanel.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-12
//  Description : Represents a UI containter holding Heads Up Display entities
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

enum class HUDSlotAlignment
{
    Left,
    Center, // optional
    Right
};

// ============================================================================
//  Class       : UIHUDPanel
//  Purpose     : Manages this UIHUDPanel logic at the ui level.
//
//  Responsibilities:
//      - Set container position/size
//      - Adjust children layout
//      - Update and render all components
//
// ============================================================================
class UIHUDPanel : public UIElement
{
  public:
    UIHUDPanel(const sf::Vector2f &position, const sf::Vector2f &size);
    ~UIHUDPanel() override = default;

    // Disable copy
    UIHUDPanel(const UIHUDPanel &) = delete;
    UIHUDPanel &operator=(const UIHUDPanel &) = delete;

    // Allow move
    UIHUDPanel(UIHUDPanel &&) noexcept = default;
    UIHUDPanel &operator=(UIHUDPanel &&) noexcept = default;

    void Update(const sf::Vector2i &mousePosition, bool isMousePressed, bool isMouseJustPressed, float dt) override;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

    void SetFillColor(const sf::Color &color);
    void SetOutlineColor(const sf::Color &color);
    void SetOutlineThickness(float thickness);

    void AddElement(std::shared_ptr<UIElement> element, HUDSlotAlignment alignment = HUDSlotAlignment::Left);
    const std::vector<std::shared_ptr<UIElement>> &GetChildren() const;

    void SetLayoutMode(LayoutMode mode);
    void SetCenterChildren(bool center);
    void SetInternalPadding(float padding);
    void SetEdgePadding(float padding);
    void RealignChildren();

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::RectangleShape m_background;
    LayoutMode m_layoutMode = LayoutMode::Horizontal;

    std::vector<std::shared_ptr<UIElement>> m_children;
    std::vector<std::shared_ptr<UIElement>> m_leftAnchored;
    std::vector<std::shared_ptr<UIElement>> m_rightAnchored;
    std::vector<std::shared_ptr<UIElement>> m_centerAnchored;

    sf::Vector2f m_position;
    sf::Vector2f m_size;

    float m_internalPadding = 4.f;
    float m_edgePadding = 4.f;
    bool m_centerChildren = false;
};