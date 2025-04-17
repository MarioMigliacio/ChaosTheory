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
#include "Macros.h"

SceneManager::SceneManager(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

// TODO: MARIO - The organization of the scenemanager h and cpp functions is correct, just need to add comments for
// each, and do same for scenefactory. then 1.1.1_scene-manager will be ready for readme updates and final touch.s
void SceneManager::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_isInitialized = true;

    CT_LOG_INFO("SceneManager Initialized.");
}

void SceneManager::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "Shutdown");

    while (!m_scenes.empty())
    {
        if (m_scenes.top())
        {
            m_scenes.top()->OnExit();   // Optional
            m_scenes.top()->Shutdown(); // Explicit call instead of relying on destructor
        }
        m_scenes.pop(); // smart pointer auto-deletes
    }

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("SceneManager Shutdown.");
}

void SceneManager::Update(float dt)
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "Update");

    if (!m_scenes.empty())
    {
        m_scenes.top()->Update(dt);
    }
}

void SceneManager::HandleEvent(const sf::Event &event)
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "HandleEvent");

    if (!m_scenes.empty())
    {
        m_scenes.top()->HandleEvent(event);
    }
}

void SceneManager::Render()
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "Render");

    if (!m_scenes.empty())
    {
        m_scenes.top()->Render();
    }
}

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

void SceneManager::PopScene()
{
    CT_WARN_IF_UNINITIALIZED("SceneManager", "PopScene");

    if (!m_scenes.empty())
    {
        CT_LOG_INFO("Popping scene: {}", typeid(*m_scenes.top()).name());
        m_scenes.pop();
    }
}

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

bool SceneManager::IsEmpty() const
{
    CT_WARN_IF_UNINITIALIZED_RET("SceneManager", "IsEmpty", false);

    return m_scenes.empty();
}

std::size_t SceneManager::GetSceneCount() const
{
    CT_WARN_IF_UNINITIALIZED_RET("SceneManager", "GetSceneCount", 1);

    return m_scenes.size();
}

Scene *SceneManager::GetActiveScene() const
{
    CT_WARN_IF_UNINITIALIZED_RET("SceneManager", "GetActiveScene", nullptr);

    if (m_scenes.empty())
    {
        return nullptr;
    }

    return m_scenes.top().get();
}

bool SceneManager::IsInitialized() const
{
    return m_isInitialized;
}
