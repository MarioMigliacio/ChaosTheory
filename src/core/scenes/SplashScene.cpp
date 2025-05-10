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
#include "MainMenuScene.h"
#include "SceneManager.h"
#include "WindowManager.h"
#include <filesystem>
#include <random>

namespace
{
constexpr float SHAKE_AMPLITUDE = 4.f;
constexpr float FADE_IN_DURATION = 2.0f;
constexpr float FADE_OUT_DURATION = 2.0f;
} // namespace

SplashScene::SplashScene(std::shared_ptr<Settings> settings) : m_settings(std::move(settings))
{
}

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

void SplashScene::LoadRequiredAssets()
{
    const std::string key = SplashAssets::SplashBackground;
    const std::string path = SplashAssets::Textures.at(key);

    if (!AssetManager::Instance().LoadTexture(key, path))
    {
        CT_LOG_WARN("Failed to load splash background.");
        return;
    }
}

// There is no extraneous logic for Shutdown on this Splash Scene, but logging is useful.
void SplashScene::Shutdown()
{
    m_isInitialized = false;
    CT_LOG_INFO("SplashScene Shutdown.");
}

// There is no extraneous logic for Exit on this Splash Scene, but logging is useful.
void SplashScene::OnExit()
{
    CT_LOG_INFO("SplashScene exited.");
}

// Per frame, update the Background image with Shake Effect, Fade-in/out, and silent asset loading.
void SplashScene::Update(float dt)
{
    UpdateFadeInOut(dt);
    ApplyShakeEffect(dt);

    if (m_fadingOut && m_fadeTimer >= FADE_OUT_DURATION)
    {
        SceneManager::Instance().RequestSceneChange(SceneID::MainMenu);
    }
}

// Explicitely remove the ability for Resize event flowdowns.
void SplashScene::HandleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::Resized)
    {
        // Ignore resize during splash
        return;
    }
}

// Disable any ability for user to resize window during Splash Scene.
void SplashScene::OnResize(const sf::Vector2u &newSize)
{
    // Intentionally do nothing
}

// While this scene is active, render the necessary components to the Splash Scene.
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

void SplashScene::LoadBackground()
{
    sf::Texture &bgTexture = AssetManager::Instance().GetTexture(SplashAssets::SplashBackground);

    m_background = std::make_unique<sf::Sprite>(bgTexture);

    const auto windowSize = WindowManager::Instance().GetWindow().getSize();
    const auto textureSize = bgTexture.getSize();

    const float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    const float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    m_background->setScale(scaleX, scaleY);
    m_background->setPosition(0.f, 0.f);

    CT_LOG_INFO("Splash background scaled to window.");
}

// Begin the fading in timer and boolean logic.
void SplashScene::StartFadeIn()
{
    m_fadingIn = true;
    m_fadeTimer = 0.f;
}

void SplashScene::StartFadeOut()
{
    m_fadingOut = true;
    m_fadeTimer = 0.f;
}

// Use update fade timer based on update delta times, Fade in the background alpha value.
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

// Use Sin and Cos functions plus some variance from clock cycles to give the Background image some motion.
void SplashScene::ApplyShakeEffect(float dt)
{
    m_shakeTimer += dt;

    float offsetX = static_cast<float>(std::sin(m_shakeTimer * 10.f)) * SHAKE_AMPLITUDE;
    float offsetY = static_cast<float>(std::cos(m_shakeTimer * 13.f)) * SHAKE_AMPLITUDE;

    sf::Vector2u winSize = WindowManager::Instance().GetWindow().getSize();
    m_background->setPosition(offsetX, offsetY);
}

// Forces the Window to a set resolution size, which uses the Titlebar style so no resizing or fullscreen buttons.
void SplashScene::LockWindow()
{
    sf::VideoMode mode = sf::VideoMode(1280, 720);
    sf::Uint32 style = sf::Style::Titlebar;

    WindowManager::Instance().Recreate(mode.width, mode.height, "Chaos Theory", style);
}
