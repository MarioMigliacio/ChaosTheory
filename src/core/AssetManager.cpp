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

/// @brief These static references to certain sf objects are used for short circuit logic where the AssetManager might
/// not yet be initialized, so doing routine logic would be dangerous.
namespace
{
/// @brief An empty, but valid SoundBuffer.
static sf::SoundBuffer dummySoundBuffer;

/// @brief An empty, but valid Texture.
static sf::Texture dummyTexture;

/// @brief An empty, but valid Font.
static sf::Font dummyFont;
} // namespace

/// @brief Get the current Instance for this AssetManager singleton.
/// @return reference to existing AssetManager interface.
AssetManager &AssetManager::Instance()
{
    static AssetManager instance;
    return instance;
}

/// @brief Initializes the AssetManager using the provided settings.
/// @param settings Settings needed for initializing.
void AssetManager::Init(std::shared_ptr<const Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;

    m_isInitialized = true;

    CT_LOG_INFO("AssetManager initialized.");
}

/// @brief Shuts down the AssetManager and resets internal state.
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

/// @brief Returns whether the Asset manager has been initialized.
/// @return m_isInitialized.
bool AssetManager::IsInitialized() const
{
    return m_isInitialized;
}

/// @brief Load the requested font into internal storage for later use by name index.
/// @param name index to store.
/// @param filepath value to store.
/// @return true / false
bool AssetManager::LoadFont(const std::string &name, const std::string &filepath)
{
    CT_WARN_IF_UNINITIALIZED_RET("AssetManager", "LoadFont", false);

    if (m_fonts.contains(name))
    {
        return true;
    }

    sf::Font font;

    if (!font.loadFromFile(filepath))
    {
        CT_LOG_ERROR("Failed to load font: {}", filepath);

        return false;
    }

    m_fonts[name] = std::move(font);

    return true;
}

/// @brief Return a pointer to the requested font if it exists in internal storage.
/// @param name index to fetch.
/// @return m_fonts[index]
sf::Font *AssetManager::GetFont(const std::string &name)
{
    CT_WARN_IF_UNINITIALIZED_RET("AssetManager", "GetFont", nullptr);

    auto it = m_fonts.find(name);

    if (it == m_fonts.end())
    {
        CT_LOG_WARN("Font '{}' not found.", name);

        return nullptr;
    }

    return &it->second;
}

/// @brief Load the requested texture into internal storage for later use by name index.
/// @param name index to store.
/// @param filepath value to store.
/// @return true / false
bool AssetManager::LoadTexture(const std::string &name, const std::string &filepath)
{
    CT_WARN_IF_UNINITIALIZED_RET("AssetManager", "LoadTexture", false);

    if (m_textures.contains(name))
    {
        return true;
    }

    sf::Texture texture;

    if (!texture.loadFromFile(filepath))
    {
        CT_LOG_ERROR("Failed to load texture: {}", filepath);

        return false;
    }

    m_textures[name] = std::move(texture);

    return true;
}

/// @brief Return a pointer to the requested texture if it exists in internal storage.
/// @param name index to fetch.
/// @return m_textures[index]
sf::Texture *AssetManager::GetTexture(const std::string &name)
{
    CT_WARN_IF_UNINITIALIZED_RET("AssetManager", "GetTexture", nullptr);

    auto it = m_textures.find(name);

    if (it == m_textures.end())
    {
        CT_LOG_WARN("Texture '{}' not found.", name);

        return nullptr;
    }

    return &it->second;
}

/// @brief Load the requested sound into internal storage for later use by name index.
/// @param name index to store.
/// @param filepath value to store.
/// @return true / false
bool AssetManager::LoadSound(const std::string &name, const std::string &filepath)
{
    CT_WARN_IF_UNINITIALIZED_RET("AssetManager", "LoadSound", false);

    if (m_sounds.contains(name))
    {
        return true;
    }

    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(filepath))
    {
        CT_LOG_ERROR("Failed to load sound: {}", filepath);

        return false;
    }

    m_sounds[name] = std::move(buffer);

    return true;
}

/// @brief Return a pointer to the requested sound if it exists in internal storage.
/// @param name index to fetch.
/// @return m_sounds[index]
sf::SoundBuffer *AssetManager::GetSound(const std::string &name)
{
    CT_WARN_IF_UNINITIALIZED_RET("AssetManager", "GetSound", nullptr);

    auto it = m_sounds.find(name);

    if (it == m_sounds.end())
    {
        CT_LOG_WARN("Texture '{}' not found.", name);

        return nullptr;
    }

    return &it->second;
}
