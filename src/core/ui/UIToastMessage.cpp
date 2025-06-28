// ============================================================================
//  File        : UIToastMessage.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-08
//  Description : Implementation of UIToastMessage - brief on-screen messages
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIToastMessage.h"
#include "ResolutionScaleManager.h"
#include "UIFactory.h"
#include "WindowManager.h"

/// @brief Constructor for the UIToastMessage.
/// @param text String representation for this UIToastMessage.
/// @param position Position for this UIToastMessage.
/// @param fontSize Font size to initialize the UITextLabel with.
/// @param durationSeconds Lifespan in delta time for this UIToastMessage.
/// @param centerOrigin Positional origin for the UIToastMessage.
/// @param scheme Style of UITextLabel to apply to ToastMessage.
UIToastMessage::UIToastMessage(const std::string &text, const sf::Vector2f &position, unsigned int fontSize,
                               float durationSeconds, bool centerOrigin, UITextLabelScheme scheme)
    : m_duration(durationSeconds), m_centerOrigin(centerOrigin)
{
    m_startY = 0.f;
    m_targetY = 0.f;

    InitTextLabel(text, position, fontSize, centerOrigin, scheme);
    SetPosition(position);
}

/// @brief Initializes this Toast message with the appropriate UITextLabel style.
/// @param text String to initialize with.
/// @param position Position to initialize with.
/// @param fontSize Font size to initialize with.
/// @param centerOrigin Whether or not the center the UITextLabel about the origin.
/// @param scheme Color scheme pattern to initialize the UITextLabel with.
void UIToastMessage::InitTextLabel(const std::string &text, const sf::Vector2f &position, unsigned int fontSize,
                                   bool centerOrigin, UITextLabelScheme scheme)
{
    TextLabelConfig cfg = INIT_TEXTLABEL_CONFIG(text, position, fontSize, centerOrigin, scheme);
    m_label = UIFactory::Instance().CreateTextLabel(cfg);
}

/// @brief Performs internal state management during a single frame. Note the default update signature is in line with
/// interface, but only delta time is relevant for this UIToastMessage.
/// @param none
/// @param none
/// @param none
/// @param dt delta time since last update.
void UIToastMessage::Update(const sf::Vector2i &, bool, bool, float dt)
{
    if (!IsEnabled())
    {
        return;
    }

    m_elapsed += dt;

    // Slide animation: interpolate Y position upward (toast rise)
    float t = std::min(1.f, m_elapsed / m_duration);
    float newY = m_startY + (m_targetY - m_startY) * t;
    auto pos = m_label->GetPosition();
    m_label->SetPosition({pos.x, newY});

    // Fade out near end
    if (m_elapsed >= m_duration - m_fadeOutDuration)
    {
        float fadeT = (m_duration - m_elapsed) / m_fadeOutDuration;
        m_alpha = 255.f * std::clamp(fadeT, 0.f, 1.f);
        auto fillColor = m_label->GetFillColor();
        auto outlineColor = m_label->GetOutlineColor();
        float thickness = m_label->GetOutlineThickness();
        fillColor.a = static_cast<sf::Uint8>(m_alpha);
        outlineColor.a = static_cast<sf::Uint8>(m_alpha);
        m_label->SetColor(fillColor);
        m_label->SetOutline(thickness, outlineColor);
    }
}

/// @brief Returns whether or not the point is within the bounds of this UIToastMessage.
/// @param point Point to compair against us.
/// @return true / false
bool UIToastMessage::Contains(const sf::Vector2i &point) const
{
    return m_label->Contains(point);
}

/// @brief Sets the position for this UIToastMessage.
/// @param position new text.position.
void UIToastMessage::SetPosition(const sf::Vector2f &position)
{
    // Calculate Y drift
    float drift = ResolutionScaleManager::Instance().ScaledReferenceY(TOAST_DEFAULT_DRIFT_PERCENTAGE);
    const auto winSize = WindowManager::Instance().GetWindow().getSize();

    // Clamp start and target Y within screen bounds
    m_targetY = std::min(position.y, winSize.y - drift);        // target must remain on-screen
    m_startY = std::min(position.y + drift, winSize.y - drift); // drift downward but within screen

    m_label->SetPosition({position.x, m_startY});
}

/// @brief Returns the current position for this UIToastMessage.
/// @return m_label->position.
sf::Vector2f UIToastMessage::GetPosition() const
{
    return m_label->GetPosition();
}

/// @brief Not relevant for this UIToastMessage, but consistent with interface.
/// @param none
void UIToastMessage::SetSize(const sf::Vector2f &)
{
    // Not applicable for text-only toast, but required by interface.
}

/// @brief Returns the size bounds for this UIToastMessage.
/// @return size.
sf::Vector2f UIToastMessage::GetSize() const
{
    return m_label->GetSize();
}

/// @brief Sets the font for this UIToastMessage.
/// @param font new m_label->font.
void UIToastMessage::SetFont(const sf::Font &font)
{
    m_label->SetFont(font);
}

/// @brief Sets the font for this UIToastMessage.
/// @param size new m_label->size.
void UIToastMessage::SetFontSize(unsigned int size)
{
    m_label->SetFontSize(size);
}

/// @brief Calls the ApplyTextLabelStyle method of the internal UITextLabel.
/// @param scheme The scheme or color pattern to apply.
/// @param labelBorderSize The border thickness to utilize for the label.
void UIToastMessage::ApplyStyle(UITextLabelScheme scheme, const float labelBorderSize)
{
    m_label->ApplyTextLabelStyle(scheme, labelBorderSize);
}

/// @brief Returns whether or not this UIToastMessage lifespan is expired.
/// @return true / false.
bool UIToastMessage::IsExpired() const
{
    return m_elapsed >= m_duration;
}

/// @brief Draw this UIToastMessage to the Renderable Target.
/// @param target render target.
/// @param states optional sf::RenderStates.
void UIToastMessage::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (IsEnabled() && !IsExpired())
    {
        target.draw(*m_label, states);
    }
}
