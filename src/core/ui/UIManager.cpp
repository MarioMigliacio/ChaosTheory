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

const std::vector<std::shared_ptr<UIElement>> &UIManager::GetElements() const
{
    return m_elements;
}

// Blocks any pending actions from impacting the UI for a brief logic check.
void UIManager::BlockInputUntilMouseRelease()
{
    m_blockedUntilMouseRelease = true;
}

// Performs collected Update logic for any UI components this UI manager handles.
void UIManager::Update(const sf::Vector2i &mousePos, bool isLeftClick)
{
    CT_WARN_IF_UNINITIALIZED("UIManager", "Update");

    if (m_blockedUntilMouseRelease)
    {
        if (!isLeftClick)
        {
            m_blockedUntilMouseRelease = false;
        }

        else
        {
            return;
        }
    }

    m_isUpdating = true;

    for (auto &element : m_elements)
    {
        element->Update(mousePos, isLeftClick);
    }

    m_isUpdating = false;

    if (m_pendingClear)
    {
        Clear();
        m_pendingClear = false;
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

    if (m_isUpdating)
    {
        m_pendingClear = true;

        return;
    }

    m_elements.clear();
}
