// ============================================================================
//  File        : Background.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-16
//  Description : Provides simplified ResolutionScale with background images
//                and supports multiple layers, as well as fully controlable
//                directional updates. Implements parallax scrolling effect.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

/// @brief Represents a single parallax background layer.
struct ParallaxLayer
{
    std::string textureId;
    float parallaxFactor = 0.0f;
    sf::Sprite sprite;
    sf::Vector2f offset = {0.f, 0.f}; // horizontal and vertical offset
    sf::Vector2f motion = {1.f, 0.f}; // Default: scroll horizontally only
};

// ============================================================================
//  Class       : Background
//  Purpose     : Background class that supports both static and parallax layers.
//
//  Responsibilities:
//      - Initializes either static (one non moving texture), or parallax
//        (multiple, and moving)
//      - Updates position, and handles wrapping.
//
// ============================================================================
class Background
{
  public:
    Background();
    ~Background();

    void InitStatic(const std::string &textureId);
    void InitParallax(const std::vector<std::pair<std::string, float>> &layerData);

    void Update(float dt);
    void Draw(sf::RenderWindow &window);

    void SetLayerMotion(const std::string &textureId, const sf::Vector2f &motion);
    size_t GetLayerCount() const;
    sf::Vector2f GetLayerOffset(const std::string &textureId) const;

    void Clear();

  private:
    std::vector<ParallaxLayer> m_layers;
    bool m_isParallax = false;
};
