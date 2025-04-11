#include "AssetManager.h"
#include <stdexcept>

AssetManager& AssetManager::Instance() {
    static AssetManager instance;
    return instance;
}

sf::Texture& AssetManager::GetTexture(const std::string& filename) {
    auto found = textures.find(filename);
    if (found != textures.end()) {
        return found->second;
    }

    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load texture: " + filename);
    }

    textures[filename] = std::move(texture);
    return textures[filename];
}

sf::Font& AssetManager::GetFont(const std::string& filename) {
    auto found = fonts.find(filename);
    if (found != fonts.end()) {
        return found->second;
    }

    sf::Font font;
    if (!font.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load font: " + filename);
    }

    fonts[filename] = std::move(font);
    return fonts[filename];
}

sf::SoundBuffer& AssetManager::GetSound(const std::string& filename) {
    auto found = sounds.find(filename);
    if (found != sounds.end()) {
        return found->second;
    }

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load sound: " + filename);
    }

    sounds[filename] = std::move(buffer);
    return sounds[filename];
}