// ============================================================================
//  File        : DialogConstants.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-12
//  Description : Host definition for const dialog lines.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "Assets.h"
#include "DialogLine.h"
#include "UIIcon.h"

#pragma once

/// @brief Constants that can be adjusted throughout the Dialog definitions.
namespace
{
/// @brief Intro dialog - line 1.
constexpr auto DIALOG_INTRO_1 = "Welcome to the front line, pilot!\nThere are numerous enemy vessels detected "
                                "in your A-O.\nProceed with absolute caution.";

/// @brief Intro dialog - line 2.
constexpr auto DIALOG_INTRO_2 = "Do be wary of the asteroid belt.\nTake it slow and conserve fuel.";

/// @brief Intro dialog - line 3.
constexpr auto DIALOG_INTRO_3 = "Dont get hot-headed on us pilot, DO NOT bomb black holes.\nIt can be unpredictable!";
} // namespace

/// @brief Namespace which provides dialog constants and collection definitions for ease of use and abstraction.
namespace DialogConstants
{
/// @brief Sub Namespace which provides the intro dialog.
namespace IntroDialog
{
/// @brief Introductory dialog - line 1.
inline const DialogLine INTRO_LINE_1{
    .text = DIALOG_INTRO_1,
    .speakerName = "Commander",
    .iconTextureKey = SpriteAssets::AstronautSpeakerWhiteKey,
    .iconType = IconType::SpeakerIcon,
    .showTitle = true,
};

/// @brief Introductory dialog - line 2.
inline const DialogLine INTRO_LINE_2{
    .text = DIALOG_INTRO_2,
    .speakerName = "Co-Pilot",
    .iconTextureKey = SpriteAssets::AstronautSpeakerBlueKey,
    .iconType = IconType::SpeakerIcon,
    .showTitle = true,
};

/// @brief Introductory dialog - line 3.
inline const DialogLine INTRO_LINE_3{
    .text = DIALOG_INTRO_3,
    .speakerName = "Wingman",
    .iconTextureKey = SpriteAssets::AstronautSpeakerRedKey,
    .iconType = IconType::SpeakerIcon,
    .showTitle = true,
};

/// @brief Group introductory dialog collection for easy reuse.
inline const std::vector<DialogLine> INTRO_SEQUENCE{
    INTRO_LINE_1,
    INTRO_LINE_2,
    INTRO_LINE_3,
};
} // namespace IntroDialog
} // namespace DialogConstants
