// ============================================================================
//  File        : UIManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-17
//  Description : UIManager is the CT library dedicated manager of
//                interactable user interface objects
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIElement.h"
#include <memory>
#include <vector>

// ============================================================================
//  Class       : UIManager
//  Purpose     : Handles the grouping logic for ui elements such as Buttons,
//                sliders, chatboxes, etc.
//
//  Responsibilities:
//      - Initializes and shuts down
//      - Generate UI Elements
//      - Group collected elements logic for update, render, dispose, etc.
//      - Display button specifics during render
//
// ============================================================================
class UIManager
{
  public:
    static UIManager &Instance();

    void Init();
    void Shutdown();

    bool IsInitialized() const;

    void AddElement(std::shared_ptr<UIElement> element);

    void Update(const sf::Vector2i &mousePos, bool isLeftClick);
    void Render(sf::RenderWindow &window);
    void Clear();

  private:
    UIManager() = default;
    ~UIManager() = default;

    UIManager(const UIManager &) = delete;
    UIManager &operator=(const UIManager &) = delete;

  private:
    std::vector<std::shared_ptr<UIElement>> m_elements;
    bool m_isInitialized = false;
};
