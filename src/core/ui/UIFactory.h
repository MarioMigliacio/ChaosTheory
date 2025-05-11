// ============================================================================
//  File        : UIFactory.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-17
//  Description : UIFactory is responsible for creating different types of
//                UI elements such as buttons, sliders, chatboxes, etc.
// ============================================================================

#pragma once

#include "UIArrow.h"
#include "UIButton.h"
#include "UIElement.h"
#include "UIGroupBox.h"
#include "UISelectableButton.h"
#include "UISlider.h"
#include "UITextLabel.h"
#include "UIToastMessage.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>

// Simple enumeration for supported UIButton types.
enum class ButtonType
{
    Classic,
    Radio
};

// ============================================================================
//  Class       : UIFactory
//  Purpose     : Singleton class that manages the generation of UI elements.
//
//  Responsibilities:
//      - Register callback functions to a scene index by string
//      - Create a unique pointer to a Scene based on id string
//
// ============================================================================
class UIFactory
{
  public:
    static UIFactory &Instance();

    std::shared_ptr<UIElement> CreateButton(ButtonType type, const sf::Vector2f &position, const sf::Vector2f &size,
                                            const std::string &label, std::function<void()> onClick);

    std::shared_ptr<UISlider> CreateSlider(const std::string &label, const sf::Vector2f &position,
                                           const sf::Vector2f &size, float minValue, float maxValue, float initialValue,
                                           std::function<void(float)> onChange);

    std::shared_ptr<UIArrow> CreateArrow(float x, float y, ArrowDirection direction, std::function<void()> onClick);

    // Creates a standard vertical UIGroupBox occupying relative screen space with automatic scaling.
    std::shared_ptr<UIGroupBox> CreateGroupBox(const std::string &title, const sf::Vector2f &relativePos,
                                               const sf::Vector2f &relativeSize);

    // Fully configurable UIGroupBox with layout, alignment, padding, and font size.
    std::shared_ptr<UIGroupBox> CreateGroupBox(const std::string &title, const sf::Vector2f &relativePosition,
                                               const sf::Vector2f &relativeSize, LayoutMode layoutMode,
                                               bool centerChildren, float internalPadRatio, float edgePadRatio,
                                               unsigned int fontSize);

    std::shared_ptr<UITextLabel> CreateTextLabel(const std::string &text, const sf::Vector2f &position,
                                                 unsigned int baseFontSize, bool centerOrigin);

    std::shared_ptr<UIToastMessage> CreateToastMessage(const std::string &text, const sf::Vector2f &position,
                                                       float duration);

  private:
    UIFactory() = default;
    ~UIFactory() = default;

    UIFactory(const UIFactory &) = delete;
    UIFactory &operator=(const UIFactory &) = delete;
};
