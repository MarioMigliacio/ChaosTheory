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
#include "Macros.h"
#include "ResolutionScaleManager.h"

/// @brief Get the current Instance for this ShipFactory singleton.
/// @return reference to existing ShipFactory interface.
ShipFactory &ShipFactory::Instance()
{
    static ShipFactory instance;
    return instance;
}

/// @brief Creates a BasicShip, scaled appropriately with the window resolution.
/// @param pos Starting position to emplace ship at.
/// @param allegiance Allegiance to employ ship with.
/// @return Safe pointer to a BasicShip, compatible with the BaseShip base class.
std::shared_ptr<BaseShip> ShipFactory::CreateBasicShip(const sf::Vector2f &pos, int allegiance)
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

/// @brief Creates a AlienShip, scaled appropriately with the window resolution.
/// @param pos Starting position to emplace ship at.
/// @param allegiance Allegiance to employ ship with.
/// @return Safe pointer to an AlienShip, compatible with the BaseShip base class.
std::shared_ptr<BaseShip> ShipFactory::CreateAlienShip(const sf::Vector2f &pos, int allegiance)
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
