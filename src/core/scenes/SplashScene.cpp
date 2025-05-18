// ============================================================================
//  File        : SplashScene.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-21
//  Description : Hosts the definitions for Splash Screen scene Object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SplashScene.h"
#include "AssetManager.h"
#include "Macros.h"
#include "SceneManager.h"
#include "SplashAssets.h"
#include "WindowManager.h"
#include <filesystem>
#include <random>

namespace
{
/// @brief Controls how far the texture image will shake during update.
constexpr float SHAKE_AMPLITUDE = 4.f;

/// @brief Controls how long the SplashScene will fade in.
constexpr float FADE_IN_DURATION = 2.0f;

/// @brief Controls how long the SplashScene will fade out.
constexpr float FADE_OUT_DURATION = 2.0f;
} // namespace

/// @brief Constructor for the SplashScene.
/// @param settings Internal settings to initialize with.
SplashScene::SplashScene(std::shared_ptr<Settings> settings) : m_settings(std::move(settings))
{
}

/// @brief Initialize the internal components for this SplashScene.
void SplashScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    LockWindow();
    LoadRequiredAssets();
    LoadBackground();
    StartFadeIn();

    m_isInitialized = true;

    CT_LOG_INFO("SplashScene initialized.");
}

/// @brief Load any required assets listed in the SplashAssets namespace.
void SplashScene::LoadRequiredAssets()
{
    const std::string key = SplashAssets::SplashBackground;
    const std::string path = SplashAssets::Textures.at(key);

    if (!AssetManager::Instance().LoadTexture(key, path))
    {
        CT_LOG_ERROR("SplashScene: Failed to load splash background.");
    }

    CT_LOG_INFO("SplashScene finished LoadRequiredAssets.");
}

/// @brief Shuts down this scene and resets internal state.
void SplashScene::Shutdown()
{
    m_isInitialized = false;

    CT_LOG_INFO("SplashScene Shutdown.");
}

/// @brief Handles the exit criteria for this scene.
void SplashScene::OnExit()
{
    CT_LOG_INFO("SplashScene OnExit.");
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update.
void SplashScene::Update(float dt)
{
    UpdateFadeInOut(dt);
    ApplyShakeEffect(dt);

    if (m_fadingOut && m_fadeTimer >= FADE_OUT_DURATION || m_hasPendingTransition)
    {
        CT_LOG_INFO("SplashScene requesting scene change.");

        SceneManager::Instance().RequestSceneChange(SceneID::MainMenu);
    }
}

/// @brief Handle any quick cancelation requests if present.
/// @param event bubbled down from caller, not needed.
void SplashScene::HandleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Enter ||
            event.key.code == sf::Keyboard::Space)
        {
            m_hasPendingTransition = true;

            CT_LOG_INFO("SplashScene: skip event handled.");
        }
    }
}

/// @brief Not used in SplashScene context.
/// @param newSize bubbled down from caller, not needed.
void SplashScene::OnResize(const sf::Vector2u &newSize)
{
}

/// @brief While this scene is active, render the necessary components.
void SplashScene::Render()
{
    auto &window = WindowManager::Instance().GetWindow();

    window.clear();

    if (m_background)
    {
        window.draw(*m_background);
    }

    window.display();
}

/// @brief Loads the Splash texture background image from the SplashAssets namespace.
/// @note SplashScene uses a very specific movement pattern on the sprite itself, so we
/// do not use the Background class here.
void SplashScene::LoadBackground()
{
    sf::Texture &bgTexture = *AssetManager::Instance().GetTexture(SplashAssets::SplashBackground);

    m_background = std::make_unique<sf::Sprite>(bgTexture);

    const auto windowSize = WindowManager::Instance().GetWindow().getSize();
    const auto textureSize = bgTexture.getSize();

    const float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    const float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    m_background->setScale(scaleX, scaleY);
    m_background->setPosition(0.f, 0.f);

    CT_LOG_INFO("Splash background scaled to window.");
}

/// @brief Begin the fading in timer and boolean logic.
void SplashScene::StartFadeIn()
{
    m_fadingIn = true;
    m_fadeTimer = 0.f;
}

/// @brief Begin the fading out timer and boolean logic.
void SplashScene::StartFadeOut()
{
    m_fadingOut = true;
    m_fadeTimer = 0.f;
}

/// @brief Update fade timer based on update delta times, Fade in the background alpha value.
/// @param dt delta time since last update.
void SplashScene::UpdateFadeInOut(float dt)
{
    m_fadeTimer += dt;

    uint8_t alpha = 255;

    if (m_fadingIn)
    {
        alpha = static_cast<uint8_t>(std::min(255.f, (m_fadeTimer / FADE_IN_DURATION) * 255.f));

        if (m_fadeTimer >= FADE_IN_DURATION)
        {
            m_fadingIn = false;

            StartFadeOut();
        }
    }

    else if (m_fadingOut)
    {
        alpha = static_cast<uint8_t>(std::max(0.f, 255.f - (m_fadeTimer / FADE_OUT_DURATION) * 255.f));
    }

    sf::Color bgColor = m_background->getColor();
    bgColor.a = alpha;
    m_background->setColor(bgColor);
}

/// @brief Use Sin and Cos functions plus some variance from clock cycles to give the Background image some motion.
/// @param dt delta time since last update.
void SplashScene::ApplyShakeEffect(float dt)
{
    m_shakeTimer += dt;

    float offsetX = static_cast<float>(std::sin(m_shakeTimer * 10.f)) * SHAKE_AMPLITUDE;
    float offsetY = static_cast<float>(std::cos(m_shakeTimer * 13.f)) * SHAKE_AMPLITUDE;

    sf::Vector2u winSize = WindowManager::Instance().GetWindow().getSize();
    m_background->setPosition(offsetX, offsetY);
}

/// @brief Forces the Window to a specific resolution size, tailored for the SplashScene.
void SplashScene::LockWindow()
{
    sf::VideoMode mode = sf::VideoMode(1280, 720);
    sf::Uint32 style = sf::Style::Titlebar;

    WindowManager::Instance().Recreate(mode.width, mode.height, m_settings->m_windowTitle, style);
}
