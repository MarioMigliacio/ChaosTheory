// ============================================================================
//  File        : BaseShip.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-17
//  Description : Abstract base extraction for a Ship type object interface.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Allegiance.h"
#include "BaseCollidable.h"
#include "ConfigurableGun.h"
#include "Macros.h"
#include "ProjectileStats.h"
#include <SFML/Graphics.hpp>

// ============================================================================
//  Class       : BaseShip
//  Purpose     : A versatile abstraction for common ship behaviors that also
//                supports collision detection via BaseCollidable interface.
//
//  Responsibilities:
//      - Provide Draw, Render, Update interface.
//      - Provide Accessors and Setters, including:
//          - Health, Position, Aliveness, Allegiance.
//
// ============================================================================
class BaseShip : public BaseCollidable
{
  public:
    virtual ~BaseShip() = default;

    /// @brief Update logic per frame.
    virtual void Update(float dt) = 0;

    /// @brief Allows unit to customize the base gun stats they spawn with.
    virtual void InitializeGunStats() = 0;

    /// @brief Draw the ship if it is alive.
    virtual void Draw(sf::RenderTarget &target)
    {
        if (m_alive)
        {
            target.draw(m_sprite);
        }
    }

    /// @brief Return the health of the ship.
    virtual int GetHealth() const
    {
        return m_health;
    }

    /// @brief Set ship health.
    virtual void SetHealth(const int maxHealth)
    {
        m_health = maxHealth;
    }

    /// @brief Apply damage and check for death.
    virtual void TakeDamage(int amount)
    {
        m_health -= amount;

        if (m_health <= 0)
        {
            m_health = 0;
            m_alive = false;
            CT_LOG_DEBUG("Ship has taken catastrophic damage and died.");
        }
    }

    /// @brief Implements BaseCollidable::IsAlive().
    bool IsAlive() const override
    {
        return m_alive;
    }

    /// @brief Commandable kill for this Ship.
    void Kill()
    {
        m_alive = false;
    }

    /// @brief Return allegiance (player, enemy, etc.).
    virtual Allegiance GetAllegiance() const
    {
        return m_allegiance;
    }

    /// @brief Set allegiance.
    virtual void SetAllegiance(const Allegiance allegiance)
    {
        m_allegiance = allegiance;
    }

    /// @brief Return world position.
    virtual sf::Vector2f GetPosition() const
    {
        return m_sprite.getPosition();
    }

    /// @brief Set world position.
    virtual void SetPosition(const sf::Vector2f &pos)
    {
        m_sprite.setPosition(pos);
    }

    /// @brief Move the ship by an offset.
    virtual void Move(const sf::Vector2f &offset)
    {
        m_sprite.move(offset);
    }

    /// @brief Implements BaseCollidable::GetBounds().
    sf::FloatRect GetBounds() const override
    {
        return m_sprite.getGlobalBounds();
    }

    /// @brief Set render scale.
    virtual void SetScale(float scaleX, float scaleY)
    {
        m_sprite.setScale(scaleX, scaleY);
    }

    /// @brief Return current speed vector.
    virtual sf::Vector2f GetSpeed() const
    {
        return m_speed;
    }

    /// @brief Set speed vector.
    virtual void SetSpeed(const sf::Vector2f &speed)
    {
        m_speed = speed;
    }

    /// @brief Get the current Gun for this Ship.
    virtual ConfigurableGun *GetGun()
    {
        return m_gun.get();
    }

    /// @brief Const version for Gun safety, get gun for readonly.
    virtual const ConfigurableGun *GetGun() const
    {
        return m_gun.get();
    }

    /// @brief Implements BaseCollidable::GetCollisionCategory().
    virtual CollisionCategory GetCollisionCategory() const override
    {
        return (m_allegiance == Allegiance::Player) ? CollisionCategory::Player : CollisionCategory::Enemy;
    }

  protected:
    Allegiance m_allegiance = Allegiance::Neutral;
    ProjectileStats m_gunStats;
    std::unique_ptr<ConfigurableGun> m_gun;
    sf::Sprite m_sprite;
    sf::Vector2f m_speed;

    int m_health = 0;
    bool m_alive = true;
};
