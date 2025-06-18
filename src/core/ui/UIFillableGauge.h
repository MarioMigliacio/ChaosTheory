// ============================================================================
//  File        : UIFillableGauge.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-13
//  Description : Visual bar UI element that fills based on a value
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>

enum class GaugeColorScheme
{
    Health,
    Gas,
    Mana,
    Default,
};

class UIFillableGauge : public UIElement
{
  public:
    UIFillableGauge(const sf::Vector2f &position, const sf::Vector2f &size);
    ~UIFillableGauge() override = default;

    // No copy
    UIFillableGauge(const UIFillableGauge &) = delete;
    UIFillableGauge &operator=(const UIFillableGauge &) = delete;

    // Allow move
    UIFillableGauge(UIFillableGauge &&) noexcept = default;
    UIFillableGauge &operator=(UIFillableGauge &&) noexcept = default;

    void Update(const sf::Vector2i &mousePosition, bool isMousePressed, bool isMouseJustPressed, float dt) override;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

    void SetValue(float percentage);
    void SetScheme(GaugeColorScheme scheme);
    void SetOrientation(LayoutMode orientation);
    void SetBorder(float thickness, const sf::Color &color);

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void UpdateFillVisual();

  private:
    float m_value = 1.f; // 100% full

    LayoutMode m_orientation = LayoutMode::Horizontal;
    sf::RectangleShape m_fillBar;       // Foreground (value bar)
    sf::RectangleShape m_backgroundBar; // Background (empty portion)

    sf::Color m_barFillColor = sf::Color::Green;
    sf::Color m_barBackgroundColor = sf::Color(50, 50, 50);

    sf::Vector2f m_position;
    sf::Vector2f m_size;

    sf::RectangleShape m_border;

    bool m_drawBorder = false;
};
