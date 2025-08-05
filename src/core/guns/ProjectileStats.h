// ============================================================================
//  File        : ProjectileStats.h
//  Project     : ChaosTheory (CT)
// ============================================================================

#pragma once

#include "GunPattern.h"

/// @brief Provides a clean data structure that makes ConfigurableGun customizable.
/// @param fireRate Float value for adjusting the rate which gun is capable of firing.
/// @param damage Float value for scaling single projectile damage. Generally casted to int for trading damage.
/// @param speed Float velocity that the projectile is capable of updating with.
/// @param tint sf::Color for special bullet coloring.
/// @param pattern GunPattern enumeration representing the type of Projectile fire pattern to use.
/// @param homing Bool is the Gun capable of shooting Homing type projectiles?
struct ProjectileStats
{
    float fireRate = 0.3f;
    float damage = 10.f;
    float speed = 250.f;
    sf::Color tint = sf::Color::White;
    GunPattern pattern = GunPattern::SingleShot;
    bool homing = false;
};