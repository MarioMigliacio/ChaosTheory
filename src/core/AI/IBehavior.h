// ============================================================================
//  File        : IBehavior.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-08-15
//  Description : Interface for enemy AI behaviors. Ships inherit one behavior.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

class BaseShip; // forward declare, keeps interface concise and short - no need #include

// ============================================================================
//  Class       : IBehavior
//  Purpose     : Abstract Base class that provides a meaningful interface to
//                implement update movement and gun update logic.
//
//  Responsibilities:
//      - Update logic wrapped into one convenient public UpdateBehavior().
//      - Enforces inheriting behavior classes to override:
//          - UpdateMovementLogic()
//          - UpdateGunLogic()
//
// ============================================================================
class IBehavior
{
  public:
    virtual ~IBehavior() = default;

    /// @brief Provides the implementation for the behavior interface that links directly in the Ships Update.
    /// @param ship Pointer to the Ship which inherits this interface.
    /// @param dt Delta time since last update.
    virtual void UpdateBehavior(BaseShip &ship, float dt)
    {
        UpdateMovementLogic(ship, dt);
        UpdateGunLogic(ship, dt);
    }

  protected:
    /// @brief Deterministic state management of movement for inheriting class to provide.
    /// @param ship Pointer to the Ship which inherits this interface.
    /// @param dt Delta time since last update.
    virtual void UpdateMovementLogic(BaseShip &ship, float dt) = 0;

    /// @brief Deterministic state management of gun action for inheriting class to provide.
    /// @param ship Pointer to the Ship which inherits this interface.
    /// @param dt Delta time since last update.
    virtual void UpdateGunLogic(BaseShip &ship, float dt) = 0;
};
