// ============================================================================
//  File        : Assets.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-06-01
//  Description : Hosts the namespaces for game Assets.
//                - UI Assets
//                - Audio Assets
//                - Background Assets
//                - Font Assets
//                - Sprite Assets
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <string>
#include <unordered_map>

// ============================================================================
// Audio Assets:
// ============================================================================

/// @brief Exposes relevent assets to the AudioAssets namespace.
namespace AudioAssets
{
/// @brief Key to the BombAudioKey Texture Asset.
static auto BombAudioKey = "Bomb";

/// @brief Key to the ExplosionAudioKey Texture Asset.
static auto ExplosionAudioKey = "Explosion";

/// @brief Key to the SplashIntroAudioKey Texture Asset.
static auto SplashIntroAudioKey = "IntroClip";

/// @brief Key to the PingAudioKey Texture Asset.
static auto PingAudioKey = "Ping";

/// @brief Key to the RootMenuAudioKey Texture Asset.
static auto RootMenuAudioKey = "RootMenu";

// @brief Sounds contain a Key and Value pair collection of Audio Assets
static const std::unordered_map<std::string, std::string> Sounds = {
    {"Bomb", "assets/audio/Bomb.wav"},           {"Explosion", "assets/audio/Explosion.wav"},
    {"IntroClip", "assets/audio/IntroClip.wav"}, {"Ping", "assets/audio/Ping.wav"},
    {"RootMenu", "assets/audio/RootMenu.wav"},
};
} // namespace AudioAssets

// ============================================================================
// Background Assets:
// ============================================================================

/// @brief Exposes relevent assets to the BackgroundAssets namespace.
namespace BackgroundAssets
{
/// @brief Key to the BrightNebulaBackgroundKey Background Asset.
static auto BrightNebulaBackgroundKey = "BrightNebula";

/// @brief Key to the DarkNebulaBackgroundKey Background Asset.
static auto DarkNebulaBackgroundKey = "DarkNebula";

/// @brief Key to the ChaosTheorySplashBackgroundKey Background Asset.
static auto ChaosTheorySplashBackgroundKey = "ChaosTheorySplash";

/// @brief Key to the GasPattern1BackgroundKey Background Asset.
static auto GasPattern1BackgroundKey = "GasPattern1";

/// @brief Key to the GasPattern2BackgroundKey Background Asset.
static auto GasPattern2BackgroundKey = "GasPattern2";

/// @brief Key to the GasPattern3BackgroundKey Background Asset.
static auto GasPattern3BackgroundKey = "GasPattern3";

/// @brief Key to the LoneGalaxyBackgroundKey Background Asset.
static auto LoneGalaxyBackgroundKey = "LoneGalaxy";

/// @brief Key to the PlainStarBackgroundKey Background Asset.
static auto PlainStarBackgroundKey = "PlainStarBackground";

// @brief Textures contain a Key and Value pair collection of Image Assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"BrightNebula", "assets/backgrounds/BrightNebula.png"},
    {"DarkNebula", "assets/backgrounds/DarkNebula.png"},
    {"ChaosTheorySplash", "assets/backgrounds/ChaosTheorySplash.png"},
    {"GasPattern1", "assets/backgrounds/GasPattern1.png"},
    {"GasPattern2", "assets/backgrounds/GasPattern2.png"},
    {"GasPattern3", "assets/backgrounds/GasPattern3.png"},
    {"LoneGalaxy", "assets/backgrounds/LoneGalaxy.png"},
    {"PlainStarBackground", "assets/backgrounds/PlainStarBackground.png"},
};
} // namespace BackgroundAssets

// ============================================================================
// Font Assets:
// ============================================================================

namespace FontAssets
{
/// @brief Key to the DefaultFontKey Font Asset.
static const auto DefaultFontKey = "Default";

/// @brief Fonts contain a Key and Value pair collection of Font assets.
static const std::unordered_map<std::string, std::string> Fonts = {
    {"Default", "assets/fonts/Default.ttf"},
};

} // namespace FontAssets

// ============================================================================
// Sprite Assets:
// ============================================================================

namespace SpriteAssets
{
/// @brief Key to the PlayerShipSpriteKey Sprite Asset.
static auto PlayerShipSpriteKey = "PlayerShip";

/// @brief Key to the AlienShipSpriteKey Sprite Asset.
static auto AlienShipSpriteKey = "AlienShip";

/// @brief Key to the BasicShipSpriteKey Sprite Asset.
static auto BasicShipSpriteKey = "BasicShip";

/// @brief Key to the BombBlastOriginalSpriteKey Sprite Asset.
static auto BombBlastOriginalSpriteKey = "BombBlastOriginal";

/// @brief Key to the BombBlastFinalSpriteKey Sprite Asset.
static auto BombBlastFinalSpriteKey = "BombBlastFinal";

/// @brief Key to the BombTokenSpriteKey Sprite Asset.
static auto BombTokenSpriteKey = "BombToken";

/// @brief Key to the DamageTokenSpriteKey Sprite Asset.
static auto DamageTokenSpriteKey = "DamageToken";

/// @brief Key to the FireRateTokenSpriteKey Sprite Asset.
static auto FireRateTokenSpriteKey = "FireRateToken";

/// @brief Key to the FreeScoreTokenSpriteKey Sprite Asset.
static auto FreeScoreTokenSpriteKey = "FreeScoreToken";

/// @brief Key to the PatternTokenSpriteKey Sprite Asset.
static auto PatternTokenSpriteKey = "PatternToken";

/// @brief Key to the LifeTokenSpriteKey Sprite Asset.
static auto LifeTokenSpriteKey = "LifeToken";

/// @brief Key to the BulletBlueSpriteKey Sprite Asset.
static auto BulletBlueSpriteKey = "BulletBlue";

/// @brief Key to the BulletGreenSpriteKey Sprite Asset.
static auto BulletGreenSpriteKey = "BulletGreen";

/// @brief Key to the BulletRedSpriteKey Sprite Asset.
static auto BulletRedSpriteKey = "BulletRed";

/// @brief Key to the HomingRocketSpriteKey Sprite Asset.
static auto HomingRocketSpriteKey = "HomingRocket";

/// @brief Key to the LazerBlueSpriteKey Sprite Asset.
static auto LazerBlueSpriteKey = "LazerBlue";

/// @brief Key to the LazerGreenSpriteKey Sprite Asset.
static auto LazerGreenSpriteKey = "LazerGreen";

/// @brief Key to the LazerRedSpriteKey Sprite Asset.
static auto LazerRedSpriteKey = "LazerRed";

/// @brief Key to the WideLazerBlueSpriteKey Sprite Asset.
static auto WideLazerBlueSpriteKey = "WideLazerBlue";

/// @brief Key to the WideLazerGreenSpriteKey Sprite Asset.
static auto WideLazerGreenSpriteKey = "WideLazerGreen";

/// @brief Key to the WideLazerRedSpriteKey Sprite Asset.
static auto WideLazerRedSpriteKey = "WideLazerRed";

/// @brief Sprites contain a Key and Value pair collection of sprite assets
static const std::unordered_map<std::string, std::string> Sprites = {
    {"PlayerShip", "assets/sprites/PlayerShip.png"},
    {"AlienShip", "assets/sprites/AlienShip.png"},
    {"BasicShip", "assets/sprites/BasicShip.png"},
    {"BombBlastOriginal", "assets/sprites/BombBlastOriginal.png"},
    {"BombBlastFinal", "assets/sprites/BombBlastFinal.png"},
    {"BombToken", "assets/sprites/BombToken.png"},
    {"DamageToken", "assets/sprites/DamageToken.png"},
    {"FireRateToken", "assets/sprites/FireRateToken.png"},
    {"FreeScoreToken", "assets/sprites/FreeScoreToken.png"},
    {"PatternToken", "assets/sprites/PatternToken.png"},
    {"LifeToken", "assets/sprites/LifeToken.png"},
    {"BulletBlue", "assets/sprites/BulletBlue.png"},
    {"BulletGreen", "assets/sprites/BulletGreen.png"},
    {"BulletRed", "assets/sprites/BulletRed.png"},
    {"HomingRocket", "assets/sprites/HomingRocket.png"},
    {"LazerBlue", "assets/sprites/LazerBlue.png"},
    {"LazerGreen", "assets/sprites/LazerGreen.png"},
    {"LazerRed", "assets/sprites/LazerRed.png"},
    {"WideLazerBlue", "assets/sprites/WideLazerBlue.png"},
    {"WideLazerGreen", "assets/sprites/WideLazerGreen.png"},
    {"WideLazerRed", "assets/sprites/WideLazerRed.png"},
};
} // namespace SpriteAssets

// ============================================================================
// UI Assets:
// ============================================================================

/// @brief Exposes relevent assets to the UIAssets namespace.
namespace UIAssets
{
/// @brief Key to the UIArrow Texture Asset.
static auto UIArrowTextureKey = "UIArrow";

/// @brief Key to the UISkinnableButton: Blue-Idle Texture Asset.
static auto UISkinButtonBlueIdleKey = "BlueButtonIdle";

/// @brief Key to the UISkinnableButton: Blue-Hover Texture Asset.
static auto UISkinButtonBlueHoverKey = "BlueButtonHover";

/// @brief Key to the UISkinnableButton: Green-Idle Texture Asset.
static auto UISkinButtonGreenIdleKey = "GreenButtonIdle";

/// @brief Key to the UISkinnableButton: Green-Hover Texture Asset.
static auto UISkinButtonGreenHoverKey = "GreenButtonHover";

/// @brief Key to the UISkinnableButton: Red-Idle Texture Asset.
static auto UISkinButtonRedIdleKey = "RedButtonIdle";

/// @brief Key to the UISkinnableButton: Red-Hover Texture Asset.
static auto UISkinButtonRedHoverKey = "RedButtonHover";

/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"UIArrow", "assets/ui/arrow/UIArrow.png"},
    {"BlueButtonIdle", "assets/ui/button/blue/BlueButtonIdle.png"},
    {"BlueButtonHover", "assets/ui/button/blue/BlueButtonHover.png"},
    {"GreenButtonIdle", "assets/ui/button/green/GreenButtonIdle.png"},
    {"GreenButtonHover", "assets/ui/button/green/GreenButtonHover.png"},
    {"RedButtonIdle", "assets/ui/button/red/RedButtonIdle.png"},
    {"RedButtonHover", "assets/ui/button/red/RedButtonHover.png"},
};
} // namespace UIAssets

// ============================================================================
// SettingsScene Preconfigurable Assets for quick loading
// ============================================================================

/// @brief Exposes Textures, Fonts, and Audio assets to the SettingsAssets namespace.
namespace SettingsAssets
{
/// @brief Key to the SettingsSound Asset.
constexpr auto SettingsSound = "SettingsSound";

/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"PlainStarBackground", "assets/backgrounds/PlainStarBackground.png"},
    {"GasPattern3", "assets/backgrounds/GasPattern3.png"},
};

/// @brief Sounds contain a Key and Value pair collection of audio sfx assets
static const std::unordered_map<std::string, std::string> Sounds = {
    {"SettingsSound", "assets/audio/Ping.wav"},
};

/// @brief Fonts contain a Key and Value pair collection of font assets
static const std::unordered_map<std::string, std::string> Fonts = {
    {"Default", "assets/fonts/Default.ttf"},
    {"SettingsFont", "assets/fonts/Default.ttf"},
};
} // namespace SettingsAssets

// ============================================================================
// MainMenuScene Preconfigurable Assets for quick loading
// ============================================================================

/// @brief Exposes Textures, Fonts, and Audio assets to the MainMenuAssets namespace.
namespace MainMenuAssets
{
/// @brief Path for the MenuSong.
constexpr auto MenuSong = "assets/audio/RootMenu.wav";

/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"PlainStarBackground", "assets/backgrounds/PlainStarBackground.png"},
    {"GasPattern1", "assets/backgrounds/GasPattern1.png"},
    {"GasPattern2", "assets/backgrounds/GasPattern2.png"},
    {"PlayerShip", "assets/sprites/PlayerShip.png"},
};

/// @brief Fonts contain a Key and Value pair collection of font assets
static const std::unordered_map<std::string, std::string> Fonts = {
    {"Default", "assets/fonts/Default.ttf"},
    {"MenuFont", "assets/fonts/Default.ttf"},
};
} // namespace MainMenuAssets

// ============================================================================
// SplashScene Preconfigurable Assets for quick loading
// ============================================================================

/// @brief Exposes Textures, Fonts, and Audio assets to the SplashAssets namespace.
namespace SplashAssets
{
/// @brief Key to the Splash intro sound asset.
static auto SplashIntroSound = "assets/audio/IntroClip.wav";

/// @brief Key to the Splash Background asset.
static auto SplashBackground = "SplashBackground";

/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"SplashBackground", "assets/backgrounds/ChaosTheorySplash.png"},
};

/// @brief Fonts contain a Key and Value pair collection of font assets
static const std::unordered_map<std::string, std::string> Fonts = {
    {"Default", "assets/fonts/Default.ttf"},
    {"SplashFont", "assets/fonts/Default.ttf"},
};
} // namespace SplashAssets

// ============================================================================
// SandBoxScene Preconfigurable Assets for quick loading
// ============================================================================

/// @brief Exposes Textures, Fonts, and Audio assets to the GameAssets namespace.
namespace SandBoxAssets
{
/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"PlainStarBackground", "assets/backgrounds/PlainStarBackground.png"},
    {"GasPattern1", "assets/backgrounds/GasPattern1.png"},
    {"GasPattern2", "assets/backgrounds/GasPattern2.png"},
};

/// @brief Fonts contain a Key and Value pair collection of font assets
static const std::unordered_map<std::string, std::string> Fonts = {
    {"Default", "assets/fonts/Default.ttf"},
    {"GameFont", "assets/fonts/Default.ttf"},
};
} // namespace SandBoxAssets

// ============================================================================
// GameScene Preconfigurable Assets for quick loading
// ============================================================================

/// @brief Exposes Textures, Fonts, and Audio assets to the GameAssets namespace.
namespace GameAssets
{
/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"PlainStarBackground", "assets/backgrounds/PlainStarBackground.png"},
    {"GasPattern1", "assets/backgrounds/GasPattern1.png"},
    {"GasPattern2", "assets/backgrounds/GasPattern2.png"},
};

/// @brief Fonts contain a Key and Value pair collection of font assets
static const std::unordered_map<std::string, std::string> Fonts = {
    {"Default", "assets/fonts/Default.ttf"},
    {"GameFont", "assets/fonts/Default.ttf"},
};
} // namespace GameAssets
