// ============================================================================
//  File        : UIIcon.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-19
//  Description : Represents a square icon with a sprite.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseCollidable.h"
#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>

/// @brief Enumeration type representing supported Icon type UI objects.
enum class IconType
{
    /// @brief Default nothing entity.
    None,

    /// @brief Atomic Icon grants a bomb item.
    AtomicIcon,

    /// @brief FireRate Icon grants faster shooting capability.
    FireRateIcon,

    /// @brief Gas Boost Icon grants permanent increased Fuel meter.
    GasBoostIcon,

    /// @brief Gas Restore Icon replenishes lost gas up to a certain amount.
    GasRestoreIcon,

    /// @brief Health Boost Icon grants permanent increased Health gauge.
    HealthBoostIcon,

    /// @brief Health Restore Icon replenishes lost health up to a certain amount.
    HealthRestoreIcon,

    /// @brief Life Icon grants an additional life.
    LifeIcon,

    /// @brief Power Icon grants increased damage per shot potential.
    PowerIcon,

    /// @brief Upgrade Icon grants enhanced damage signature pattern.
    UpgradeIcon,

    /// @brief Velocity Icon grants increased bullet travel speed.
    VelocityIcon,

    /// @brief Warp Icon allows for teleportation to matching warp icon.
    WarpIcon,

    /// @brief Represents an icon for a Speaker embedded in a ChatBox or other UI entity.
    SpeakerIcon,

    /// @brief Represents a Next indicator for a dialog ChatBox or other UI entity.
    DialogNextIcon,
};

/// @brief Provides a convinient IconType To String.
/// @param i IconType enumeration to convert to string.
/// @return const char* representation for an IconType enumeration.
inline const char *ToString(IconType i)
{
    switch (i)
    {
        case IconType::None:
            return "None";
        case IconType::AtomicIcon:
            return "AtomicIcon";
        case IconType::FireRateIcon:
            return "FireRateIcon";
        case IconType::GasBoostIcon:
            return "GasBoostIcon";
        case IconType::GasRestoreIcon:
            return "GasRestoreIcon";
        case IconType::HealthBoostIcon:
            return "HealthBoostIcon";
        case IconType::HealthRestoreIcon:
            return "HealthRestoreIcon";
        case IconType::LifeIcon:
            return "LifeIcon";
        case IconType::PowerIcon:
            return "PowerIcon";
        case IconType::UpgradeIcon:
            return "UpgradeIcon";
        case IconType::VelocityIcon:
            return "VelocityIcon";
        case IconType::WarpIcon:
            return "WarpIcon";
        case IconType::SpeakerIcon:
            return "SpeakerIcon";
        case IconType::DialogNextIcon:
            return "DialogNextIcon";
        default:
            return "Unknown";
    }
}

/// @brief Enumeration class useful for applying icon effect to player.
enum class IconEffectType
{
    /// @brief No effect.
    None,

    /// @brief Increment player bomb count.
    BombQuantityBoost,

    /// @brief Decrease time it takes to fire gun.
    GunFireRateBoost,

    /// @brief Boosts permanent gas maximum.
    GasBoost,

    /// @brief Restores current gas value.
    GasRestore,

    /// @brief  Boosts permanent health maximum.
    HealthBoost,

    /// @brief Restores current health value.
    HealthRestore,

    /// @brief Grants player an additional life.
    LifeIncrease,

    /// @brief Boosts raw power of projectiles fired by gun.
    GunDamageBoost,

    /// @brief Upgrades GunPattern fire type for gun.
    GunUpgradeBoost,

    /// @brief Boosts the speed of projectiles fired by gun.
    GunVelocityBoost,

    /// @brief Teleports the player to a new location.
    Teleport,
};

/// @brief Data structure holding internal configurations useful for IconConfig construction.
/// @param position Vector2f position for Icon Config.
/// @param size Vector2f size for Icon Config.
/// @param textureKey String texture key for Icon Config.
/// @param type IconType type for Icon Config.
struct IconConfig
{
    sf::Vector2f position;
    sf::Vector2f size;
    std::string textureKey;
    IconType type = IconType::None;
    IconEffectType effectType = IconEffectType::None;
};

// ============================================================================
//  Class       : UIIcon
//  Purpose     : Handles the logic for ui Icon elements providing robust
//                Enhancements during gameplay. Also supports collision
//                detection via BaseCollidable interface.
//
//  Responsibilities:
//      - Initializes and expires.
//      - Generate texture with bounding sprite.
//      - Update based on time.
//
// ============================================================================
class UIIcon : public UIElement, public BaseCollidable
{
  public:
    UIIcon(const sf::Vector2f &size, const sf::Vector2f &position);
    ~UIIcon() override = default;

    // Disable copy
    UIIcon(const UIIcon &) = delete;
    UIIcon &operator=(const UIIcon &) = delete;

    // Allow move
    UIIcon(UIIcon &&) noexcept = default;
    UIIcon &operator=(UIIcon &&) noexcept = default;

    void SetTextureSkin(const std::string &textureKey);
    void SetIconType(IconType type);
    IconType GetIconType() const;

    void Update(const sf::Vector2i &mousePos, bool isMousePressed, bool isMouseJustPressed, float dt) override;
    void StartFalling();
    bool IsExpired() const;
    void Expire();

    sf::FloatRect GetGlobalBounds() const;
    bool Contains(const sf::Vector2i &point) const override;

    void SetPosition(const sf::Vector2f &position) override;
    sf::Vector2f GetPosition() const override;

    void SetSize(const sf::Vector2f &size) override;
    sf::Vector2f GetSize() const override;

    sf::FloatRect GetBounds() const override;
    bool IsAlive() const override;
    CollisionCategory GetCollisionCategory() const override;

    IconEffectType GetEffectType() const;
    void SetEffectType(const IconEffectType type);

  private:
    void ApplySpriteTransform();
    void ApplyAlphaPulse();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    bool IsWorldIcon() const;

  private:
    sf::Sprite m_sprite;
    sf::Vector2f m_size;
    std::string m_textureKey;

    IconType m_iconType = IconType::None;
    IconEffectType m_effectType = IconEffectType::None;

    float m_fallDelay;
    float m_timeAlive;
    float m_driftSpeed;

    bool m_falling = false;
    bool m_expired = false;
    bool m_driftEnabled = false;
};
