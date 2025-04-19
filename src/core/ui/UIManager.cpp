// ============================================================================
//  File        : UIManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-17
//  Description : UIManager is the CT library dedicated manager of
//                interactable user interface objects
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "UIManager.h"
#include "LogManager.h"
#include "Macros.h"

UIManager &UIManager::Instance()
{
    static UIManager instance;
    return instance;
}

// Initializes the UI manager.
void UIManager::Init()
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_isInitialized = true;

    CT_LOG_INFO("UIManager initialized.");
}

// Shuts down the UI manager and resets internal state.
void UIManager::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("UIManager", "Shutdown");

    Clear();
    m_isInitialized = false;

    CT_LOG_INFO("UIManager shutdown.");
}

// Returns whether or not the UI manager is initialized.
bool UIManager::IsInitialized() const
{
    return m_isInitialized;
}

// Pushes a new ui element to the UI managers collection of elements.
void UIManager::AddElement(std::shared_ptr<UIElement> element)
{
    CT_WARN_IF_UNINITIALIZED("UIManager", "AddElement");

    m_elements.push_back(std::move(element));
}

// Performs collected Update logic for any UI components this UI manager handles.
void UIManager::Update(const sf::Vector2i &mousePos, bool isLeftClick)
{
    CT_WARN_IF_UNINITIALIZED("UIManager", "Update");

    for (auto &element : m_elements)
    {
        element->Update(mousePos, isLeftClick);
    }
}

// Performs collected Draw logic for any UI components this UI manager handles.
void UIManager::Render(sf::RenderWindow &window)
{
    CT_WARN_IF_UNINITIALIZED("UIManager", "Draw");

    for (auto &element : m_elements)
    {
        window.draw(*element);
    }
}

// Performs collected Clear logic, useful for the shutting down of smart object collectables.
void UIManager::Clear()
{
    CT_WARN_IF_UNINITIALIZED("UIManager", "Clear");

    m_elements.clear();
}
