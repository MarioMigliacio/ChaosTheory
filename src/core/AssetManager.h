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

#include "Settings.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class AssetManager
{
  public:
    static AssetManager &Instance();

    void Init(std::shared_ptr<const Settings> settings); // const - May not adjust settings
    void Shutdown();

    // Font
    bool LoadFont(const std::string &name, const std::string &filepath);
    sf::Font &GetFont(const std::string &name);

    // Texture
    bool LoadTexture(const std::string &name, const std::string &filepath);
    sf::Texture &GetTexture(const std::string &name);

    // Sound
    bool LoadSound(const std::string &name, const std::string &filepath);
    sf::SoundBuffer &GetSound(const std::string &name);

  private:
    AssetManager() = default;
    ~AssetManager() = default;

    AssetManager(const AssetManager &) = delete;
    AssetManager &operator=(const AssetManager &) = delete;

    std::unordered_map<std::string, sf::Texture> m_textures;
    std::unordered_map<std::string, sf::SoundBuffer> m_sounds;
    std::unordered_map<std::string, sf::Font> m_fonts;

    std::shared_ptr<const Settings> m_settings; // const - May not adjust settings
};
