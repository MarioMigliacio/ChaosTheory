// ============================================================================
//  File        : ShipFactory.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-17
//  Description : Factory for creating ship instances.

//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "ShipFactory.h"
#include "AlienShip.h"
#include "AssetManager.h"
#include "Assets.h"
#include "BasicShip.h"
#include "BerserkerShip.h"
#include "CrusaderShip.h"
#include "GruntShip.h"
#include "InvaderShip.h"
#include "Macros.h"
#include "PlayerShip.h"
#include "ResolutionScaleManager.h"

/// @brief Get the current Instance for this ShipFactory singleton.
/// @return reference to existing ShipFactory interface.
ShipFactory &ShipFactory::Instance()
{
    static ShipFactory instance;
    return instance;
}

/// @brief Creates a PlayerShip, scaled appropriately with window resolution.
/// @param pos Starting to position to emplalce ship at.
/// @return Safe pointer to a PlayerShip, compatible with the BaseShip base class.
std::shared_ptr<BaseShip> ShipFactory::CreatePlayerShip(const sf::Vector2f &pos)
{
    auto ship = std::make_shared<PlayerShip>();
    ship->SetPosition(pos);

    // Setup resolution-based scaling
    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::PlayerAssets::PlayerShipWhiteKey);

    if (tex)
    {
        // A little redundant to getSize.x and getSize.y because PlayerShip is a square. But explicit.
        float textureWidth = static_cast<float>(tex->getSize().x);
        float textureHeight = static_cast<float>(tex->getSize().y);
        float scaledWidth = ResolutionScaleManager::Instance().ScaleX(textureWidth);
        float scaledHeight = ResolutionScaleManager::Instance().ScaleY(textureHeight);
        float scaleFactorX = scaledWidth / textureWidth;
        float scaleFactorY = scaledHeight / textureHeight;
        ship->SetScale(scaleFactorX, scaleFactorY);
    }

    return ship;
}

/// @brief Creates a BasicShip, scaled appropriately with the window resolution.
/// @param pos Starting position to emplace ship at.
/// @param allegiance Allegiance to employ ship with.
/// @return Safe pointer to a BasicShip, compatible with the BaseShip base class.
std::shared_ptr<BaseShip> ShipFactory::CreateBasicShip(const sf::Vector2f &pos, Allegiance allegiance)
{
    auto ship = std::make_shared<BasicShip>(pos, allegiance);

    // Setup resolution-based scaling
    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::EnemyAssets::BasicShipSpriteKey);

    if (tex)
    {
        // A little redundant to getSize.x and getSize.y because BasicShip is a square. But explicit.
        float textureWidth = static_cast<float>(tex->getSize().x);
        float textureHeight = static_cast<float>(tex->getSize().y);
        float scaledWidth = ResolutionScaleManager::Instance().ScaleX(textureWidth);
        float scaledHeight = ResolutionScaleManager::Instance().ScaleY(textureHeight);
        float scaleFactorX = scaledWidth / textureWidth;
        float scaleFactorY = scaledHeight / textureHeight;
        ship->SetScale(scaleFactorX, scaleFactorY);
    }

    else
    {
        CT_LOG_ERROR("ShipFactory-BasicShip texture not found during creation!");
    }

    return ship;
}

/// @brief Creates an AlienShip, scaled appropriately with the window resolution.
/// @param pos Starting position to emplace ship at.
/// @param allegiance Allegiance to employ ship with.
/// @return Safe pointer to an AlienShip, compatible with the BaseShip base class.
std::shared_ptr<BaseShip> ShipFactory::CreateAlienShip(const sf::Vector2f &pos, Allegiance allegiance)
{
    auto ship = std::make_shared<AlienShip>(pos, allegiance);

    // Setup resolution-based scaling
    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::EnemyAssets::AlienShipSpriteKey);

    if (tex)
    {
        // A little redundant to getSize.x and getSize.y because AlienShip is a square. But explicit.
        float textureWidth = static_cast<float>(tex->getSize().x);
        float textureHeight = static_cast<float>(tex->getSize().y);
        float scaledWidth = ResolutionScaleManager::Instance().ScaleX(textureWidth);
        float scaledHeight = ResolutionScaleManager::Instance().ScaleY(textureHeight);
        float scaleFactorX = scaledWidth / textureWidth;
        float scaleFactorY = scaledHeight / textureHeight;
        ship->SetScale(scaleFactorX, scaleFactorY);
    }

    else
    {
        CT_LOG_ERROR("ShipFactory-AlienShip texture not found during creation!");
    }

    return ship;
}

/// @brief Creates a BerserkerShip, scaled appropriately with the window resolution.
/// @param pos Starting position to emplace ship at.
/// @param allegiance Allegiance to employ ship with.
/// @return Safe pointer to a BerserkerShip, compatible with the BaseShip base class.
std::shared_ptr<BaseShip> ShipFactory::CreateBerserkerShip(const sf::Vector2f &pos, Allegiance allegiance)
{
    auto ship = std::make_shared<BerserkerShip>(pos, allegiance);

    // Setup resolution-based scaling
    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::EnemyAssets::BerserkerShipSpriteKey);

    if (tex)
    {
        // resolution for BerserkerShip DOES matter, because it is 32 x 64 pixel.
        float textureWidth = static_cast<float>(tex->getSize().x);
        float textureHeight = static_cast<float>(tex->getSize().y);
        float scaledWidth = ResolutionScaleManager::Instance().ScaleX(textureWidth);
        float scaledHeight = ResolutionScaleManager::Instance().ScaleY(textureHeight);
        float scaleFactorX = scaledWidth / textureWidth;
        float scaleFactorY = scaledHeight / textureHeight;
        ship->SetScale(scaleFactorX, scaleFactorY);
    }

    else
    {
        CT_LOG_ERROR("ShipFactory-BerserkerShip texture not found during creation!");
    }

    return ship;
}

/// @brief Creates a CrusaderShip, scaled appropriately with the window resolution.
/// @param pos Starting position to emplace ship at.
/// @param allegiance Allegiance to employ ship with.
/// @return Safe pointer to a CrusaderShip, compatible with the BaseShip base class.
std::shared_ptr<BaseShip> ShipFactory::CreateCrusaderShip(const sf::Vector2f &pos, Allegiance allegiance)
{
    auto ship = std::make_shared<CrusaderShip>(pos, allegiance);

    // Setup resolution-based scaling
    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::EnemyAssets::CrusaderShipSpriteKey);

    if (tex)
    {
        // resolution for CrusaderShip DOES matter, because it is 32 x 64 pixel.
        float textureWidth = static_cast<float>(tex->getSize().x);
        float textureHeight = static_cast<float>(tex->getSize().y);
        float scaledWidth = ResolutionScaleManager::Instance().ScaleX(textureWidth);
        float scaledHeight = ResolutionScaleManager::Instance().ScaleY(textureHeight);
        float scaleFactorX = scaledWidth / textureWidth;
        float scaleFactorY = scaledHeight / textureHeight;
        ship->SetScale(scaleFactorX, scaleFactorY);
    }

    else
    {
        CT_LOG_ERROR("ShipFactory-CrusaderShip texture not found during creation!");
    }

    return ship;
}

/// @brief Creates a GruntShip, scaled appropriately with the window resolution.
/// @param pos Starting position to emplace ship at.
/// @param allegiance Allegiance to employ ship with.
/// @return Safe pointer to a GruntShip, compatible with the BaseShip base class.
std::shared_ptr<BaseShip> ShipFactory::CreateGruntShip(const sf::Vector2f &pos, Allegiance allegiance)
{
    auto ship = std::make_shared<GruntShip>(pos, allegiance);

    // Setup resolution-based scaling
    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::EnemyAssets::GruntShipSpriteKey);

    if (tex)
    {
        // A little redundant to getSize.x and getSize.y because GruntShip is a square. But explicit.
        float textureWidth = static_cast<float>(tex->getSize().x);
        float textureHeight = static_cast<float>(tex->getSize().y);
        float scaledWidth = ResolutionScaleManager::Instance().ScaleX(textureWidth);
        float scaledHeight = ResolutionScaleManager::Instance().ScaleY(textureHeight);
        float scaleFactorX = scaledWidth / textureWidth;
        float scaleFactorY = scaledHeight / textureHeight;
        ship->SetScale(scaleFactorX, scaleFactorY);
    }

    else
    {
        CT_LOG_ERROR("ShipFactory-GruntShip texture not found during creation!");
    }

    return ship;
}

/// @brief Creates an InvaderShip, scaled appropriately with the window resolution.
/// @param pos Starting position to emplace ship at.
/// @param allegiance Allegiance to employ ship with.
/// @return Safe pointer to a InvaderShip, compatible with the BaseShip base class.
std::shared_ptr<BaseShip> ShipFactory::CreateInvaderShip(const sf::Vector2f &pos, Allegiance allegiance)
{
    auto ship = std::make_shared<InvaderShip>(pos, allegiance);

    // Setup resolution-based scaling
    auto tex = AssetManager::Instance().GetTexture(SpriteAssets::EnemyAssets::InvaderShipSpriteKey);

    if (tex)
    {
        // resolution for InvaderShip DOES matter, because it is 32 x 64 pixel.
        float textureWidth = static_cast<float>(tex->getSize().x);
        float textureHeight = static_cast<float>(tex->getSize().y);
        float scaledWidth = ResolutionScaleManager::Instance().ScaleX(textureWidth);
        float scaledHeight = ResolutionScaleManager::Instance().ScaleY(textureHeight);
        float scaleFactorX = scaledWidth / textureWidth;
        float scaleFactorY = scaledHeight / textureHeight;
        ship->SetScale(scaleFactorX, scaleFactorY);
    }

    else
    {
        CT_LOG_ERROR("ShipFactory-InvaderShip texture not found during creation!");
    }

    return ship;
}
