// ============================================================================
//  File        : UIElement.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-17
//  Description : Hosts the definition for abstract UI elements and logic.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <SFML/Graphics.hpp>

// ============================================================================
//  Class       : UiElement
//  Purpose     : Provide a base abstraction layer for elements of the UI
//
//  Responsibilities:
//      - Provide Update pure virtual function
//      - Supports 'Contains' logic, for if the UIElement is being targetted
//      - Provide draw pure virtual function
//
// ============================================================================
class UIElement : public sf::Drawable
{
  public:
    virtual ~UIElement() = default;

    virtual void Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt) = 0;
    virtual bool Contains(const sf::Vector2i &point) const = 0;

    virtual void SetPosition(const sf::Vector2f &position) = 0;
    virtual sf::Vector2f GetPosition() const = 0;

    virtual void SetSize(const sf::Vector2f &size) = 0;
    virtual sf::Vector2f GetSize() const = 0;

  public:
    // Common, inheritting elements need not override these simple capabilities.
    void SetEnabled(bool enabled)
    {
        m_enabled = enabled;
    }

    bool IsEnabled() const
    {
        return m_enabled;
    }

  protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;

    bool m_enabled = true;
};