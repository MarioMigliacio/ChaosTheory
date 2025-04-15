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
    void Update(float /*dt*/) override
    {
    }
    void Render() override
    {
    }
    void OnExit() override
    {
    }
    void HandleEvent(const sf::Event & /*event*/) override
    {
    }
};