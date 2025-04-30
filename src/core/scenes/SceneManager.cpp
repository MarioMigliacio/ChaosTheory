// ============================================================================
//  File        : SceneManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-13
//  Description : Scene Manager is the CT library dedicated manager of
//                game transitional states
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SceneManager.h"
#include "GameScene.h"
#include "Macros.h"
#include "MainMenuScene.h"
#include "SettingsScene.h"
#include "SplashScene.h"

SceneManager &SceneManager::Instance()
{
    static SceneManager instance;
    return instance;
}

// Initializes the Scene Manager.
void SceneManager::Init(std::shared_ptr<Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;
    m_isInitialized = true;

    RegisterAllDefaultScenes();

    CT_LOG_INFO("SceneManager ready. Awaiting first scene push.");
    CT_LOG_INFO("SceneManager Initialized.");
}

// Shuts down the Scene manager and resets internal state, calling for all child scenes to exit and shutdown.
void SceneManager::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "Shutdown");

    while (!m_scenes.empty())
    {
        if (m_scenes.top())
        {
            m_scenes.top()->OnExit();
            m_scenes.top()->Shutdown();
        }

        m_scenes.pop();
    }

    m_settings.reset();
    m_sceneRegistry.clear();
    m_isInitialized = false;

    CT_LOG_INFO("SceneManager Shutdown.");
}

// Returns whether or not Scene Manager has been initialized.
bool SceneManager::IsInitialized() const
{
    return m_isInitialized;
}

// Performs internal state management during a single frame.
void SceneManager::Update(float dt)
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "Update");

    if (!m_scenes.empty())
    {
        auto &scene = m_scenes.top();
        scene->Update(dt);
    }
}

// Handle any internal logic that should be done relevant to the current scene.
void SceneManager::HandleEvent(const sf::Event &event)
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "HandleEvent");

    if (!m_scenes.empty())
    {
        m_scenes.top()->HandleEvent(event);
    }
}

// Render the necessary components to the current existing top Scene.
void SceneManager::Render()
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "Render");

    if (!m_scenes.empty())
    {
        m_scenes.top()->Render();
    }
}

// Registers a callback function for a scene by ID.
void SceneManager::Register(SceneID sceneId, SceneCreateFunc createFn)
{
    if (m_sceneRegistry.contains(sceneId))
    {
        CT_LOG_WARN("SceneManager: Scene '{}' is already registered!", ToString(sceneId));

        return;
    }

    m_sceneRegistry[sceneId] = std::move(createFn);

    CT_LOG_INFO("Scene '{}' registered with SceneManager.", ToString(sceneId));
}

// Return a safe pointer to a scene that is already registered by sceneId key, else nullptr.
std::unique_ptr<Scene> SceneManager::Create(SceneID sceneId)
{

    if (m_sceneRegistry.contains(sceneId))
    {
        CT_LOG_INFO("SceneManager: Create '{}'.", ToString(sceneId));

        return m_sceneRegistry[sceneId]();
    }

    CT_LOG_WARN("SceneManager::Create failed: '{}' is not registered.", ToString(sceneId));

    return nullptr;
}

// Grow with application development. Easily register all the game scenes to be retrieved and updated.
void SceneManager::RegisterAllDefaultScenes()
{
    Register(SceneID::Splash, [this]() { return std::make_unique<SplashScene>(m_settings); });
    Register(SceneID::MainMenu, [this]() { return std::make_unique<MainMenuScene>(m_settings); });
    Register(SceneID::Settings, [this]() { return std::make_unique<SettingsScene>(m_settings); });
    Register(SceneID::Game, [this]() { return std::make_unique<GameScene>(m_settings); });

    CT_LOG_INFO("All default scenes registered.");
}

// Initialize the requested scene, and place it on the top of the collection of scenes.
void SceneManager::PushScene(std::unique_ptr<Scene> scene)
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "PushScene");

    if (scene)
    {
        scene->Init();
        CT_LOG_INFO("Pushing new scene: {}", typeid(*scene).name());
        m_scenes.push(std::move(scene));
    }
}

// Remove the top scene from the collection of scenes.
void SceneManager::PopScene()
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "PopScene");

    if (!m_scenes.empty())
    {
        CT_LOG_INFO("Popping scene: {}", typeid(*m_scenes.top()).name());
        m_scenes.top()->Shutdown();
        m_scenes.pop();
    }
}

// Exposes a callback scenario where the current scene is removed, and then its new scene is added to the m_scenes list
void SceneManager::ReplaceScene(std::unique_ptr<Scene> newScene)
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "ReplaceScene");

    PopScene();
    PushScene(std::move(newScene));
}

// Remove all the scenes from the collection.
void SceneManager::ClearScenes()
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "ClearScenes");

    while (!m_scenes.empty())
    {
        m_scenes.top()->OnExit();
        m_scenes.top().reset();
        m_scenes.pop();
    }

    CT_LOG_INFO("All scenes cleared.");
}

// Returns the state of whether the scene collection is empty or not.
bool SceneManager::IsEmpty() const
{
    CT_WARN_IF_UNINITIALIZED_RET("SceneManager", "IsEmpty", false);

    return m_scenes.empty();
}

// Returns whether or not there is a currently active scene.
bool SceneManager::HasActiveScene() const
{
    CT_WARN_IF_UNINITIALIZED_RET("SceneManager", "HasActiveScene", false);

    return !m_scenes.empty();
}

// Returns the collection size of the number of existing scenes.
std::size_t SceneManager::GetSceneCount() const
{
    CT_WARN_IF_UNINITIALIZED_RET("SceneManager", "GetSceneCount", 0);

    return m_scenes.size();
}

// Returns a pointer to the currently active scene.
Scene *SceneManager::GetActiveScene() const
{
    CT_WARN_IF_UNINITIALIZED_RET("SceneManager", "GetActiveScene", nullptr);

    if (m_scenes.empty())
    {
        return nullptr;
    }

    return m_scenes.top().get();
}

// Request that a scene transition based on ID immediately take place.
void SceneManager::RequestSceneChange(SceneID id)
{
    if (!m_sceneRegistry.contains(id))
    {
        CT_LOG_WARN("SceneManager::RequestSceneChange: SceneID '{}' is not registered.", ToString(id));

        return;
    }

    auto nextScene = Create(id);

    if (nextScene)
    {
        ReplaceScene(std::move(nextScene));
    }
    else
    {
        CT_LOG_ERROR("SceneManager::RequestSceneChange failed to create scene!");
    }
}
