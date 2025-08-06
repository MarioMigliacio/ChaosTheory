// ============================================================================
//  File        : SpatialGrid.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-25
//  Description : Grid-based spatial partitioning for collision optimization.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SpatialGrid.h"
#include <cmath>
#include <unordered_set>

/// @brief Constructs a SpatialGrid with specified cell size.
/// @param cellSize Size in pixels for each grid cell.
SpatialGrid::SpatialGrid(float cellSize) : m_cellSize(cellSize)
{
}

/// @brief Removes all objects from the spatial grid.
void SpatialGrid::Clear()
{
    m_cells.clear();
}

/// @brief Inserts an object into every grid cell it overlaps.
/// @param object Shared pointer to a BaseCollidable.
void SpatialGrid::Insert(std::shared_ptr<BaseCollidable> object)
{
    if (!object || !object->IsAlive())
    {
        return;
    }

    const auto &bounds = object->GetBounds();
    const auto cells = GetCoveredCells(bounds);

    for (const auto &cell : cells)
    {
        m_cells[cell].push_back(object);
    }
}

/// @brief Queries nearby objects that may collide with the provided bounding box.
/// @param bounds Axis-aligned bounding box in world coordinates.
/// @return A vector of candidate objects.
std::vector<std::shared_ptr<BaseCollidable>> SpatialGrid::QueryNearby(const sf::FloatRect &bounds) const
{
    std::vector<std::shared_ptr<BaseCollidable>> result;
    std::unordered_set<std::shared_ptr<BaseCollidable>> added;

    const auto cells = GetCoveredCells(bounds);

    for (const auto &cell : cells)
    {
        auto it = m_cells.find(cell);
        if (it == m_cells.end())
        {
            continue;
        }

        for (const auto &obj : it->second)
        {
            if (added.insert(obj).second) // prevents duplicates
            {
                result.push_back(obj);
            }
        }
    }

    return result;
}

/// @brief Returns the number of active (non-empty) cells in the grid.
size_t SpatialGrid::GetActiveCellCount() const
{
    return m_cells.size();
}

/// @brief Converts a world position to a grid coordinate.
/// @param pos World position.
/// @return Corresponding grid coordinate.
GridCoord SpatialGrid::GetCoord(const sf::Vector2f &pos) const
{
    int x = static_cast<int>(std::floor(pos.x / m_cellSize));
    int y = static_cast<int>(std::floor(pos.y / m_cellSize));

    return {x, y};
}

/// @brief Finds all cells covered by a bounding box.
/// @param bounds World-space bounding box.
/// @return Vector of grid cell coordinates overlapped by the bounds.
std::vector<GridCoord> SpatialGrid::GetCoveredCells(const sf::FloatRect &bounds) const
{
    std::vector<GridCoord> cells;

    int startX = static_cast<int>(std::floor(bounds.left / m_cellSize));
    int endX = static_cast<int>(std::floor((bounds.left + bounds.width) / m_cellSize));
    int startY = static_cast<int>(std::floor(bounds.top / m_cellSize));
    int endY = static_cast<int>(std::floor((bounds.top + bounds.height) / m_cellSize));

    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            cells.push_back({x, y});
        }
    }

    return cells;
}
