#pragma once

#include <string>
#include <unordered_map>

namespace SplashAssets
{
// Keys
constexpr auto SplashBackground = "SplashBackground";

// future scenes will likely have more than just one element in the map, so enforcing use of map even with one element
// for future consistency.
static const std::unordered_map<std::string, std::string> Textures = {
    {"SplashBackground", "assets/backgrounds/ChaosTheorySplash.png"},
};
} // namespace SplashAssets
