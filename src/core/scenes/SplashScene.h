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
#include <mutex>
#include <queue>
#include <thread>

// Struct representing an asset loading request
struct AssetLoadRequest
{
    std::string type;     // "texture", "sound", "font"
    std::string filepath; // Path to the asset file
};

// ============================================================================
//  Class       : SplashScene
//  Purpose     : Scene that displays the interactive main menu.
//                Contains background, title, and Play/Exit buttons.
//
//  Responsibilities:
//      - Fade in background image on scene start
//      - Shake animation(random offsets during display)
//      - Asynchronously load assets so AssetManager is fully loaded
//      - Transition to MainMenuScene once assets are loaded and fades out
//
// ============================================================================
class SplashScene final : public Scene
{
  public:
    using SceneChangeCallback = std::function<void(std::unique_ptr<Scene>)>;

    SplashScene(std::shared_ptr<Settings> settings);
    ~SplashScene() = default;

    SplashScene(const SplashScene &) = delete;
    SplashScene &operator=(const SplashScene &) = delete;

    void Init() override;
    void Shutdown() override;
    void OnExit() override;

    void Update(float dt) override;
    void HandleEvent(const sf::Event &event) override;
    void OnResize(const sf::Vector2u &newSize) override;
    void Render() override;

    void SetSceneChangeCallback(SceneChangeCallback callback);

  private:
    void LoadBackground();
    void QueueAssets();
    void ProcessAssetQueue(float dt);
    void StartFadeIn();
    void UpdateFadeInOut(float dt);
    void ApplyShakeEffect(float dt);
    void LockWindow();

  private:
    std::shared_ptr<Settings> m_settings;
    SceneChangeCallback m_sceneChangeCallback;

    std::unique_ptr<sf::Sprite> m_background;
    std::unique_ptr<sf::Texture> m_backgroundTexture;

    bool m_fadingIn = false;
    bool m_fadingOut = false;
    bool m_doneLoading = false;

    float m_assetTimer = 0.f;
    float m_assetLoadDelay = 0.05f;

    float m_fadeTimer = 0.f;
    float m_fadeInDuration = 2.0f;
    float m_fadeOutDuration = 4.0f;

    float m_shakeTimer = 0.f;
    float m_shakeAmplitude = 4.f;

    std::queue<AssetLoadRequest> m_assetQueue;
};
