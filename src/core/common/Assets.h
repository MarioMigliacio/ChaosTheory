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
/// @brief Key to the BombAudioKey Audio Asset.
static auto BombAudioKey = "Bomb";

/// @brief Key to the ChatBlipKey Audio Asset.
static auto ChatBlipKey = "ChatBlip";

/// @brief Key to the ExplosionAudioKey Audio Asset.
static auto ExplosionAudioKey = "Explosion";

/// @brief Key to the SplashIntroAudioKey Audio Asset.
static auto SplashIntroAudioKey = "IntroClip";

/// @brief Key to the PingAudioKey Audio Asset.
static auto PingAudioKey = "Ping";

/// @brief Key to the RootMenuAudioKey Audio Asset.
static auto RootMenuAudioKey = "RootMenu";

// @brief Sounds contain a Key and Value pair collection of Audio Assets
static const std::unordered_map<std::string, std::string> Sounds = {
    {"Bomb", "assets/audio/Bomb.wav"},           {"Explosion", "assets/audio/Explosion.wav"},
    {"IntroClip", "assets/audio/IntroClip.wav"}, {"Ping", "assets/audio/Ping.wav"},
    {"RootMenu", "assets/audio/RootMenu.wav"},   {"ChatBlip", "assets/audio/ChatBlip.wav"},
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

/// @brief Exposes relevent assets to the FontAssets namespace.
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
// 1. Enemy Assets
// 2. Icon Assets
// 3. Player Assets
// 4. Projectile Assets
// ============================================================================

/// @brief Exposes relevent assets to the SpriteAssets namespace.
namespace SpriteAssets
{

/// @brief Exposes relevent assets to the EnemyAssets namespace.
namespace EnemyAssets
{
/// @brief Key to the AlienShipSpriteKey Sprite Asset.
static auto AlienShipSpriteKey = "AlienShip";

/// @brief Key to the BasicShipSpriteKey Sprite Asset.
static auto BasicShipSpriteKey = "BasicShip";

/// @brief Key to the BerserkerShipSpriteKey Sprite Asset.
static auto BerserkerShipSpriteKey = "BerserkerShip";

/// @brief Key to the CrusaderShipSpriteKey Sprite Asset.
static auto CrusaderShipSpriteKey = "CrusaderShip";

/// @brief Key to the GruntShipSpriteKey Sprite Asset.
static auto GruntShipSpriteKey = "GruntShip";

/// @brief Key to the InvaderShipSpriteKey Sprite Asset.
static auto InvaderShipSpriteKey = "InvaderShip";

/// @brief Key to the JammerShipSpriteKey Sprite Asset.
static auto JammerShipSpriteKey = "JammerShip";

/// @brief Key to the RammerShipSpriteKey Sprite Asset.
static auto RammerShipSpriteKey = "RammerShip";

/// @brief Key to the ScoutingShipSpriteKey Sprite Asset.
static auto ScoutingShipSpriteKey = "ScoutingShip";

/// @brief Key to the SquidShipSpriteKey Sprite Asset.
static auto SquidShipSpriteKey = "SquidShip";

/// @brief Sprites contain a Key and Value pair collection of sprite assets
static const std::unordered_map<std::string, std::string> Sprites = {
    {"AlienShip", "assets/sprites/enemies/AlienShip.png"},
    {"BasicShip", "assets/sprites/enemies/BasicShip.png"},
    {"BerserkerShip", "assets/sprites/enemies/BerserkerShip.png"},
    {"CrusaderShip", "assets/sprites/enemies/CrusaderShip.png"},
    {"GruntShip", "assets/sprites/enemies/GruntShip.png"},
    {"InvaderShip", "assets/sprites/enemies/InvaderShip.png"},
    {"JammerShip", "assets/sprites/enemies/JammerShip.png"},
    {"RammerShip", "assets/sprites/enemies/RammerShip.png"},
    {"ScoutingShip", "assets/sprites/enemies/ScoutingShip.png"},
    {"SquidShip", "assets/sprites/enemies/SquidShip.png"},
};

} // namespace EnemyAssets

/// @brief Exposes relevent assets to the IconAssets namespace.
namespace IconAssets
{
/// @brief Key to the AtomicIconSpriteKey Sprite Asset.
static auto AtomicIconSpriteKey = "AtomicIcon";

/// @brief Key to the FireRateIconSpriteKey Sprite Asset.
static auto FireRateIconSpriteKey = "FireRateIcon";

/// @brief Key to the GasBoostIconSpriteKey Sprite Asset.
static auto GasBoostIconSpriteKey = "GasBoostIcon";

/// @brief Key to the GasRestoreIconSpriteKey Sprite Asset.
static auto GasRestoreIconSpriteKey = "GasRestoreIcon";

/// @brief Key to the HealthBoostIconSpriteKey Sprite Asset.
static auto HealthBoostIconSpriteKey = "HealthBoostIcon";

/// @brief Key to the HealthRestoreIconSpriteKey Sprite Asset.
static auto HealthRestoreIconSpriteKey = "HealthRestoreIcon";

/// @brief Key to the LifeIconSpriteKey Sprite Asset.
static auto LifeIconSpriteKey = "LifeIcon";

/// @brief Key to the PowerIconSpriteKey Sprite Asset.
static auto PowerIconSpriteKey = "PowerIcon";

/// @brief Key to the UpgradeIconSpriteKey Sprite Asset.
static auto UpgradeIconSpriteKey = "UpgradeIcon";

/// @brief Key to the VelocityIconSpriteKey Sprite Asset.
static auto VelocityIconSpriteKey = "VelocityIcon";

/// @brief Key to the WarpIconSpriteKey Sprite Asset.
static auto WarpIconSpriteKey = "WarpIcon";

/// @brief Sprites contain a Key and Value pair collection of sprite assets
static const std::unordered_map<std::string, std::string> Sprites = {
    {"AtomicIcon", "assets/sprites/icons/AtomicIcon.png"},
    {"FireRateIcon", "assets/sprites/icons/FireRateIcon.png"},
    {"GasBoostIcon", "assets/sprites/icons/GasBoostIcon.png"},
    {"GasRestoreIcon", "assets/sprites/icons/GasRestoreIcon.png"},
    {"HealthBoostIcon", "assets/sprites/icons/HealthBoostIcon.png"},
    {"HealthRestoreIcon", "assets/sprites/icons/HealthRestoreIcon.png"},
    {"LifeIcon", "assets/sprites/icons/LifeIcon.png"},
    {"PowerIcon", "assets/sprites/icons/PowerIcon.png"},
    {"UpgradeIcon", "assets/sprites/icons/UpgradeIcon.png"},
    {"VelocityIcon", "assets/sprites/icons/VelocityIcon.png"},
    {"WarpIcon", "assets/sprites/icons/WarpIcon.png"},
};

} // namespace IconAssets

/// @brief Exposes relevent assets to the PlayerAssets namespace.
namespace PlayerAssets
{
/// @brief Key to the AstronautSpeakerBlackKey Sprite Asset.
static auto AstronautSpeakerBlackKey = "AstronautSpeakerBlack";

/// @brief Key to the AstronautSpeakerBlueKey Sprite Asset.
static auto AstronautSpeakerBlueKey = "AstronautSpeakerBlue";

/// @brief Key to the AstronautSpeakerGoldKey Sprite Asset.
static auto AstronautSpeakerGoldKey = "AstronautSpeakerGold";

/// @brief Key to the AstronautSpeakerGreenKey Sprite Asset.
static auto AstronautSpeakerGreenKey = "AstronautSpeakerGreen";

/// @brief Key to the AstronautSpeakerRedKey Sprite Asset.
static auto AstronautSpeakerRedKey = "AstronautSpeakerRed";

/// @brief Key to the AstronautSpeakerWhiteKey Sprite Asset.
static auto AstronautSpeakerWhiteKey = "AstronautSpeakerWhite";

/// @brief Key to the PlayerShipBlackKey Sprite Asset.
static auto PlayerShipBlackKey = "PlayerShipBlack";

/// @brief Key to the PlayerShipBlueKey Sprite Asset.
static auto PlayerShipBlueKey = "PlayerShipBlue";

/// @brief Key to the PlayerShipGoldKey Sprite Asset.
static auto PlayerShipGoldKey = "PlayerShipGold";

/// @brief Key to the PlayerShipGreenKey Sprite Asset.
static auto PlayerShipGreenKey = "PlayerShipGreen";

/// @brief Key to the PlayerShipRedKey Sprite Asset.
static auto PlayerShipRedKey = "PlayerShipRed";

/// @brief Key to the PlayerShipWhiteKey Sprite Asset.
static auto PlayerShipWhiteKey = "PlayerShipWhite";

/// @brief Sprites contain a Key and Value pair collection of sprite assets
static const std::unordered_map<std::string, std::string> Sprites = {
    {"AstronautSpeakerBlack", "assets/sprites/players/AstronautSpeakerBlack.png"},
    {"AstronautSpeakerBlue", "assets/sprites/players/AstronautSpeakerBlue.png"},
    {"AstronautSpeakerGold", "assets/sprites/players/AstronautSpeakerGold.png"},
    {"AstronautSpeakerGreen", "assets/sprites/players/AstronautSpeakerGreen.png"},
    {"AstronautSpeakerRed", "assets/sprites/players/AstronautSpeakerRed.png"},
    {"AstronautSpeakerWhite", "assets/sprites/players/AstronautSpeakerWhite.png"},
    {"PlayerShipBlack", "assets/sprites/players/PlayerShipBlack.png"},
    {"PlayerShipBlue", "assets/sprites/players/PlayerShipBlue.png"},
    {"PlayerShipGold", "assets/sprites/players/PlayerShipGold.png"},
    {"PlayerShipGreen", "assets/sprites/players/PlayerShipGreen.png"},
    {"PlayerShipRed", "assets/sprites/players/PlayerShipRed.png"},
    {"PlayerShipWhite", "assets/sprites/players/PlayerShipWhite.png"},
};

} // namespace PlayerAssets

/// @brief Exposes relevent assets to the ProjectileAssets namespace.
namespace ProjectileAssets
{
/// @brief Key to the BombBlastFinalSpriteKey Sprite Asset.
static auto BombBlastFinalSpriteKey = "BombBlastFinal";

/// @brief Key to the BombBlastOriginalSpriteKey Sprite Asset.
static auto BombBlastOriginalSpriteKey = "BombBlastOriginal";

/// @brief Key to the BasicBulletSpriteKey Sprite Asset.
static auto BasicBulletSpriteKey = "BasicBullet";

/// @brief Key to the HomingRocketSpriteKey Sprite Asset.
static auto HomingRocketSpriteKey = "HomingRocket";

/// @brief Sprites contain a Key and Value pair collection of sprite assets
static const std::unordered_map<std::string, std::string> Sprites = {
    {"BombBlastFinal", "assets/sprites/projectiles/BombBlastFinal.png"},
    {"BombBlastOriginal", "assets/sprites/projectiles/BombBlastOriginal.png"},
    {"BasicBullet", "assets/sprites/projectiles/BasicBullet.png"},
    {"HomingRocket", "assets/sprites/projectiles/HomingRocket.png"},
};

} // namespace ProjectileAssets

} // namespace SpriteAssets

// ============================================================================
// UI Assets:
// ============================================================================

/// @brief Exposes relevent assets to the UIAssets namespace.
namespace UIAssets
{
/// @brief Key to the UIArrowLeft Texture Asset.
static auto UIArrowLeftTextureKey = "UIArrowLeft";

/// @brief Key to the UIArrowDown Texture Asset.
static auto UIArrowDownTextureKey = "UIArrowDown";

/// @brief Key to the UIArrowRight Texture Asset.
static auto UIArrowRightTextureKey = "UIArrowRight";

/// @brief Key to the UIArrowUp Texture Asset.
static auto UIArrowUpTextureKey = "UIArrowUp";

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
    {"UIArrowLeft", "assets/ui/arrow/UIArrowLeft.png"},
    {"UIArrowDown", "assets/ui/arrow/UIArrowDown.png"},
    {"UIArrowRight", "assets/ui/arrow/UIArrowRight.png"},
    {"UIArrowUp", "assets/ui/arrow/UIArrowUp.png"},
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

/// @brief Exposes relevent assets to the SettingsAssets namespace.
namespace SettingsAssets
{
/// @brief Key to the SettingsSound Asset.
constexpr auto SettingsSound = "SettingsSound";

/// @brief Backgrounds contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Backgrounds = {
    {"PlainStarBackground", "assets/backgrounds/PlainStarBackground.png"},
    {"GasPattern3", "assets/backgrounds/GasPattern3.png"},
};

/// @brief Sounds contain a Key and Value pair collection of audio sfx assets
static const std::unordered_map<std::string, std::string> Sounds = {
    {"SettingsSound", "assets/audio/Ping.wav"},
};

/// @brief UI contain a Key and Value pair collection of ui assets
static const std::unordered_map<std::string, std::string> UI = {
    {"UIArrowLeft", "assets/ui/arrow/UIArrowLeft.png"},
    {"UIArrowRight", "assets/ui/arrow/UIArrowRight.png"},
    {"GreenButtonIdle", "assets/ui/button/green/GreenButtonIdle.png"},
    {"GreenButtonHover", "assets/ui/button/green/GreenButtonHover.png"},
    {"RedButtonIdle", "assets/ui/button/red/RedButtonIdle.png"},
    {"RedButtonHover", "assets/ui/button/red/RedButtonHover.png"},
};

} // namespace SettingsAssets

// ============================================================================
// MainMenuScene Preconfigurable Assets for quick loading
// ============================================================================

/// @brief Exposes relevent assets to the MainMenuAssets namespace.
namespace MainMenuAssets
{
/// @brief Path for the MenuSong.
constexpr auto MenuSong = "assets/audio/RootMenu.wav";

/// @brief Backgrounds contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Backgrounds = {
    {"PlainStarBackground", "assets/backgrounds/PlainStarBackground.png"},
    {"GasPattern1", "assets/backgrounds/GasPattern1.png"},
    {"GasPattern2", "assets/backgrounds/GasPattern2.png"},
};

/// @brief Sprites contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Sprites = {
    {"PlayerShipBlack", "assets/sprites/players/PlayerShipBlack.png"},
    {"PlayerShipBlue", "assets/sprites/players/PlayerShipBlue.png"},
    {"PlayerShipGold", "assets/sprites/players/PlayerShipGold.png"},
    {"PlayerShipGreen", "assets/sprites/players/PlayerShipGreen.png"},
    {"PlayerShipRed", "assets/sprites/players/PlayerShipRed.png"},
    {"PlayerShipWhite", "assets/sprites/players/PlayerShipWhite.png"},
};

/// @brief UI contain a Key and Value pair collection of ui assets
static const std::unordered_map<std::string, std::string> UI = {
    {"BlueButtonIdle", "assets/ui/button/blue/BlueButtonIdle.png"},
    {"BlueButtonHover", "assets/ui/button/blue/BlueButtonHover.png"},
};

} // namespace MainMenuAssets

// ============================================================================
// SplashScene Preconfigurable Assets for quick loading
// ============================================================================

/// @brief Exposes relevent assets to the SplashAssets namespace.
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

} // namespace SplashAssets

// ============================================================================
// IntroductionScene Preconfigurable Assets for quick loading
// ============================================================================

/// @brief Exposes relevent assets to the SandBoxAssets namespace.
namespace IntroductionAssets
{
/// @brief Backgrounds contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Backgrounds = {
    {"PlainStarBackground", "assets/backgrounds/PlainStarBackground.png"},
    {"GasPattern1", "assets/backgrounds/GasPattern1.png"},
    {"GasPattern2", "assets/backgrounds/GasPattern2.png"},
};

/// @brief Sprites contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Sprites = {
    {"AstronautSpeakerBlack", "assets/sprites/players/AstronautSpeakerBlack.png"},
    {"AstronautSpeakerBlue", "assets/sprites/players/AstronautSpeakerBlue.png"},
    {"AstronautSpeakerGold", "assets/sprites/players/AstronautSpeakerGold.png"},
    {"AstronautSpeakerGreen", "assets/sprites/players/AstronautSpeakerGreen.png"},
    {"AstronautSpeakerRed", "assets/sprites/players/AstronautSpeakerRed.png"},
    {"AstronautSpeakerWhite", "assets/sprites/players/AstronautSpeakerWhite.png"},
    {"PlayerShipBlack", "assets/sprites/players/PlayerShipBlack.png"},
    {"PlayerShipBlue", "assets/sprites/players/PlayerShipBlue.png"},
    {"PlayerShipGold", "assets/sprites/players/PlayerShipGold.png"},
    {"PlayerShipGreen", "assets/sprites/players/PlayerShipGreen.png"},
    {"PlayerShipRed", "assets/sprites/players/PlayerShipRed.png"},
    {"PlayerShipWhite", "assets/sprites/players/PlayerShipWhite.png"},
    {"UIArrowDown", "assets/ui/arrow/UIArrowDown.png"},
};

/// @brief Sounds contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Sounds = {
    {"ChatBlip", "assets/audio/ChatBlip.wav"},
};

} // namespace IntroductionAssets

// ============================================================================
// SandBoxScene Preconfigurable Assets for quick loading
// ============================================================================

/// @brief Exposes relevent assets to the SandBoxAssets namespace.
namespace SandBoxAssets
{
/// @brief Path for the GameTrack.
constexpr auto GameTrack = "assets/audio/RootMenu.wav";

/// @brief Backgrounds contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Backgrounds = {
    {"PlainStarBackground", "assets/backgrounds/PlainStarBackground.png"},
    {"GasPattern1", "assets/backgrounds/GasPattern1.png"},
    {"GasPattern2", "assets/backgrounds/GasPattern2.png"},
};

/// @brief Sprites contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Sprites = {
    {"BasicBullet", "assets/sprites/projectiles/BasicBullet.png"},
    {"AlienShip", "assets/sprites/enemies/AlienShip.png"},
    {"AstronautSpeakerBlack", "assets/sprites/players/AstronautSpeakerBlack.png"},
    {"AstronautSpeakerBlue", "assets/sprites/players/AstronautSpeakerBlue.png"},
    {"AstronautSpeakerGold", "assets/sprites/players/AstronautSpeakerGold.png"},
    {"AstronautSpeakerGreen", "assets/sprites/players/AstronautSpeakerGreen.png"},
    {"AstronautSpeakerRed", "assets/sprites/players/AstronautSpeakerRed.png"},
    {"AstronautSpeakerWhite", "assets/sprites/players/AstronautSpeakerWhite.png"},
    {"BasicShip", "assets/sprites/enemies/BasicShip.png"},
    {"AtomicIcon", "assets/sprites/icons/AtomicIcon.png"},
    {"FireRateIcon", "assets/sprites/icons/FireRateIcon.png"},
    {"GasBoostIcon", "assets/sprites/icons/GasBoostIcon.png"},
    {"GasRestoreIcon", "assets/sprites/icons/GasRestoreIcon.png"},
    {"HealthBoostIcon", "assets/sprites/icons/HealthBoostIcon.png"},
    {"HealthRestoreIcon", "assets/sprites/icons/HealthRestoreIcon.png"},
    {"LifeIcon", "assets/sprites/icons/LifeIcon.png"},
    {"PowerIcon", "assets/sprites/icons/PowerIcon.png"},
    {"UpgradeIcon", "assets/sprites/icons/UpgradeIcon.png"},
    {"VelocityIcon", "assets/sprites/icons/VelocityIcon.png"},
    {"WarpIcon", "assets/sprites/icons/WarpIcon.png"},
    {"UIArrowDown", "assets/ui/arrow/UIArrowDown.png"},
};

/// @brief Sounds contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Sounds = {
    {"ChatBlip", "assets/audio/ChatBlip.wav"},
};

} // namespace SandBoxAssets

// ============================================================================
// GameScene Preconfigurable Assets for quick loading
// ============================================================================

/// @brief Exposes relevent assets to the GameAssets namespace.
namespace GameAssets
{
/// @brief Textures contain a Key and Value pair collection of image assets
static const std::unordered_map<std::string, std::string> Textures = {
    {"PlainStarBackground", "assets/backgrounds/PlainStarBackground.png"},
    {"GasPattern1", "assets/backgrounds/GasPattern1.png"},
    {"GasPattern2", "assets/backgrounds/GasPattern2.png"},
};

} // namespace GameAssets

// ============================================================================
// PauseScene Preconfigurable Assets for quick loading
// ============================================================================

/// @brief Exposes relevent assets to the PauseAssets namespace.
namespace PauseAssets
{
/// @brief UI contain a Key and Value pair collection of ui assets
static const std::unordered_map<std::string, std::string> UI = {
    {"BlueButtonIdle", "assets/ui/button/blue/BlueButtonIdle.png"},
    {"BlueButtonHover", "assets/ui/button/blue/BlueButtonHover.png"},
    {"GreenButtonIdle", "assets/ui/button/green/GreenButtonIdle.png"},
    {"GreenButtonHover", "assets/ui/button/green/GreenButtonHover.png"},
    {"RedButtonIdle", "assets/ui/button/red/RedButtonIdle.png"},
    {"RedButtonHover", "assets/ui/button/red/RedButtonHover.png"},
};

} // namespace PauseAssets
