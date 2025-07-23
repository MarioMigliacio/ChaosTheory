// ============================================================================
//  File        : BaseProjectile.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-20
//  Description : Abstract base extraction for a projectile type object interface.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Allegiance.h"
#include <SFML/Graphics.hpp>

// ============================================================================
//  Class       : BaseProjectile
//  Purpose     : A versatile abstraction for common projectile behaviors.
//
//  Responsibilities:
//      - Provide Update, TryFire and TryFireTowards interface.
//
// ============================================================================
class BaseProjectile
{
  public:
    virtual ~BaseProjectile() = default;

    virtual void Update(float dt) = 0;

    virtual void Draw(sf::RenderTarget &target)
    {
        if (m_alive)
        {
            target.draw(m_sprite);
        }
    }

    virtual sf::Vector2f GetPosition() const
    {
        return m_sprite.getPosition();
    }

    virtual void SetPosition(const sf::Vector2f &pos)
    {
        m_sprite.setPosition(pos);
    }

    virtual sf::FloatRect GetBounds() const
    {
        return m_sprite.getGlobalBounds();
    }

    virtual bool IsAlive() const
    {
        return m_alive;
    }

    virtual void Kill()
    {
        m_alive = false;
    }

    virtual Allegiance GetAllegiance() const
    {
        return m_allegiance;
    }

    virtual void SetAllegiance(const Allegiance allegiance)
    {
        m_allegiance = allegiance;
    }

    virtual int GetDamage() const
    {
        return m_damage;
    }

    virtual void SetDamage(const int dmg)
    {
        m_damage = dmg;
    }

    virtual sf::Vector2f GetVelocity() const
    {
        return m_velocity;
    }

    virtual void SetVelocity(const sf::Vector2f velocity)
    {
        m_velocity = velocity;
    }

    virtual void SetScale(float scaleX, float scaleY)
    {
        m_sprite.setScale(scaleX, scaleY);
    }

  protected:
    Allegiance m_allegiance = Allegiance::Neutral;

    sf::Sprite m_sprite;
    sf::Vector2f m_velocity;

    int m_damage = 0;

    bool m_alive = true;
};
