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

class ShipFactory
{
  public:
    static ShipFactory &Instance();

    std::shared_ptr<BaseShip> CreateBasicShip(const sf::Vector2f &pos, int allegiance);
    std::shared_ptr<BaseShip> CreateAlienShip(const sf::Vector2f &pos, int allegiance);

  private:
    ShipFactory() = default;
    ~ShipFactory() = default;

    ShipFactory(const ShipFactory &) = delete;
    ShipFactory &operator=(const ShipFactory &) = delete;
};
