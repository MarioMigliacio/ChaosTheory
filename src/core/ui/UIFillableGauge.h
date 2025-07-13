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
#include "UIPresets.h"
#include "UITextLabel.h"
#include <SFML/Graphics.hpp>
#include <memory>

/// @brief Enumeration field allowing for preset color customization schemes for ease of use,
enum class GaugeColorScheme
{
    Health,
    Gas,
    Mana,
    Default,
};

/// @brief Enumeration field allowing for UITextLabels to exist Left, or Above of FillableGauge bar.
enum class GaugeTitlePosition
{
    Left,
    Above,
};

/// @brief Data structure holding internal configurations useful for FillableGauge construction.
/// @param position Vector2f relative screen position for the gauge.
/// @param size Vector2f relative screen size for the gauge.
/// @param orientation LayoutMode layout mode (horizontal or vertical) for gauge fill direction. [default horizontal]
/// @param colorScheme GaugeColorScheme specifying the visual color scheme of the gauge. [default default color scheme]
/// @param initialValue Float representing the starting fill level of the gauge (range: 0.0–1.0). [default 1.f]
/// @param borderThickness Float representing thickness of the gauge border. [default 0.f (none)]
/// @param borderColor sf::Color value for the border color of the gauge. [default transparent (none)]
/// @param showPercentage Boolean toggle to render percentage overlay on the gauge. [default false]
/// @param showTitle Boolean toggle to render a title label alongside the gauge. [default false]
/// @param titleText String label displayed when title is enabled. [default (empty)]
/// @param titleFontSize Font size used when title is enabled. [default 14U]
/// @param titleScheme UITextLabelScheme enum defining the text color style for the title. [default default color
/// scheme]
/// @param titlePosition GaugeTitlePosition defining title appearance. [default left position]
/// @param titlePadding Float amount of padding between title and gauge body. [default 8.f]
struct FillableGaugeConfig
{
    // Layout & Size
    sf::Vector2f position;
    sf::Vector2f size;
    LayoutMode orientation = LayoutMode::Horizontal;

    // Fill Scheme & Value
    GaugeColorScheme colorScheme = GaugeColorScheme::Default;
    float initialValue = DEFAULT_GAUGE_FULL_VALUE;

    // Border
    float borderThickness = 0.f;
    sf::Color borderColor = sf::Color::Transparent;

    // Percentage Overlay
    bool showPercentage = false;

    // Title Display
    bool showTitle = false;
    std::string titleText = std::string();
    unsigned int titleFontSize = DEFAULT_GAUGE_FONT_SIZE;
    UITextLabelScheme titleScheme = UITextLabelScheme::DefaultScheme;
    GaugeTitlePosition titlePosition = GaugeTitlePosition::Left;
    float titlePadding = DEFAULT_GAUGE_TITLE_PADDING;
};

// ============================================================================
//  Class       : UIFillableGauge
//  Purpose     : UI element which represents a rectangle bar that fills and
//                depletes dynamically, with customizable text displays.
//
//  Responsibilities:
//      - Adjust value when set, update display fillbar.
//      - Set color scheme and border if requested.
//      - Display title text or % filled if requested.
//
// ============================================================================
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

    void SetShowPercentage(bool show);
    void SetShowTitleLabel(const std::string &text, unsigned int fontSize, float padding,
                           UITextLabelScheme scheme = UITextLabelScheme::DefaultScheme,
                           GaugeTitlePosition position = GaugeTitlePosition::Left);
    void SetTitleLabel(const std::string &text);

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void UpdateFillVisual();

  private:
    float m_value = DEFAULT_GAUGE_FULL_VALUE;
    float m_titlePadding = DEFAULT_GAUGE_TITLE_PADDING;

    LayoutMode m_orientation = LayoutMode::Horizontal;
    GaugeTitlePosition m_titlePosition = GaugeTitlePosition::Left;

    std::shared_ptr<UITextLabel> m_percentageLabel;
    std::shared_ptr<UITextLabel> m_titleLabel;

    sf::RectangleShape m_fillBar;       // Foreground (value bar)
    sf::RectangleShape m_backgroundBar; // Background (empty portion)

    sf::Color m_barFillColor = DEFAULT_GAUGE_FILL_COLOR;
    sf::Color m_barBackgroundColor = DEFAULT_GAUGE_BACKGROUND_COLOR;

    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::Vector2f m_totalSize = {0.f, 0.f}; // Includes bar + optional title

    sf::RectangleShape m_border;

    bool m_showPercentage = false;
    bool m_showTitle = false;
    bool m_drawBorder = false;
};
