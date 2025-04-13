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
#include "Settings.h"

#include <filesystem>

AssetManager &AssetManager::Instance()
{
    static AssetManager instance;
    return instance;
}

void AssetManager::Init(std::shared_ptr<const Settings> settings)
{
    m_settings = std::move(settings);
    namespace fs = std::filesystem;

    CT_LOG_INFO("Initializing AssetManager...");

    if (!fs::exists(m_settings->m_fontDirectory))
    {
        CT_LOG_WARN("Font directory not found: {}",
                    m_settings->m_fontDirectory);
    }

    if (!fs::exists(m_settings->m_audioDirectory))
    {
        CT_LOG_WARN("Audio directory not found: {}",
                    m_settings->m_audioDirectory);
    }

    if (!fs::exists(m_settings->m_spriteDirectory))
    {
        CT_LOG_WARN("Sprite directory not found: {}",
                    m_settings->m_spriteDirectory);
    }

    // Load fallback font
    std::string fallbackFont = m_settings->m_fontDirectory + "DefaultFont.ttf";
    if (!LoadFont("default", fallbackFont))
    {
        CT_LOG_WARN("Failed to load fallback font: {}", fallbackFont);
    }

    // Load fallback texture
    std::string fallbackTexture =
        m_settings->m_spriteDirectory + "playerShip.png";
    if (!LoadTexture("default", fallbackTexture))
    {
        CT_LOG_WARN("Failed to load fallback texture: {}", fallbackTexture);
    }

    // Load fallback sound
    std::string fallbackSound = m_settings->m_audioDirectory + "fallback.wav";
    if (!LoadSound("default", fallbackSound))
    {
        CT_LOG_WARN("Failed to load fallback sound: {}", fallbackSound);
    }

    CT_LOG_INFO("AssetManager initialized.");
}

void AssetManager::Shutdown()
{
    CT_LOG_INFO("Clearing asset cache...");
    m_textures.clear();
    m_sounds.clear();
    m_fonts.clear();

    CT_LOG_INFO("AssetManager shutdown.");
}

bool AssetManager::LoadFont(const std::string &name,
                            const std::string &filepath)
{
    sf::Font font;
    if (!font.loadFromFile(filepath))
    {
        CT_LOG_ERROR("Failed to load font: {}", filepath);
        return false;
    }

    m_fonts[name] = std::move(font);
    return true;
}

sf::Font &AssetManager::GetFont(const std::string &name)
{
    if (m_fonts.find(name) == m_fonts.end())
    {
        CT_LOG_WARN("Font '{}' not found. Using fallback.", name);
        return m_fonts["default"];
    }
    return m_fonts[name];
}

bool AssetManager::LoadTexture(const std::string &name,
                               const std::string &filepath)
{
    sf::Texture texture;
    if (!texture.loadFromFile(filepath))
    {
        CT_LOG_ERROR("Failed to load texture: {}", filepath);
        return false;
    }

    m_textures[name] = std::move(texture);
    return true;
}

sf::Texture &AssetManager::GetTexture(const std::string &name)
{
    if (m_textures.find(name) == m_textures.end())
    {
        CT_LOG_WARN("Texture '{}' not found. Using fallback.", name);
        return m_textures["default"];
    }
    return m_textures[name];
}

bool AssetManager::LoadSound(const std::string &name,
                             const std::string &filepath)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath))
    {
        CT_LOG_ERROR("Failed to load sound: {}", filepath);
        return false;
    }

    m_sounds[name] = std::move(buffer);
    return true;
}

sf::SoundBuffer &AssetManager::GetSound(const std::string &name)
{
    if (m_sounds.find(name) == m_sounds.end())
    {
        CT_LOG_WARN("Sound '{}' not found. Using fallback.", name);
        return m_sounds["default"];
    }
    return m_sounds[name];
}