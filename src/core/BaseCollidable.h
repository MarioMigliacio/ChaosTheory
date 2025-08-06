// ============================================================================
//  File        : BaseCollidable.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-25
//  Description : Abstract interface for any object that can be collided with.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================
#pragma once

#include <SFML/Graphics.hpp>

/// @brief Enumeration field representing types of Collisions supported.
enum class CollisionCategory
{
    /// @brief A default type for no category.
    None = 0,

    /// @brief Represents the Player category.
    Player,

    /// @brief Represents the Enemy category.
    Enemy,

    /// @brief Represents the Projectile category.
    Projectile,

    /// @brief Represents the Icon category.
    Icon,

    /// @brief Represents the Environment category.
    Environment,
};

// ============================================================================
//  Class       : BaseCollidable
//  Purpose     : Pure abstract base class to provide support for inheriting
//                Classes to be used for CollisionManager hit detection logic.
//
//  Responsibilities:
//      - Get Rectangle Bounds of object.
//      - Get Alive state of object.
//      - Get Collision Category type of object.
//
// ============================================================================
class BaseCollidable
{
  public:
    virtual ~BaseCollidable() = default;

    /// @brief Return the axis-aligned bounding box for this object.
    virtual sf::FloatRect GetBounds() const = 0;

    /// @brief Return whether this object is still valid/alive in the world.
    virtual bool IsAlive() const = 0;

    /// @brief Return the category/type of this object for filtering.
    virtual CollisionCategory GetCollisionCategory() const = 0;
};
