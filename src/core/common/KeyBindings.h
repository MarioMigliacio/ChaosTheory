// ============================================================================
//  File        : KeyBindings.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-23
//  Description : Contains useful constants for Key Bindings
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <string>

// ============================================================================
// Key Binding index strings
// ============================================================================

/// @brief Exposes relevent assets to the AudioAssets namespace.
namespace KeyBindings
{
/// @brief Key to the MoveUpConstantKey.
const auto MoveUpConstantKey = "MoveUp";

/// @brief Key to the MoveDownConstantKey.
const auto MoveDownConstantKey = "MoveDown";

/// @brief Key to the MoveLeftConstantKey.
const auto MoveLeftConstantKey = "MoveLeft";

/// @brief Key to the MoveRightConstantKey.
const auto MoveRightConstantKey = "MoveRight";

/// @brief Key to the FireGunConstantKey.
const auto FireGunConstantKey = "FireGun";

/// @brief Key to the AccelerateConstantKey.
const auto AccelerateConstantKey = "Accelerate";

/// @brief Key to the LaunchBombConstantKey.
const auto LaunchBombConstantKey = "LaunchBomb";
} // namespace KeyBindings
