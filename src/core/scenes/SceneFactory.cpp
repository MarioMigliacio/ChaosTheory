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

SceneFactory &SceneFactory::Instance()
{
    static SceneFactory instance;
    return instance;
}

// Load into SceneFactory memory the callback function associated with the id string.
void SceneFactory::Register(const std::string &sceneId, CreatorFn creator)
{
    m_creators[sceneId] = std::move(creator);
}

// Generate a new Scene object using unique_ptr based on the requested id string.
std::unique_ptr<Scene> SceneFactory::Create(const std::string &sceneId)
{
    if (m_creators.contains(sceneId))
    {
        return m_creators[sceneId]();
    }

    return nullptr;
}
