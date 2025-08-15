// ============================================================================
//  File        : GunPattern.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-01
//  Description : Holds definitions for gun upgrade paths.
//
//  License     : N/A Open source
// ============================================================================

#pragma once

/// @brief Enumeration providing the types of available Gun Spray patterns.
enum class GunPattern
{
    /// @brief A single shot in its most basic form.
    SingleShot,

    /// @brief A single shot that is directed to a set location at spawn time.
    DirectedShot,

    /// @brief A spread of two angle-directed shots.
    DoubleShot,

    /// @brief A single shot that expands in size as it travels.
    GrowingBullet,

    /// @brief A spread of three angle-directed shots.
    TripleShot,

    /// @brief A single shot that seeks out the nearest target at spawn time.
    HomingRocket,

    /// @brief A spread of four angle-directed shots.
    QuadShot,

    /// @brief A continuous stream of heavy damaging lazer beam.
    LazerBeam,

    /// @brief A spread of two angle-directed shots, along with a central lazer beam.
    LazerBeamDoubleShot,

    /// @brief A single shot that seeks out the nearest target upon spawn time, along with a central lazer beam.
    LazerBeamHomingRocket,

    /// @brief A plethora of shots accompanying a central lazer beam.
    LazerBeamHybrid,

    /// @brief A Final-Form spray pattern that is to be witnessed in all its glory.
    UltimateArcBeam
};

/// @brief Returns the next Gun Pattern type in line when upgrading.
/// @param current the current GunPattern which is in place.
/// @return The next GunPattern to advance to.
inline GunPattern GetNextPattern(GunPattern current)
{
    switch (current)
    {
        case GunPattern::SingleShot:
            return GunPattern::DoubleShot;
        case GunPattern::DoubleShot:
            return GunPattern::GrowingBullet;
        case GunPattern::GrowingBullet:
            return GunPattern::TripleShot;
        case GunPattern::TripleShot:
            return GunPattern::HomingRocket;
        case GunPattern::HomingRocket:
            return GunPattern::QuadShot;
        case GunPattern::QuadShot:
            return GunPattern::LazerBeam;
        case GunPattern::LazerBeam:
            return GunPattern::LazerBeamDoubleShot;
        case GunPattern::LazerBeamDoubleShot:
            return GunPattern::LazerBeamHomingRocket;
        case GunPattern::LazerBeamHomingRocket:
            return GunPattern::LazerBeamHybrid;
        case GunPattern::LazerBeamHybrid:
            return GunPattern::UltimateArcBeam;
        default:
            return GunPattern::SingleShot;
    }
}
