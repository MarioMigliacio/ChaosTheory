// ============================================================================
//  File        : GameScene.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-14
//  Description : Hosts the definitions for Game Scene Object
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "GameScene.h"
#include "AssetManager.h"
#include "Assets.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "Macros.h"
#include "MainMenuScene.h"
#include "ResolutionScaleManager.h"
#include "SceneFactory.h"
#include "SceneTransitionManager.h"
#include "UIFactory.h"
#include "UIManager.h"
#include "UIPresets.h"
#include "WindowManager.h"

/// @brief Constructor for the GameScene.
/// @param settings Internal settings to initialize with.
GameScene::GameScene(std::shared_ptr<Settings> settings) : m_settings(settings)
{
}

/// @brief  Initializes the GameScene.
void GameScene::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    UIManager::Instance().Clear();

    LoadRequiredAssets();

    SceneTransitionManager::Instance().StartFadeIn();
    m_isInitialized = true;

    CT_LOG_INFO("GameScene initialized.");
}

/// @brief Load any required assets listed in the GameAssets namespace.
void GameScene::LoadRequiredAssets()
{
    CT_LOG_INFO("GameScene finished LoadRequiredAssets.");
}

/// @brief Shuts down this scene and resets internal state.
void GameScene::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("GameScene", "Shutdown");

    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("GameScene shutdown.");
}

/// @brief Handles the exit criteria for this scene.
void GameScene::OnExit()
{
    CT_LOG_INFO("GameScene OnExit.");
}

/// @brief Performs internal state management during a single frame.
/// @param dt delta time since last update.
void GameScene::Update(float dt)
{
}

/// @brief Handle any quick cancelation requests if present.
/// @param event bubbled down from caller, not needed.
void GameScene::HandleEvent(const sf::Event &event)
{
}

/// @brief Not used in GameScene context.
/// @param newSize bubbled down from caller, not needed.
void GameScene::OnResize(const sf::Vector2u &newSize)
{
}

/// @brief While this scene is active, render the necessary components.
void GameScene::Render()
{
}
