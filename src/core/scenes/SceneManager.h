// ============================================================================
//  File        : SceneManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-13
//  Description : Scene Manager is the CT library dedicated manager of
//                game transitional states
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Scene.h"
#include "Settings.h"
#include <memory>
#include <stack>

/// @brief Simple enumeration field to represent a type of scene.
enum class SceneID
{
    Splash,
    MainMenu,
    Settings,
    Game,
};

/// @brief Utility function to convert SceneID to string
/// @param id which SceneID enumeration.
/// @return readyonly string identifying the SceneID enum.
inline const char *SceneIDToString(SceneID id)
{
    switch (id)
    {
        case SceneID::Splash:
            return "Splash";
        case SceneID::MainMenu:
            return "MainMenu";
        case SceneID::Settings:
            return "Settings";
        case SceneID::Game:
            return "Game";
        default:
            return "Unknown";
    }
}

// ============================================================================
//  Class       : SceneManager
//  Purpose     : Singleton class that manages the Scene Abstract base class.
//
//  Responsibilities:
//      - Initializes and shuts down
//      - Stores Scene transition logic, and update management
//
// ============================================================================
class SceneManager
{
  public:
    using SceneCreateFunc = std::function<std::unique_ptr<Scene>()>;

    static SceneManager &Instance();

    void Init(std::shared_ptr<Settings> settings);
    void Shutdown();
    bool IsInitialized() const;

    void Update(float dt);
    void HandleEvent(const sf::Event &event);
    void Render();

    void Register(SceneID sceneId, SceneCreateFunc creator);
    void RegisterAllDefaultScenes();
    std::unique_ptr<Scene> Create(SceneID sceneId);

    void PushScene(std::unique_ptr<Scene> scene);
    void PopScene();
    void ReplaceScene(std::unique_ptr<Scene> newScene);
    void ClearScenes();

    bool IsEmpty() const;
    bool HasActiveScene() const;
    std::size_t GetSceneCount() const;
    Scene *GetActiveScene() const;

    void RequestSceneChange(SceneID id);

  private:
    SceneManager() = default;
    ~SceneManager() = default;

    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;

  private:
    std::unordered_map<SceneID, SceneCreateFunc> m_sceneRegistry;
    std::stack<std::unique_ptr<Scene>> m_scenes;
    std::shared_ptr<Settings> m_settings;
    bool m_isInitialized = false;
};
