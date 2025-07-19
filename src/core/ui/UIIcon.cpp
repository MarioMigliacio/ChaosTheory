// ============================================================================
//  File        : UIIcon.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-19
//  Description : Represents a square icon with a sprite.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIIcon.h"
#include "AssetManager.h"
#include "Macros.h"
#include "ResolutionScaleManager.h"
#include "WindowManager.h"

/// @brief Constants that can be adjusted throughout this UIIcon.
namespace
{
/// @brief Default drifting speed of the AtomicIcon.
constexpr float ATOMIC_ICON_DRIFT_SPEED = 60.f;

/// @brief Default fall delay for the AtomicIcon.
constexpr float ATOMIC_ICON_FALL_DELAY = 3.5f;

/// @brief Default drifting speed of the FireRateIcon.
constexpr float FIRERATE_ICON_DRIFT_SPEED = 70.f;

/// @brief Default fall delay for the FireRateIcon.
constexpr float FIRERATE_ICON_FALL_DELAY = 4.f;

/// @brief Default drifting speed of the GasIcon.
constexpr float GAS_ICON_DRIFT_SPEED = 50.f;

/// @brief Default fall delay for the GasIcon.
constexpr float GAS_ICON_FALL_DELAY = 5.f;

/// @brief Default drifting speed of the LifeIcon.
constexpr float LIFE_ICON_DRIFT_SPEED = 100.f;

/// @brief Default fall delay for the LifeIcon.
constexpr float LIFE_ICON_FALL_DELAY = 3.f;

/// @brief Default drifting speed of the PowerIcon.
constexpr float POWER_ICON_DRIFT_SPEED = 80.f;

/// @brief Default fall delay for the PowerIcon.
constexpr float POWER_ICON_FALL_DELAY = 2.5f;

/// @brief Default drifting speed of the UpgradeIcon.IconType
constexpr float UPGRADE_ICON_DRIFT_SPEED = 90.f;

/// @brief Default fall delay for the UpgradeIcon.
constexpr float UPGRADE_ICON_FALL_DELAY = 2.f;

/// @brief Adjustment for variance of sine phasing with alpha pulse.
constexpr float ALPHA_PULSE_VARIANCE_CONST = 6.f;
} // namespace

/// @brief Constructs a UI Icon object.
/// @param size Size to initialize with.
/// @param position Position to emplace.
UIIcon::UIIcon(const sf::Vector2f &size, const sf::Vector2f &position) : m_size(size)
{
    SetPosition(position);
    m_sprite.setColor(sf::Color::White);
}

/// @brief Sets the internal sprite texture.
/// @param textureKey Desired asset path key.
void UIIcon::SetTextureSkin(const std::string &textureKey)
{
    m_textureKey = textureKey;

    auto tex = AssetManager::Instance().GetTexture(textureKey);

    if (tex)
    {
        m_sprite.setTexture(*tex, true);
        ApplySpriteTransform();
    }

    else
    {
        CT_LOG_WARN("UIIcon: No texture found for {}.", textureKey);
    }
}

/// @brief Sets the internal Type for this UIIcon.
/// @param type Enumeration for m_iconType.
void UIIcon::SetIconType(IconType type)
{
    m_iconType = type;
}

/// @brief Returns the internal type this UIIcon represents.
/// @return m_iconType.
IconType UIIcon::GetIconType() const
{
    return m_iconType;
}

/// @brief Performs internal state management during a single frame.
/// @param mousePos Current MousePosition.
/// @param isLeftClick IsLeftClick?
/// @param isJustClicked IsJustClicked?
/// @param dt delta time since last update.
void UIIcon::Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt)
{
    if (m_expired)
    {
        return;
    }

    m_timeAlive += dt;

    if (m_timeAlive >= m_fallDelay && m_driftEnabled)
    {
        m_falling = true;
        ApplyAlphaPulse();

        auto pos = m_sprite.getPosition();
        pos.y += m_driftSpeed * dt;
        m_sprite.setPosition(pos);

        const auto winSize = WindowManager::Instance().GetWindow().getSize();

        if (pos.y > winSize.y + 32.f)
        {
            m_expired = true;
        }
    }
}

/// @brief Provokes logic for the Icon to begin drifting to lower Y coordinate bounds of screen if Drifting allowed.
void UIIcon::StartFalling()
{
    const float scaleY = ResolutionScaleManager::Instance().GetScaleY();

    m_timeAlive = 0.f;
    m_falling = false;
    m_expired = false;
    m_driftEnabled = true;

    // Set fall speed based on type
    switch (m_iconType)
    {
        case IconType::AtomicIcon:
            m_driftSpeed = ATOMIC_ICON_DRIFT_SPEED;
            m_fallDelay = ATOMIC_ICON_FALL_DELAY;
            break;
        case IconType::FireRateIcon:
            m_driftSpeed = FIRERATE_ICON_DRIFT_SPEED;
            m_fallDelay = FIRERATE_ICON_FALL_DELAY;
            break;
        case IconType::GasIcon:
            m_driftSpeed = GAS_ICON_DRIFT_SPEED;
            m_fallDelay = GAS_ICON_FALL_DELAY;
            break;
        case IconType::LifeIcon:
            m_driftSpeed = LIFE_ICON_DRIFT_SPEED;
            m_fallDelay = LIFE_ICON_FALL_DELAY;
            break;
        case IconType::PowerIcon:
            m_driftSpeed = POWER_ICON_DRIFT_SPEED;
            m_fallDelay = POWER_ICON_FALL_DELAY;
            break;
        case IconType::UpgradeIcon:
            m_driftSpeed = UPGRADE_ICON_DRIFT_SPEED;
            m_fallDelay = UPGRADE_ICON_FALL_DELAY;
            break;
        case IconType::WarpIcon:
            m_driftEnabled = false;
            m_driftSpeed = 0.f;
            break;
        case IconType::SpeakerIcon:
            m_driftEnabled = false;
            m_driftSpeed = 0.f;
            break;
        case IconType::DialogNextIcon:
            // intentional leave m_driftEnabled = true, but remove drift speed, so it does FadeAlpha, but doesn't move.
            m_driftSpeed = 0.f;
        default:
            break;
    }

    m_driftSpeed = m_driftSpeed * scaleY; // account for bigger resolution windows.
}

/// @brief Returns the state of if this Icon has drifted off screen, and needs to be removed.
/// @return m_expired.
bool UIIcon::IsExpired() const
{
    return m_expired;
}

/// @brief Returns the bounds of the internal sprite of this UIIcon.
/// @return rectangle surrounding m_sprite.
sf::FloatRect UIIcon::GetGlobalBounds() const
{
    return m_sprite.getGlobalBounds();
}

/// @brief Overloaded logic if this Icon is contained inside of a point.
/// @param point X,Y coordinate point to compare against us.
/// @return true / false.
bool UIIcon::Contains(const sf::Vector2i &point) const
{
    return m_sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

/// @brief Sets the internal position for the bounding sprite of this UIIcon.
/// @param position m_sprite.Position.
void UIIcon::SetPosition(const sf::Vector2f &position)
{
    m_sprite.setPosition(position);
    ApplySpriteTransform();
}

/// @brief Returns the position that is bound to the sprite of this UIIcon.
/// @return m_sprite.Postion.
sf::Vector2f UIIcon::GetPosition() const
{
    return m_sprite.getPosition();
}

/// @brief Sets the internal size for this UIIcon.
/// @param size new m_size.
void UIIcon::SetSize(const sf::Vector2f &size)
{
    m_size = size;
    ApplySpriteTransform();
}

/// @brief Returns the size for this UIIcon.
/// @return m_size.
sf::Vector2f UIIcon::GetSize() const
{
    return m_size;
}

/// @brief Adjust dynamically for size, and positioning of this UIIcon.
void UIIcon::ApplySpriteTransform()
{
    if (!m_sprite.getTexture())
    {
        return;
    }

    auto bounds = m_sprite.getLocalBounds();

    float scale = std::min(m_size.x / bounds.width, m_size.y / bounds.height);
    m_sprite.setScale(scale, scale);

    sf::Vector2f offset((m_size.x - bounds.width * scale) / 2.f, (m_size.y - bounds.height * scale) / 2.f);
    m_sprite.setPosition(m_sprite.getPosition() + offset);
}

/// @brief Adjust dynamically for color, alpha and effect for this UIIcon.
void UIIcon::ApplyAlphaPulse()
{
    // oscillate between visible and invisible with sin logic
    float alpha = 128.f + 127.f * std::sin(m_timeAlive * ALPHA_PULSE_VARIANCE_CONST);
    sf::Color color = m_sprite.getColor();
    color.a = static_cast<sf::Uint8>(alpha);
    m_sprite.setColor(color);
}

/// @brief Draw this FillableGauge to the Renderable Target.
/// @param target render target.
/// @param states optional sf::RenderStates.
void UIIcon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (!m_expired && m_sprite.getTexture())
    {
        target.draw(m_sprite, states);
    }
}
