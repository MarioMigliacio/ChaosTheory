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
#include "IGun.h"
#include <SFML/Graphics.hpp>

// ============================================================================
//  Class       : BaseShip
//  Purpose     : A versatile abstraction for common ship behaviors.
//
//  Responsibilities:
//      - Provide Draw, Render, Update interface.
//      - Provide Accessors and Setters, including:
//          - Health, Position, Aliveness, Allegiance.
//
// ============================================================================
class BaseShip
{
  public:
    virtual ~BaseShip() = default;

    virtual void Update(float dt) = 0;

  public:
    virtual void Draw(sf::RenderTarget &target)
    {
        if (m_alive)
        {
            target.draw(m_sprite);
        }
    }

    virtual int GetHealth() const
    {
        return m_health;
    }

    virtual void SetHealth(const int maxHealth)
    {
        m_health = maxHealth;
    }

    virtual void TakeDamage(int amount)
    {
        m_health -= amount;

        if (m_health <= 0)
        {
            m_health = 0;
            m_alive = false;
        }
    }

    virtual bool IsAlive() const
    {
        return m_alive;
    }

    virtual Allegiance GetAllegiance() const
    {
        return m_allegiance;
    }

    virtual void SetAllegiance(const Allegiance allegiance)
    {
        m_allegiance = allegiance;
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

    virtual void SetScale(float scaleX, float scaleY)
    {
        m_sprite.setScale(scaleX, scaleY);
    }

  protected:
    virtual void ApplyDifficultyScaling() = 0;

  protected:
    Allegiance m_allegiance = Allegiance::Neutral;

    std::shared_ptr<IGun> m_gun;
    sf::Sprite m_sprite;

    float m_speed = 0;
    int m_health = 0;

    bool m_alive = true;
};
