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
#include "BaseCollidable.h"
#include <SFML/Graphics.hpp>
#include <cmath>

// ============================================================================
//  Class       : BaseProjectile
//  Purpose     : A versatile abstraction for common projectile behavior and
//                implements the BaseCollidable interface for collision
//                management.
//
//  Responsibilities:
//      - Provide Update, TryFire and TryFireTowards interface.
//
// ============================================================================
class BaseProjectile : public BaseCollidable
{
  public:
    virtual ~BaseProjectile() = default;

    /// @brief Called every frame to update projectile movement and state.
    virtual void Update(float dt) = 0;

    /// @brief Draws the projectile if still alive.
    virtual void Draw(sf::RenderTarget &target)
    {
        if (m_alive)
        {
            target.draw(m_sprite);
        }
    }

    /// @brief Get current position.
    virtual sf::Vector2f GetPosition() const
    {
        return m_sprite.getPosition();
    }

    /// @brief Set projectile position.
    virtual void SetPosition(const sf::Vector2f &pos)
    {
        m_sprite.setPosition(pos);
    }

    /// @brief Implements BaseCollidable::GetBounds().
    sf::FloatRect GetBounds() const override
    {
        return m_sprite.getGlobalBounds();
    }

    /// @brief Implements BaseCollidable::IsAlive().
    bool IsAlive() const override
    {
        return m_alive;
    }

    /// @brief Kill this projectile.
    virtual void Kill()
    {
        m_alive = false;
    }

    /// @brief Return allegiance (Player, Enemy, etc.)
    virtual Allegiance GetAllegiance() const
    {
        return m_allegiance;
    }

    /// @brief Set allegiance.
    virtual void SetAllegiance(const Allegiance allegiance)
    {
        m_allegiance = allegiance;
    }

    /// @brief Get the damage value.
    virtual float GetDamage() const
    {
        return m_damage;
    }

    /// @brief Get the damage as an int.
    virtual int GetDamageAsInt() const
    {
        return static_cast<int>(std::round(m_damage));
    }

    /// @brief Set damage value.
    virtual void SetDamage(const int dmg)
    {
        m_damage = dmg;
    }

    /// @brief Get projectile velocity.
    virtual sf::Vector2f GetVelocity() const
    {
        return m_velocity;
    }

    /// @brief Set projectile velocity.
    virtual void SetVelocity(const sf::Vector2f velocity)
    {
        m_velocity = velocity;
    }

    /// @brief Set render scale.
    virtual void SetScale(float scaleX, float scaleY)
    {
        m_sprite.setScale(scaleX, scaleY);
    }

    /// @brief Implements BaseCollidable::GetCollisionCategory().
    CollisionCategory GetCollisionCategory() const override
    {
        return CollisionCategory::Projectile;
    }

  protected:
    Allegiance m_allegiance = Allegiance::Neutral;

    sf::Sprite m_sprite;
    sf::Vector2f m_velocity;

    float m_damage = 0;
    bool m_alive = true;
};
