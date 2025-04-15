// ============================================================================
//  File        : SceneFactory.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-14
//  Description : Generation factory pattern for scenes
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Scene.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class SceneFactory
{
  public:
    using CreatorFn = std::function<std::unique_ptr<Scene>()>;

    static SceneFactory &Instance();

    void Register(const std::string &sceneId, CreatorFn creator);
    std::unique_ptr<Scene> Create(const std::string &sceneId);

  private:
    std::unordered_map<std::string, CreatorFn> m_creators;

    SceneFactory() = default;
    ~SceneFactory() = default;
};
