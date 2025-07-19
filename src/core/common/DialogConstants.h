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

/// @brief Namespace which provides dialog constants and collection definitions for ease of use and abstraction.
namespace DialogConstants
{
/// @brief Sub Namespace which provides the intro dialog.
namespace IntroDialog
{
/// @brief Intro dialog - line 1.
constexpr auto DIALOG_INTRO_1 = "Welcome to the front line, pilot!\nThere are numerous enemy vessels detected "
                                "in this sector.\nProceed with absolute caution.";

/// @brief Intro dialog - line 2.
constexpr auto DIALOG_INTRO_2 = "Do be wary of the asteroid belt.\nTake it slow and conserve fuel.\nDestroy enemy "
                                "ships and manage your fuel gauge carefully!";

/// @brief Intro dialog - line 3.
constexpr auto DIALOG_INTRO_3 = "Dont get hot-headed on us pilot. \nDO NOT bomb black holes, it can be "
                                "unpredictable!\nEnemy fleets can be destroyed in seconds with carefully placed bombs!";

/// @brief Intro dialog - line 4.
constexpr auto DIALOG_INTRO_4 =
    "This sector is ripe with Chaos Energy, Pilot! \nWith the amount estimated, we could "
    "power 10 fleets for 100 years!\nAnd that energy is more powerful than any weaponry we can produce!..";

/// @brief Intro dialog - line 5.
constexpr auto DIALOG_INTRO_5 =
    "Don't be going overboard Scion. \nWe only harvest what Chaos we need for the mission.\nStay Focused!";

/// @brief Intro dialog - line 6.
constexpr auto DIALOG_INTRO_6 = "...Right, Commander! Sorry Sir.";

/// @brief Intro dialog - line 7.
constexpr auto DIALOG_INTRO_7 =
    "Good luck pilot! We'll meet at the rendezvous point!\nTry to find some useful components for your ship out there!";

/// @brief Introductory dialog - line 1.
inline const DialogLine INTRO_LINE_1{
    .text = DIALOG_INTRO_1,
    .speakerName = "Commander",
    .iconTextureKey = SpriteAssets::PlayerAssets::AstronautSpeakerGoldKey,
    .iconType = IconType::SpeakerIcon,
    .showTitle = true,
};

/// @brief Introductory dialog - line 2.
inline const DialogLine INTRO_LINE_2{
    .text = DIALOG_INTRO_2,
    .speakerName = "Prometheus",
    .iconTextureKey = SpriteAssets::PlayerAssets::AstronautSpeakerBlueKey,
    .iconType = IconType::SpeakerIcon,
    .showTitle = true,
};

/// @brief Introductory dialog - line 3.
inline const DialogLine INTRO_LINE_3{
    .text = DIALOG_INTRO_3,
    .speakerName = "Juggernaut",
    .iconTextureKey = SpriteAssets::PlayerAssets::AstronautSpeakerRedKey,
    .iconType = IconType::SpeakerIcon,
    .showTitle = true,
};

/// @brief Introductory dialog - line 4.
inline const DialogLine INTRO_LINE_4{
    .text = DIALOG_INTRO_4,
    .speakerName = "Scion",
    .iconTextureKey = SpriteAssets::PlayerAssets::AstronautSpeakerBlackKey,
    .iconType = IconType::SpeakerIcon,
    .showTitle = true,
};

/// @brief Introductory dialog - line 5.
inline const DialogLine INTRO_LINE_5{
    .text = DIALOG_INTRO_5,
    .speakerName = "Commander",
    .iconTextureKey = SpriteAssets::PlayerAssets::AstronautSpeakerGoldKey,
    .iconType = IconType::SpeakerIcon,
    .showTitle = true,
};

/// @brief Introductory dialog - line 6.
inline const DialogLine INTRO_LINE_6{
    .text = DIALOG_INTRO_6,
    .speakerName = "Scion",
    .iconTextureKey = SpriteAssets::PlayerAssets::AstronautSpeakerBlackKey,
    .iconType = IconType::SpeakerIcon,
    .showTitle = true,
};

/// @brief Introductory dialog - line 7.
inline const DialogLine INTRO_LINE_7{
    .text = DIALOG_INTRO_7,
    .speakerName = "Obelisk",
    .iconTextureKey = SpriteAssets::PlayerAssets::AstronautSpeakerGreenKey,
    .iconType = IconType::SpeakerIcon,
    .showTitle = true,
};

/// @brief Group introductory dialog collection for easy reuse.
inline const std::vector<DialogLine> INTRO_SEQUENCE{
    INTRO_LINE_1, INTRO_LINE_2, INTRO_LINE_3, INTRO_LINE_4, INTRO_LINE_5, INTRO_LINE_6, INTRO_LINE_7,
};
} // namespace IntroDialog
} // namespace DialogConstants
