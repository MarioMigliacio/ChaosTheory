// ============================================================================
//  File        : SceneFactory.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-14
//  Description : Hosts the definition for scene architecture and logic
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

void SceneFactory::Register(const std::string &sceneId, CreatorFn creator)
{
    m_creators[sceneId] = std::move(creator);
}

std::unique_ptr<Scene> SceneFactory::Create(const std::string &sceneId)
{
    if (auto it = m_creators.find(sceneId); it != m_creators.end())
    {
        return it->second();
    }

    return nullptr;
}
