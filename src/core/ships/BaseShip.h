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
#include "BaseGun.h"
#include "Macros.h"
#include "ProjectileStats.h"
#include <SFML/Graphics.hpp>
#include <memory>

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

  public:
    // from BaseCollidable:

    bool IsAlive() const override;
    sf::FloatRect GetBounds() const override;
    CollisionCategory GetCollisionCategory() const override;

  public:
    virtual void Draw(sf::RenderTarget &target);
    virtual void SetTint(const sf::Color &color);

    virtual int GetHealth() const;
    virtual void SetHealth(const int maxHealth);
    virtual int GetMaxHealth() const;
    virtual void TakeDamage(const float amount);

    virtual void CullIfOffscreen();
    virtual void Kill();

    virtual Allegiance GetAllegiance() const;
    virtual void SetAllegiance(const Allegiance allegiance);

    virtual sf::Vector2f GetPosition() const;
    virtual void SetPosition(const sf::Vector2f &pos);
    virtual void SetRotation(const float deg);
    virtual void Move(const sf::Vector2f &offset);

    virtual void SetScale(float scaleX, float scaleY);

    virtual sf::Vector2f GetSpeed() const;
    virtual void SetSpeed(const sf::Vector2f &speed);

    virtual BaseGun *GetGun();
    virtual const BaseGun *GetGun() const;

  protected:
    Allegiance m_allegiance = Allegiance::Neutral;
    ProjectileStats m_gunStats;
    std::unique_ptr<BaseGun> m_gun;
    sf::Sprite m_sprite;
    sf::Vector2f m_speed;

    float m_health = 0;
    float m_maxHealth = 0;

    bool m_alive = true;
};
