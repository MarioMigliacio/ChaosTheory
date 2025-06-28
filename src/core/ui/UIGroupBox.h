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
#include "UITextLabel.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

/// @brief Data structure holding internal configurations useful for GroupBoxConfig construction.
/// @param position Vector2f position for Group Box.
/// @param size Vector2f size for Group Box.
/// @param useTitle bool should title be used for Group Box [default false].
/// @param title string title label for Group Box [default = ""].
/// @param centerOrigin bool should center origin for Group Box title [default true].
/// @param scheme UITextLabelScheme Color Scheme for Group Box [default default color scheme].
struct GroupBoxConfig
{
    sf::Vector2f position;
    sf::Vector2f size;
    bool useTitle = false;
    std::string title = "";
    bool centerOrigin = true;
    UITextLabelScheme scheme = UITextLabelScheme::DefaultScheme;
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

    // Disable copy
    UIGroupBox(const UIGroupBox &) = delete;
    UIGroupBox &operator=(const UIGroupBox &) = delete;

    // Allow move
    UIGroupBox(UIGroupBox &&) noexcept = default;
    UIGroupBox &operator=(UIGroupBox &&) noexcept = default;

    void SetTitle(const std::string &title, const sf::Font &font, unsigned int fontSize = 18, bool centerOrigin = true,
                  UITextLabelScheme scheme = UITextLabelScheme::DefaultScheme);
    void SetTitleScheme(UITextLabelScheme scheme);
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
    std::shared_ptr<UITextLabel> m_titleLabel;
    sf::RectangleShape m_background;

    std::vector<std::shared_ptr<UIElement>> m_children;
    LayoutMode m_layoutMode = LayoutMode::Vertical;

    float m_internalPadding = DEFAULT_GROUPBOX_INTERNAL_PAD;
    float m_edgePadding = DEFAULT_GROUPBOX_EDGE_PAD;

    bool m_centerChildren = false;
};
