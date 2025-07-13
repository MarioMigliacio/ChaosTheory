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

#include "Macros.h"
#include "UIElement.h"
#include "UIPresets.h"
#include <SFML/Graphics.hpp>
#include <string>

/// @brief Data structure holding internal configurations useful for ToastConfig construction.
/// @param text String Label for Toast.
/// @param position Vector2f position for Toast.
/// @param duration float duration for Toast.
/// @param baseFontSize unsigned int original font size for Toast.
/// @param centerOrigin bool should center text origin for Toast.
/// @param scheme UITextLabelScheme Color Scheme for Toast.
struct ToastMessageConfig
{
    std::string text;
    sf::Vector2f position;
    float duration;
    unsigned int baseFontSize;
    bool centerOrigin;
    UITextLabelScheme scheme;
};

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
    UIToastMessage(const std::string &text, const sf::Vector2f &position, unsigned int fontSize, float durationSeconds,
                   bool centerOrigin = true, UITextLabelScheme scheme = UITextLabelScheme::DefaultScheme);

    ~UIToastMessage() override = default;

    // Disable copy
    UIToastMessage(const UIToastMessage &) = delete;
    UIToastMessage &operator=(const UIToastMessage &) = delete;

    // Enable move
    UIToastMessage(UIToastMessage &&) noexcept = default;
    UIToastMessage &operator=(UIToastMessage &&) noexcept = default;

    void InitTextLabel(const std::string &text, const sf::Vector2f &position, unsigned int fontSize,
                       bool centerOrigin = true, UITextLabelScheme scheme = UITextLabelScheme::DefaultScheme);

    virtual void Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt) override;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

    void SetFont(const sf::Font &font);
    void SetFontSize(unsigned int size);
    void ApplyStyle(UITextLabelScheme scheme, const float labelBorderSize);

    bool IsExpired() const;

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    std::shared_ptr<UITextLabel> m_label;

    float m_duration = TOAST_DEFAULT_DURATION;
    float m_elapsed = 0.0f;

    float m_startY = 0.0f;
    float m_targetY = 0.0f;
    float m_alpha = 255.f;
    float m_fadeOutDuration = TOAST_DEFAULT_FADE_DURATION;

    bool m_centerOrigin = true;
};
