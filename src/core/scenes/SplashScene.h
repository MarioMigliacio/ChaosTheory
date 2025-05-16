// ============================================================================
//  File        : SplashScene.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-21
//  Description : Hosts the definitions for Splash Screen scene Object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Scene.h"
#include "Settings.h"
#include <SFML/Graphics.hpp>
#include <memory>

// ============================================================================
//  Class       : SplashScene
//  Purpose     : Scene that displays the Splash entry scene for ChaosTheory.
//
//  Responsibilities:
//      - Fade in background image on scene start
//      - Shake animation(random offsets during display)
//      - Transition to MainMenuScene once it fades out
//
// ============================================================================
class SplashScene final : public Scene
{
  public:
    SplashScene(std::shared_ptr<Settings> settings);
    ~SplashScene() = default;

    SplashScene(const SplashScene &) = delete;
    SplashScene &operator=(const SplashScene &) = delete;

    void Init() override;
    void LoadRequiredAssets() override;
    void Shutdown() override;
    void OnExit() override;

    void Update(float dt) override;
    void HandleEvent(const sf::Event &event) override;
    void OnResize(const sf::Vector2u &newSize) override;
    void Render() override;

  private:
    void LoadBackground();
    void StartFadeIn();
    void StartFadeOut();
    void UpdateFadeInOut(float dt);
    void ApplyShakeEffect(float dt);
    void LockWindow();

  private:
    std::shared_ptr<Settings> m_settings;

    std::unique_ptr<sf::Sprite> m_background;
    std::unique_ptr<sf::Texture> m_backgroundTexture;

    bool m_fadingIn = false;
    bool m_fadingOut = false;

    float m_fadeTimer = 0.f;
    float m_shakeTimer = 0.f;
};
