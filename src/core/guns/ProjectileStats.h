// ============================================================================
//  File        : ProjectileStats.h
//  Project     : ChaosTheory (CT)
// ============================================================================

#pragma once

#include "ProjectilePresets.h"

/// @brief Provides a clean data structure that makes ConfigurableGun customizable.
/// @param fireRate Float value for adjusting the rate which gun is capable of firing.
/// @param damage Float value for scaling single projectile damage. Generally casted to int for trading damage.
/// @param speed Float velocity that the projectile is capable of updating with.
/// @param tint sf::Color for special bullet coloring.
/// @param projectilesPerShot Int amount of bullets capable of being fired in a single TryFire.
/// @param homing Bool is the Gun capable of shooting Homing type projectiles?
/// @param piercing Bool is the Gun capable of shooting Piercing type projectiles?
struct ProjectileStats
{
    float fireRate = 0.3f;
    float damage = 10.f;
    float speed = 250.f;
    sf::Color tint = sf::Color::White;
    int projectilesPerShot = 1;
    bool homing = false;
    bool piercing = false;
};