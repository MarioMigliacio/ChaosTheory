// ============================================================================
//  File        : UIFactory.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-17
//  Description : UIFactory is responsible for creating different types of
//                UI elements such as Buttons, Sliders, GroupBoxes, etc.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "UIArrow.h"
#include "UIButton.h"
#include "UIElement.h"
#include "UIFillableGauge.h"
#include "UIGroupBox.h"
#include "UIHUDPanel.h"
#include "UIIcon.h"
#include "UISelectableButton.h"
#include "UISkinnableButton.h"
#include "UISlider.h"
#include "UITextLabel.h"
#include "UIToastMessage.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>

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

    std::shared_ptr<UIButton> CreateButton(const sf::Vector2f &position, const sf::Vector2f &size,
                                           const std::string &label, std::function<void()> onClick);

    std::shared_ptr<UISelectableButton> CreateSelectableButton(const sf::Vector2f &position, const sf::Vector2f &size,
                                                               const std::string &label, std::function<void()> onClick);

    std::shared_ptr<UISkinnableButton> CreateSkinnableButton(const sf::Vector2f &pos, const sf::Vector2f &size,
                                                             const std::string &label, const std::string &idle,
                                                             const std::string &hover, UIButtonColorScheme scheme,
                                                             std::function<void()> onClick);

    std::shared_ptr<UISlider> CreateSlider(const std::string &label, const sf::Vector2f &position,
                                           const sf::Vector2f &size, float minValue, float maxValue, float initialValue,
                                           std::function<void(float)> onChange);

    std::shared_ptr<UIArrow> CreateArrow(const sf::Vector2f &position, const sf::Vector2f &size,
                                         const std::string &texture, ArrowDirection direction,
                                         std::function<void()> onClick);

    std::shared_ptr<UIGroupBox> CreateGroupBox(const sf::Vector2f &relativePos, const sf::Vector2f &relativeSize,
                                               const std::string &title = "", bool centerOrigin = true,
                                               UITextLabelScheme scheme = UITextLabelScheme::DefaultScheme);

    std::shared_ptr<UITextLabel> CreateTextLabel(const std::string &text, const sf::Vector2f &position,
                                                 unsigned int baseFontSize, bool centerOrigin = true,
                                                 UITextLabelScheme scheme = UITextLabelScheme::DefaultScheme);

    std::shared_ptr<UIToastMessage> CreateToastMessage(const std::string &text, const sf::Vector2f &position,
                                                       float duration, unsigned int baseFontSize,
                                                       bool centerOrigin = true,
                                                       UITextLabelScheme scheme = UITextLabelScheme::DefaultScheme);

    std::shared_ptr<UIHUDPanel> CreateHUDPanel(const sf::Vector2f &relativePosition, const sf::Vector2f &relativeSize,
                                               const sf::Color &fillColor = sf::Color(64, 64, 64, 255),
                                               const sf::Color &outlineColor = sf::Color::Transparent,
                                               float outlineThickness = 0.f);

    std::shared_ptr<UIFillableGauge> CreateFillableGauge(const FillableGaugeConfig &cfg);

    std::shared_ptr<UIIcon> CreateIcon(const std::string &textureKey, const sf::Vector2f &position,
                                       const sf::Vector2f &size, IconType type = IconType::None);

  private:
    UIFactory() = default;
    ~UIFactory() = default;

    UIFactory(const UIFactory &) = delete;
    UIFactory &operator=(const UIFactory &) = delete;
};
