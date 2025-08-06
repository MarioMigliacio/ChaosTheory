// ============================================================================
//  File        : Version.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Application version tracability file
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

/// @brief Major Version of Chaos Theory to date.
#define CT_VERSION_MAJOR 1

/// @brief Minor Version of Chaos Theory to date.
#define CT_VERSION_MINOR 5

/// @brief Patch Version of Chaos Theory to date.
#define CT_VERSION_PATCH 1

// Helper macros to convert numbers to strings
#define CT_STRINGIFY(x) #x
#define CT_TOSTRING(x) CT_STRINGIFY(x)

/// @brief String representation for Chaos Theory version.
#define CT_VERSION_STRING                                                                                              \
    CT_TOSTRING(CT_VERSION_MAJOR) "." CT_TOSTRING(CT_VERSION_MINOR) "." CT_TOSTRING(CT_VERSION_PATCH)
