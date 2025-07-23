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

    virtual void Update(float dt) = 0;
    virtual std::shared_ptr<BaseProjectile> TryFire() = 0;
    virtual std::shared_ptr<BaseProjectile> TryFireTowards(const sf::Vector2f &targetPos) = 0;

  public:
    virtual sf::Vector2f GetBarrelOffset() const
    {
        return m_barrelOffset;
    }

    virtual void SetOwnerPosition(const sf::Vector2f &position)
    {
        m_ownerPosition = position;
    }

    void SetAllegiance(Allegiance allegiance)
    {
        m_allegiance = allegiance;
        m_barrelOffset = GetDefaultBarrelOffset(allegiance);
    }

    virtual sf::Vector2f GetDefaultBarrelOffset(Allegiance allegiance) const
    {
        // TODO: Know that some ships barrel may be 32.f later -> add parameter based on ship type.
        constexpr float offsetY = 8.f;

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
