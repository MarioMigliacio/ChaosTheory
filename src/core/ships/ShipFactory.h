// ============================================================================
//  File        : ShipFactory.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-17
//  Description : Factory for creating ship instances.

//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseShip.h"
#include <memory>

// ============================================================================
//  Class       : ShipFactory
//  Purpose     : Singleton class that manages the generation of Ship elements.
//
//  Responsibilities:
//      - Scale texture sizes and generate safe pointer Ship objects.
//
// ============================================================================
class ShipFactory
{
  public:
    static ShipFactory &Instance();

    std::shared_ptr<BaseShip> CreatePlayerShip(const sf::Vector2f &pos);
    std::shared_ptr<BaseShip> CreateBasicShip(const sf::Vector2f &pos, Allegiance allegiance);
    std::shared_ptr<BaseShip> CreateAlienShip(const sf::Vector2f &pos, Allegiance allegiance);
    std::shared_ptr<BaseShip> CreateBerserkerShip(const sf::Vector2f &pos, Allegiance allegiance);
    std::shared_ptr<BaseShip> CreateCrusaderShip(const sf::Vector2f &pos, Allegiance allegiance);
    std::shared_ptr<BaseShip> CreateGruntShip(const sf::Vector2f &pos, Allegiance allegiance);
    std::shared_ptr<BaseShip> CreateInvaderShip(const sf::Vector2f &pos, Allegiance allegiance);

  private:
    ShipFactory() = default;
    ~ShipFactory() = default;

    ShipFactory(const ShipFactory &) = delete;
    ShipFactory &operator=(const ShipFactory &) = delete;
};
