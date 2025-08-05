// ============================================================================
//  File        : BaseGun.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-19
//  Description : Abstract base extraction for firearm behavior.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Allegiance.h"
#include "BaseProjectile.h"
#include "ProjectileFactory.h"
#include <SFML/Graphics.hpp>
#include <memory>

// ============================================================================
//  Class       : BaseGun
//  Purpose     : A versatile abstraction for common gun behaviors.
//
//  Responsibilities:
//      - Provide Update, TryFire and TryFireTowards interface.
//      - Implement simple GetBarrelOffset, OwnerPosition, Allegiance etc.
//
// ============================================================================
class BaseGun
{
  public:
    virtual ~BaseGun() = default;

    /// @brief Perform routine maintenance during a frame.
    /// @param dt delta time sincec last update.
    virtual void Update(float dt) = 0;

    /// @brief Interface for which any interiting Gun shall override for Firing behavior.
    /// @return A safe pointer to the projectile which was fired.
    virtual std::shared_ptr<BaseProjectile> TryFire() = 0;

    /// @brief Interface for which any inheriting Gun shall override for Firing towards a specific location.
    /// @param targetPos Targetted position to fire towards for the lifetime of the projectile.
    /// @return A safe pointer to the projectile which was fired.
    virtual std::shared_ptr<BaseProjectile> TryFireTowards(const sf::Vector2f &targetPos) = 0;

  public:
    /// @brief Returns the offset for attaching this gun to a unit.
    virtual sf::Vector2f GetBarrelOffset() const
    {
        return m_barrelOffset;
    }

    /// @brief Sets the position for this gun.
    /// @param position Position to which the owner for this gun resides.
    virtual void SetOwnerPosition(const sf::Vector2f &position)
    {
        m_ownerPosition = position;
    }

    /// @brief Sets the allegiance and computes barrel offset from sprite size.
    /// @param allegiance Allegiance for the gun.
    /// @param spriteSize Sprite dimensions to calculate muzzle offset.
    void SetAllegiance(Allegiance allegiance, const sf::Vector2f &spriteSize)
    {
        m_allegiance = allegiance;
        m_barrelOffset = GetDefaultBarrelOffset(allegiance, spriteSize);
    }

    /// @brief Returns the barrel offset based on ship allegiance and sprite dimensions.
    /// @param allegiance Allegiance of the ship.
    /// @param spriteSize Dimensions of the ship's texture in pixels.
    /// @return Offset vector pointing to the gun barrel.
    virtual sf::Vector2f GetDefaultBarrelOffset(Allegiance allegiance, const sf::Vector2f &spriteSize) const
    {
        // Use half of the sprite height for y offset
        float offsetY = spriteSize.y / 2.f;

        switch (allegiance)
        {
            case Allegiance::Player:
            case Allegiance::Friendly:
                return {0.f, -offsetY};
            case Allegiance::Enemy:
                return {0.f, offsetY};
            default:
                return {0.f, 0.f}; // Neutral or Environment don't shoot
        }
    }

  protected:
    Allegiance m_allegiance = Allegiance::Neutral;

    sf::Vector2f m_ownerPosition;
    sf::Vector2f m_barrelOffset;

    float m_cooldown = 0.f;
    float m_fireRate = 0.3f;
};
