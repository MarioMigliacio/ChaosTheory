// ============================================================================
//  File        : Allegiance.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-20
//  Description : Hosts the definition for Allegiances, used commonly among
//                Ship and Projectile and Environment objects.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

/// @brief Defines hostility and friendly relationships among entities.
enum class Allegiance
{
    /// @brief Represents the player.
    Player,

    /// @brief Represents any hostile entity that wants the player dead.
    Enemy,

    /// @brief Represents a 3rd party entity that wont hurt the player unless attacked.
    Neutral,

    /// @brief Represents a 3rd party entity that will never hurt the player, nor be hurt by the player.
    Friendly,

    /// @brief Represents a world object that may interact with other entities in certain conditions.
    Environment,
};

inline const char *ToString(Allegiance a)
{
    switch (a)
    {
        case Allegiance::Player:
            return "Player";
        case Allegiance::Enemy:
            return "Enemy";
        case Allegiance::Neutral:
            return "Neutral";
        case Allegiance::Friendly:
            return "Friendly";
        case Allegiance::Environment:
            return "Environment";
        default:
            return "Unknown";
    }
}