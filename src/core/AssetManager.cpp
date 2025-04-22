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
#include "Macros.h"
#include "Settings.h"

#include <filesystem>

// These static references to certain sf objects are used for short circuit logic where the AssetManager might not yet
// be initialized, so doing routine logic would be dangerous
namespace
{
static sf::SoundBuffer dummySoundBuffer;
static sf::Texture dummyTexture;
static sf::Font dummyFont;
} // namespace

AssetManager &AssetManager::Instance()
{
    static AssetManager instance;
    return instance;
}

// Initializes the Asset manager using the provided settings.
void AssetManager::Init(std::shared_ptr<const Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;
    namespace fs = std::filesystem;

    CT_LOG_INFO("Initializing AssetManager...");

    m_isInitialized = true;

    // LoadAllAssets();

    CT_LOG_INFO("AssetManager initialized.");
}

// Shuts down the Asset manager and resets internal state.
void AssetManager::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("AssetManager", "Shutdown");

    CT_LOG_INFO("Clearing asset cache...");

    m_textures.clear();
    m_sounds.clear();
    m_fonts.clear();
    m_isInitialized = false;

    CT_LOG_INFO("AssetManager shutdown.");
}

void AssetManager::LoadAllAssets()
{
    CT_LOG_INFO("Loading all assets from configured directories...");

    namespace fs = std::filesystem;

    // --------- FONTS ----------
    for (const auto &entry : fs::directory_iterator(m_settings->m_fontDirectory))
    {
        if (entry.is_regular_file())
        {
            std::string filename = entry.path().filename().string();
            std::string name = entry.path().stem().string(); // no extension
            std::string fullPath = entry.path().string();

            if (LoadFont(name, fullPath))
            {
                CT_LOG_TRACE("Loaded font '{}'", filename);
            }
            else
            {
                CT_LOG_WARN("Failed to load font '{}'", filename);
            }
        }
    }

    // --------- TEXTURES ----------
    for (const auto &entry : fs::directory_iterator(m_settings->m_spriteDirectory))
    {
        if (entry.is_regular_file())
        {
            std::string filename = entry.path().filename().string();
            std::string name = entry.path().stem().string();
            std::string fullPath = entry.path().string();

            if (LoadTexture(name, fullPath))
            {
                CT_LOG_TRACE("Loaded texture '{}'", filename);
            }
            else
            {
                CT_LOG_WARN("Failed to load texture '{}'", filename);
            }
        }
    }

    // --------- SOUNDS ----------
    for (const auto &entry : fs::directory_iterator(m_settings->m_audioDirectory))
    {
        if (entry.is_regular_file())
        {
            std::string filename = entry.path().filename().string();
            std::string name = entry.path().stem().string();
            std::string fullPath = entry.path().string();

            if (LoadSound(name, fullPath))
            {
                CT_LOG_TRACE("Loaded sound '{}'", filename);
            }
            else
            {
                CT_LOG_WARN("Failed to load sound '{}'", filename);
            }
        }
    }

    CT_LOG_INFO("Finished loading all assets.");
}

// Returns whether the Asset manager has been initialized.
bool AssetManager::IsInitialized() const
{
    return m_isInitialized;
}

// Load the requested font into internal storage for later use by name index.
bool AssetManager::LoadFont(const std::string &name, const std::string &filepath)
{
    CT_WARN_IF_UNINITIALIZED_RET("AssetManager", "LoadFont", false);

    sf::Font font;

    if (!font.loadFromFile(filepath))
    {
        CT_LOG_ERROR("Failed to load font: {}", filepath);
        return false;
    }

    m_fonts[name] = std::move(font);

    return true;
}

// Return a reference to the requested font if it exists in internal storage.
sf::Font &AssetManager::GetFont(const std::string &name)
{
    CT_WARN_IF_UNINITIALIZED_RET("AssetManager", "GetFont", dummyFont);

    if (!m_fonts.contains(name))
    {
        CT_LOG_WARN("Font '{}' not found. Using Default.", name);

        return m_fonts["Default"];
    }

    return m_fonts[name];
}

// Load the requested texture into internal storage for later use by name index.
bool AssetManager::LoadTexture(const std::string &name, const std::string &filepath)
{
    CT_WARN_IF_UNINITIALIZED_RET("AssetManager", "LoadTexture", false);

    sf::Texture texture;

    if (!texture.loadFromFile(filepath))
    {
        CT_LOG_ERROR("Failed to load texture: {}", filepath);

        return false;
    }

    m_textures[name] = std::move(texture);

    return true;
}

// Return a reference to the requested texture if it exists in internal storage.
sf::Texture &AssetManager::GetTexture(const std::string &name)
{
    CT_WARN_IF_UNINITIALIZED_RET("AssetManager", "GetTexture", dummyTexture);

    if (!m_textures.contains(name))
    {
        CT_LOG_WARN("Texture '{}' not found. Using Default.", name);

        return m_textures["Default"];
    }

    return m_textures[name];
}

// Load the requested sound into internal storage for alter use by name index.
bool AssetManager::LoadSound(const std::string &name, const std::string &filepath)
{
    CT_WARN_IF_UNINITIALIZED_RET("AssetManager", "LoadSound", false);

    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(filepath))
    {
        CT_LOG_ERROR("Failed to load sound: {}", filepath);

        return false;
    }

    m_sounds[name] = std::move(buffer);

    return true;
}

// Return a reference to the requested sound if it exists in internal storage.
sf::SoundBuffer &AssetManager::GetSound(const std::string &name)
{
    CT_WARN_IF_UNINITIALIZED_RET("AssetManager", "GetSound", dummySoundBuffer);

    if (!m_sounds.contains(name))
    {
        CT_LOG_WARN("Sound '{}' not found. Using Default.", name);

        return m_sounds["Default"];
    }

    return m_sounds[name];
}
