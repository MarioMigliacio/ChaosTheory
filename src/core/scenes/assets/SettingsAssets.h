#pragma once

#include <string>
#include <unordered_map>

namespace SettingsAssets
{
// Keys
constexpr auto SettingsBackground = "SettingsBackground";
constexpr auto SettingsSound = "SettingsSound";
constexpr auto SettingsFont = "SettingsFont";

// future scenes will likely have more than just one element in the map, so enforcing use of map even with one element
// for future consistency.
static const std::unordered_map<std::string, std::string> Textures = {
    {"SettingsBackground", "assets/backgrounds/DarkStarryBackground.png"},
};

static const std::unordered_map<std::string, std::string> Sounds = {
    {"SettingsSound", "assets/audio/PewPew.wav"},
};

static const std::unordered_map<std::string, std::string> Fonts = {
    {"Default.ttf", "assets/fonts/Default.ttf"},
    {"SettingsFont", "assets/fonts/Default.ttf"},
};
} // namespace SettingsAssets
