#pragma once

#include <string>
#include <unordered_map>

namespace MainMenuAssets
{
// Keys
constexpr auto MenuSong = "assets/audio/RootMenu.wav";
constexpr auto MenuFont = "MenuFont";
constexpr auto MenuBackground = "MenuBackground";

// future scenes will likely have more than just one element in the map, so enforcing use of map even with one element
// for future consistency.
static const std::unordered_map<std::string, std::string> Textures = {
    {"MenuBackground", "assets/backgrounds/NebulaBackground.png"},
};

static const std::unordered_map<std::string, std::string> Fonts = {
    {"Default.ttf", "assets/fonts/Default.ttf"},
    {"MenuFont", "assets/fonts/Default.ttf"},
};
} // namespace MainMenuAssets
