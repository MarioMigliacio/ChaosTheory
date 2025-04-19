// ============================================================================
//  File        : DummyScene.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : File used in the Scene Management tests
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Scene.h"
#include "Settings.h"
#include <memory>

class DummyScene : public Scene
{
  public:
    DummyScene() = default;
    ~DummyScene() override = default;

    void Init() override
    {
        m_isInitialized = true;
    }
    void Shutdown() override
    {
        m_isInitialized = false;
    }
    void OnExit() override
    {
    }
    bool IsInitialized() override
    {
        return m_isInitialized;
    }
    void Update(float /*dt*/) override
    {
    }
    void HandleEvent(const sf::Event & /*event*/) override
    {
    }
    void Render() override
    {
    }
    bool ShouldExit() override
    {
        return m_shouldExit;
    }
    void OnResize(const sf::Vector2u &newSize) override
    {
    }
};