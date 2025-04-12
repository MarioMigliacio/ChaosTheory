// ============================================================================
//  File        : AssetManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Asset Manager is the CT library dedicated manager of
//                texture, sounds and font cache memory
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "AssetManager.h"
#include "LogManager.h"

AssetManager &AssetManager::Instance()
{
    static AssetManager instance;
    return instance;
}

void AssetManager::Init(const Settings &settingsRef)
{
    settings = &settingsRef;

    std::string fallbackPath = settings->defaultFontPath;
    if (!fallbackFont.loadFromFile(fallbackPath))
    {
        CT_LOG_ERROR("Failed to load fallback font from '{}'. Text rendering "
                     "may be broken!",
                     fallbackPath);
    }
    else
    {
        CT_LOG_INFO("Loaded fallback font from '{}'", fallbackPath);
    }

    CT_LOG_INFO("AssetManager initialized.");
}

void AssetManager::Shutdown()
{
    CT_LOG_INFO("Clearing asset cache...");
    textures.clear();
    sounds.clear();
    fonts.clear();

    CT_LOG_INFO("AssetManager shutdown.");
    settings = nullptr;
}

sf::Texture &AssetManager::GetTexture(const std::string &filename)
{
    auto it = textures.find(filename);
    if (it != textures.end())
    {
        return it->second;
    }

    sf::Texture texture;
    if (!texture.loadFromFile(filename))
    {
        CT_LOG_ERROR("Failed to load texture: {}", filename);
    }
    else
    {
        CT_LOG_INFO("Loaded texture: {}", filename);
    }

    return textures[filename] = std::move(texture);
}

sf::SoundBuffer &AssetManager::GetSound(const std::string &filename)
{
    auto it = sounds.find(filename);
    if (it != sounds.end())
    {
        return it->second;
    }

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename))
    {
        CT_LOG_ERROR("Failed to load sound: {}", filename);
    }
    else
    {
        CT_LOG_INFO("Loaded sound: {}", filename);
    }

    return sounds[filename] = std::move(buffer);
}

sf::Font &AssetManager::GetFont(const std::string &filename)
{
    auto it = fonts.find(filename);
    if (it != fonts.end())
    {
        return it->second;
    }

    sf::Font font;
    if (!font.loadFromFile(filename))
    {
        CT_LOG_WARN("Failed to load font '{}'. Using fallback font instead.",
                    filename);
        return fallbackFont;
    }
    else
    {
        CT_LOG_INFO("Loaded font '{}'", filename);
    }

    return fonts[filename] = std::move(font);
}