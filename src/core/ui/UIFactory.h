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

#include "Macros.h"
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

    std::shared_ptr<UIButton> CreateButton(const ButtonConfig &cfg);
    std::shared_ptr<UISelectableButton> CreateSelectableButton(const SelectableButtonConfig &cfg);
    std::shared_ptr<UISkinnableButton> CreateSkinnableButton(const SkinnableButtonConfig &cfg);
    std::shared_ptr<UISlider> CreateSlider(const SliderConfig &cfg);
    std::shared_ptr<UIArrow> CreateArrow(const ArrowConfig &cfg);
    std::shared_ptr<UIGroupBox> CreateGroupBox(const GroupBoxConfig &cfg);
    std::shared_ptr<UITextLabel> CreateTextLabel(const TextLabelConfig &cfg);
    std::shared_ptr<UIToastMessage> CreateToastMessage(const ToastMessageConfig &cfg);
    std::shared_ptr<UIHUDPanel> CreateHUDPanel(const HUDPanelConfig &cfg);
    std::shared_ptr<UIFillableGauge> CreateFillableGauge(const FillableGaugeConfig &cfg);
    std::shared_ptr<UIIcon> CreateIcon(const IconConfig &cfg);

  private:
    UIFactory() = default;
    ~UIFactory() = default;

    UIFactory(const UIFactory &) = delete;
    UIFactory &operator=(const UIFactory &) = delete;
};
