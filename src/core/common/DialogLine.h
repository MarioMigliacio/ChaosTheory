// ============================================================================
//  File        : DialogLine.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-12
//  Description : Host definition for helper structs towards a dialog flow
//                using the UIChatBox.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIIcon.h"
#include <string>

/// @brief Represents a single line of dialogue in a sequence.
/// @param text string text content.
/// @param speakerName string speaker name (for title) [default empty string].
/// @param iconTextureKey string speaker icon texture [default empty].
/// @param iconType IconType type of icon [default None].
/// @param showTitle bool Whether to show title [default false].
struct DialogLine
{
    std::string text;
    std::string speakerName = std::string();
    std::string iconTextureKey = std::string();
    IconType iconType = IconType::None;
    bool showTitle = false;
};
