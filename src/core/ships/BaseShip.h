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

#include <SFML/Graphics.hpp>

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

    virtual int GetAllegiance() const
    {
        return m_allegiance;
    }

    virtual sf::Vector2f GetPosition() const
    {
        return m_sprite.getPosition();
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
    sf::Sprite m_sprite;

    int m_health;
    float m_speed;

    int m_allegiance = 0;
    bool m_alive = true;
};
