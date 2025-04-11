#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

class AssetManager {
public:
    static AssetManager& Instance();

    sf::Texture& GetTexture(const std::string& filename);
    sf::Font& GetFont(const std::string& filename);
    sf::SoundBuffer& GetSound(const std::string& filename);

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager& operator=(AssetManager&&) = delete;

private:
    AssetManager() = default;
    ~AssetManager() = default;

    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    std::unordered_map<std::string, sf::SoundBuffer> sounds;
};