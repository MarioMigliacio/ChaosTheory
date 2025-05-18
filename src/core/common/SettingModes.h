// ============================================================================
//  File        : SettingModes.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-17
//  Description : Extraction of setting modes from the Settings object.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <string>

/// @brief Simple enumeration type for Window Resolution settings.
enum class ResolutionSetting
{
    /// @brief Common resolution: {1280, 720}.
    Res720p,

    /// @brief Common resolution: {1920, 1080}.
    Res1080p,

    /// @brief Common setting, use full screen without title bar.
    Fullscreen
};

/// @brief Simple enumeration type for Game difficulty mode settings.
enum class GameDifficultySetting
{
    /// @brief A simple gameplay mode to enjoy and complete easily.
    Easy,

    /// @brief A gameplay mode where challenge and reward are balanced.
    Normal,

    /// @brief A gameplay mode designed to be highly difficult.
    Hard
};

/// @brief Utility function to convert ResolutionSetting to string
/// @param setting which ResolutionSetting enumeration.
/// @return readyonly string identifying the ResolutionSetting enum.
inline std::string ResolutionSettingToString(ResolutionSetting setting)
{
    switch (setting)
    {
        case ResolutionSetting::Res720p:
        default: // fallback
            return "720p";
        case ResolutionSetting::Res1080p:
            return "1080p";
        case ResolutionSetting::Fullscreen:
            return "Fullscreen";
    }
}

/// @brief Utility function to convert GameDifficultySetting to string
/// @param setting which GameDifficultySetting enumeration.
/// @return readyonly string identifying the GameDifficultySetting enum.
inline std::string GameDifficultySettingToString(GameDifficultySetting setting)
{
    switch (setting)
    {
        case GameDifficultySetting::Easy:
            return "Easy";
        case GameDifficultySetting::Normal:
        default: // fallback
            return "Normal";
        case GameDifficultySetting::Hard:
            return "Hard";
    }
}

/// @brief Returns a ResultionSetting enumeration from a string.
/// @param str input ResolutionSetting as a string representation.
/// @return an Enumeration form of ResolutionSetting.
inline ResolutionSetting FromStringToResolution(const std::string &str)
{
    if (str == "720p")
    {
        return ResolutionSetting::Res720p;
    }

    if (str == "1080p")
    {
        return ResolutionSetting::Res1080p;
    }

    if (str == "Fullscreen")
    {
        return ResolutionSetting::Fullscreen;
    }

    return ResolutionSetting::Res720p; // default fallback
}

/// @brief Returns a GameDifficultySetting enumeration from a string.
/// @param str input GameDifficultySetting as a string representation.
/// @return an Enumeration form of GameDifficultySetting.
inline GameDifficultySetting FromStringToGameDifficulty(const std::string &str)
{
    if (str == "Easy")
    {
        return GameDifficultySetting::Easy;
    }

    if (str == "Normal")
    {
        return GameDifficultySetting::Normal;
    }

    if (str == "Hard")
    {
        return GameDifficultySetting::Hard;
    }

    return GameDifficultySetting::Normal; // default fallback
}
