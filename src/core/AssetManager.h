// ============================================================================
//  File        : AssetManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Asset Manager is the CT library dedicated manager of
//                texture, sounds and font cache memory
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "Settings.h"

class AssetManager
{
  public:
    static AssetManager &Instance();

    void Init(const Settings &settings);
    void Shutdown();

    sf::Texture &GetTexture(const std::string &filename);
    sf::SoundBuffer &GetSound(const std::string &filename);
    sf::Font &GetFont(const std::string &filename);

  private:
    AssetManager() = default;
    ~AssetManager() = default;

    AssetManager(const AssetManager &) = delete;
    AssetManager &operator=(const AssetManager &) = delete;

    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::SoundBuffer> sounds;
    std::unordered_map<std::string, sf::Font> fonts;

    const Settings *settings = nullptr;
    sf::Font fallbackFont;
};
