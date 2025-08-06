// ============================================================================
//  File        : SpatialGrid.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-25
//  Description : Grid-based spatial partitioning for collision optimization.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "BaseCollidable.h"
#include <memory>
#include <unordered_map>
#include <vector>

/// @brief A utility struct to represent grid cell coordinates.
/// @param x X coordinate.
/// @param y Y coordinate.
struct GridCoord
{
    int x = 0;
    int y = 0;

    bool operator==(const GridCoord &other) const
    {
        return x == other.x && y == other.y;
    }
};

// Required to use GridCoord as unordered_map key
namespace std
{
template <> struct hash<GridCoord>
{
    std::size_t operator()(const GridCoord &k) const
    {
        return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1);
    }
};
} // namespace std

// ============================================================================
//  Class       : SpatialGrid
//  Purpose     : SpatialGrid class partitions 2D space into cells,
//                each containing collidable entities.
//
//  Responsibilities:
//      - localized collision checks instead of brute force N^2 comparisons.
//
// ============================================================================
class SpatialGrid
{
  public:
    SpatialGrid(float cellSize);
    ~SpatialGrid() = default;

    void Clear();
    void Insert(std::shared_ptr<BaseCollidable> object);

    std::vector<std::shared_ptr<BaseCollidable>> QueryNearby(const sf::FloatRect &bounds) const;
    size_t GetActiveCellCount() const;

  private:
    GridCoord GetCoord(const sf::Vector2f &pos) const;
    std::vector<GridCoord> GetCoveredCells(const sf::FloatRect &bounds) const;

  private:
    float m_cellSize;
    std::unordered_map<GridCoord, std::vector<std::shared_ptr<BaseCollidable>>> m_cells;
};
