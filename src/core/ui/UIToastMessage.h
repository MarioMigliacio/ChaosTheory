// ============================================================================
//  File        : UIToastMessage.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-08
//  Description : Implementation of UIToastMessage - brief on-screen messages
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <string>

// ============================================================================
//  Class       : UIToastMessage
//  Purpose     : Manages this UIToastMessage logic at the ui level.
//
//  Responsibilities:
//      - Set String position, size, and duration
//      - Display while duration is active
//
// ============================================================================
class UIToastMessage : public UIElement
{
  public:
    explicit UIToastMessage(const std::string &text, const sf::Vector2f &position, float durationSeconds,
                            const sf::Font &font, unsigned int fontSize = 20,
                            const sf::Color &textColor = sf::Color::White, bool centerOrigin = true);

    ~UIToastMessage() override = default;

    // Disable copy
    UIToastMessage(const UIToastMessage &) = delete;
    UIToastMessage &operator=(const UIToastMessage &) = delete;

    // Enable move
    UIToastMessage(UIToastMessage &&) noexcept = default;
    UIToastMessage &operator=(UIToastMessage &&) noexcept = default;

    virtual void Update(const sf::Vector2i &mousePos, bool isMousePressed, float dt) override;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

    bool IsExpired() const;

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void UpdateVisuals();

  private:
    sf::Text m_text;
    float m_duration = 0.f;
    float m_elapsed = 0.f;
    bool m_centerOrigin = true;
};
