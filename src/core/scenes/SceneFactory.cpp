// ============================================================================
//  File        : SceneFactory.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-14
//  Description : Generation factory pattern for scenes
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "SceneFactory.h"

/// @brief Get the current Instance for this SceneFactory singleton.
/// @return reference to existing SceneFactory interface.
SceneFactory &SceneFactory::Instance()
{
    static SceneFactory instance;
    return instance;
}

/// @brief Load into SceneFactory memory the callback function associated with the id string.
/// @param sceneId Key value in the unordered map.
/// @param creator Pointer to a function that creates the Scene.
void SceneFactory::Register(const std::string &sceneId, CreatorFn creator)
{
    m_creators[sceneId] = std::move(creator);
}

/// @brief Generate a new Scene object using unique_ptr based on the requested id string.
/// @param sceneId Key value in the unordered map.
/// @return Unique pointer to a creator function that makes a Scene based from id.
std::unique_ptr<Scene> SceneFactory::Create(const std::string &sceneId)
{
    if (m_creators.contains(sceneId))
    {
        return m_creators[sceneId]();
    }

    return nullptr;
}
