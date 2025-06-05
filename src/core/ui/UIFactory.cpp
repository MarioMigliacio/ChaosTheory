// ============================================================================
//  File        : UIFactory.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-17
//  Description : UIFactory is responsible for creating different types of
//                UI elements such as buttons, sliders, chatboxes, etc.
// ============================================================================

#include "UIFactory.h"
#include "AssetManager.h"
#include "Macros.h"
#include "ResolutionScaleManager.h"
#include "SettingsManager.h"

/// @brief Get the current Instance for this UIFactory singleton.
/// @return reference to existing UIFactory interface.
UIFactory &UIFactory::Instance()
{
    static UIFactory instance;
    return instance;
}

/// @brief Creates a UI Button element, given the custom input parameters.
/// @param position Button position.
/// @param size Button size.
/// @param label Button text with font and font size.
/// @param onClick Pointer to callback function, when clicked.
/// @return safe pointer to a UIButton.
std::shared_ptr<UIButton> UIFactory::CreateButton(const sf::Vector2f &position, const sf::Vector2f &size,
                                                  const std::string &label, std::function<void()> onClick)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(size.x),
                            ResolutionScaleManager::Instance().ScaleY(size.y));
    unsigned int scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(18);

    auto btn = std::make_shared<UIButton>(position, scaledSize);

    btn->SetText(label, *AssetManager::Instance().GetFont("Default"), scaledFontSize);
    btn->SetTextColor(BUTTON_DEFAULT_TEXT_COLOR);
    btn->SetCallback(std::move(onClick));
    btn->SetIdleColor(BUTTON_DEFAULT_IDLE_COLOR);
    btn->SetHoverColor(BUTTON_DEFAULT_HOVER_COLOR);
    btn->SetActiveColor(BUTTON_DEFAULT_ACTIVE_COLOR);

    btn->SetHoverScale(1.05f);

    return btn;
}

/// @brief Creates a UI Selectable Button element, given the custom input parameters.
/// @param position Button position.
/// @param size Button size.
/// @param label Button text with font and font size.
/// @param onClick Pointer to callback function, when clicked.
/// @return safe pointer to a UISelectableButton.
std::shared_ptr<UISelectableButton> UIFactory::CreateSelectableButton(const sf::Vector2f &position,
                                                                      const sf::Vector2f &size,
                                                                      const std::string &label,
                                                                      std::function<void()> onClick)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(size.x),
                            ResolutionScaleManager::Instance().ScaleY(size.y));
    unsigned int scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(18);

    auto btn = std::make_shared<UISelectableButton>(position, scaledSize);

    btn->SetText(label, *AssetManager::Instance().GetFont("Default"), scaledFontSize);
    btn->SetTextColor(BUTTON_DEFAULT_TEXT_COLOR);
    btn->SetCallback(std::move(onClick));
    btn->SetHoverColor(BUTTON_DEFAULT_HOVER_COLOR);
    btn->SetSelectedColor(BUTTON_DEFAULT_SELECTED_COLOR, BUTTON_DEFAULT_SELECTED_TEXT_COLOR);

    return btn;
}

/// @brief Generates a SkinnableButton ui element.
/// @param pos Position to set button at.
/// @param size Size to initialize button with.
/// @param label Text to initialize button with.
/// @param idle Path to Idle texture to be used for SkinnableButton
/// @param hover Path to Hover texture to be used for SkinnableButton
/// @param scheme Enum field representing the scheme for Button text layouts.
/// @param onClick Pointer to callback function, when clicked.
/// @return Smart pointer to a UISkinnableButton.
std::shared_ptr<UISkinnableButton> UIFactory::CreateSkinnableButton(const sf::Vector2f &pos, const sf::Vector2f &size,
                                                                    const std::string &label, const std::string &idle,
                                                                    const std::string &hover,
                                                                    UIButtonColorScheme scheme,
                                                                    std::function<void()> onClick)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(size.x),
                            ResolutionScaleManager::Instance().ScaleY(size.y));
    unsigned int scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(18);

    auto btn = std::make_shared<UISkinnableButton>(pos, scaledSize);

    btn->SetText(label, *AssetManager::Instance().GetFont("Default"), scaledFontSize);
    btn->SetTextureSkins(idle, hover);
    btn->SetCallback(onClick);

    ApplySkinnableButtonTextStyle(*btn, scheme);

    return btn;
}

/// @brief Creates a UI Slider element, given the custom input parameters.
/// @param label String representation for slider content.
/// @param position Slider position.
/// @param size Slider size.
/// @param minValue Slider minimum value.
/// @param maxValue Slider maximum value.
/// @param initialValue Slider default value.
/// @param onChange Pointer to callback function, when value changed.
/// @return safe pointer to a UISlider.
std::shared_ptr<UISlider> UIFactory::CreateSlider(const std::string &label, const sf::Vector2f &position,
                                                  const sf::Vector2f &size, float minValue, float maxValue,
                                                  float initialValue, std::function<void(float)> onChange)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    // We explicitely do scale the slider UI elements position because it impacts the slider background and knob
    const sf::Vector2f scaledPos = {scaleMgr.ScaledReferenceX(position.x), scaleMgr.ScaledReferenceY(position.y)};

    const auto scaledSize = sf::Vector2f(scaleMgr.ScaledReferenceX(size.x), scaleMgr.ScaleY(size.y));
    const auto scaledFontSize = ResolutionScaleManager::Instance().ScaleFont(14);

    auto slider = std::make_shared<UISlider>(label, minValue, maxValue, initialValue, scaledPos, scaledSize, onChange);
    slider->SetFont(*AssetManager::Instance().GetFont("Default"));
    slider->SetFontSize(scaledFontSize);
    slider->SetTitlePositionOffset(sf::Vector2f(0.f, -ResolutionScaleManager::Instance().ScaleY(24.f)));

    return slider;
}

/// @brief Creates a UI Arrow element, given the custom input parameters.
/// @param position Position to emplace.
/// @param size Size to initialize with.
/// @param texture Loadable texture path.
/// @param direction Arrow direction L, R, U, D.
/// @param onClick Pointer to callback function, when clicked.
/// @return Safe pointer to a UIArrow.
std::shared_ptr<UIArrow> UIFactory::CreateArrow(const sf::Vector2f &position, const sf::Vector2f &size,
                                                const std::string &texture, ArrowDirection direction,
                                                std::function<void()> onClick)
{
    sf::Vector2f scaledSize(ResolutionScaleManager::Instance().ScaleX(size.x),
                            ResolutionScaleManager::Instance().ScaleY(size.y));

    auto arrow = std::make_shared<UIArrow>(position, scaledSize, direction);
    arrow->SetOnClick(onClick);
    arrow->SetTextureSkin(texture);
    arrow->SetSize(scaledSize);

    return arrow;
}

/// @brief Creates a UI UIGroupBox element, given the custom input parameters.
/// @param title String representation for GroupBox.
/// @param relativePosition Screen relative position to be centered around.
/// @param relativeSize Screen relative size to occupy.
/// @return safe pointer to a UIGroupBox.
std::shared_ptr<UIGroupBox> UIFactory::CreateGroupBox(const std::string &title, const sf::Vector2f &relativePosition,
                                                      const sf::Vector2f &relativeSize)
{
    auto &scaleMgr = ResolutionScaleManager::Instance();

    const sf::Vector2f scaledPos{scaleMgr.ScaledReferenceX(relativePosition.x),
                                 scaleMgr.ScaledReferenceY(relativePosition.y)};
    const sf::Vector2f scaledSize{scaleMgr.ScaledReferenceX(relativeSize.x), scaleMgr.ScaledReferenceY(relativeSize.y)};

    const float internalPadding = scaleMgr.ScaledReferenceY(BASE_GROUPBOX_INTERNAL_PAD_RATIO);
    const float edgePadding = scaleMgr.ScaledReferenceY(BASE_GROUPBOX_EDGE_PAD_RATIO);

    auto groupBox = std::make_shared<UIGroupBox>(scaledPos, scaledSize);
    groupBox->SetTitle(title, *AssetManager::Instance().GetFont("Default"),
                       scaleMgr.ScaleFont(BASE_GROUPBOX_FONT_SIZE));
    groupBox->SetLayoutMode(LayoutMode::Vertical); // safe default state
    groupBox->SetCenterChildren(true);             // safe default state
    groupBox->SetInternalPadding(internalPadding);
    groupBox->SetEdgePadding(edgePadding);

    return groupBox;
}

/// @brief Creates a UI UITextLabel element, given the custom input parameters.
/// @param text String representation for the element.
/// @param position Position for text label.
/// @param fontSize Size of the font for the Text.
/// @param centerOrigin Used for centering the string contents around the position during construction.
/// @param scheme Enum field representing the type of shceme.
/// @return Safe pointer to a UITextLabel.
std::shared_ptr<UITextLabel> UIFactory::CreateTextLabel(const std::string &text, const sf::Vector2f &position,
                                                        unsigned int fontSize, bool centerOrigin,
                                                        UITextLabelScheme scheme)
{
    const float scaledOutline = ResolutionScaleManager::Instance().ScaleX(DEFAULT_TEXT_LABEL_BORDER_THICKNESS);

    auto label = std::make_shared<UITextLabel>(text, *AssetManager::Instance().GetFont("Default"), fontSize, position);

    if (!centerOrigin)
    {
        label->SetPosition(position); // no auto-centering
    }

    ApplyTextLabelStyle(*label, scheme, scaledOutline);

    return label;
}

/// @brief Creates a UI UIToastMessage element, given the custom input parameters.
/// @param text String representation for the toast message.
/// @param position Position for the toast message.
/// @param duration Duration for how long the toast message will exist.
/// @return safe pointer to a UIToastMessage.
std::shared_ptr<UIToastMessage> UIFactory::CreateToastMessage(const std::string &text, const sf::Vector2f &position,
                                                              float duration)
{
    const auto &font = *AssetManager::Instance().GetFont("Default");
    unsigned int fontSize = ResolutionScaleManager::Instance().ScaleFont(18);
    sf::Color color = sf::Color::White;
    bool centerOrigin = true;

    auto toast = std::make_shared<UIToastMessage>(text, position, duration, font, fontSize, color, centerOrigin);
    toast->SetSize({0.f, 0.f}); // still required by interface

    return toast;
}

/// @brief A private helper method to utilize color themes for a SkinnableButton combo.
/// @param button Reference to the button to be changed.
/// @param scheme Enum field representing  the type of scheme.
void UIFactory::ApplySkinnableButtonTextStyle(UISkinnableButton &button, UIButtonColorScheme scheme)
{
    switch (scheme)
    {
        case UIButtonColorScheme::Blue:
            button.SetTextStyle(TEX_BTN_BLUE_LABEL_TEXT_COLOR, TEX_BTN_BLUE_TEXT_OUTLINE_COLOR, 2.0f);
            break;

        case UIButtonColorScheme::Green:
            button.SetTextStyle(TEX_BTN_GREEN_LABEL_TEXT_COLOR, TEX_BTN_GREEN_TEXT_OUTLINE_COLOR, 2.0f);
            break;

        case UIButtonColorScheme::Red:
            button.SetTextStyle(TEX_BTN_RED_LABEL_TEXT_COLOR, TEX_BTN_RED_TEXT_OUTLINE_COLOR, 2.0f);
            break;
    }
}

/// @brief A private helper method to utilize color themes for a TextLabel string combo.
/// @param label Reference to the UITextLabel to be changed.
/// @param scheme Enum field representing the type of shceme.
void UIFactory::ApplyTextLabelStyle(UITextLabel &label, UITextLabelScheme scheme, const float labelBorderSize)
{
    switch (scheme)
    {
        case UITextLabelScheme::DefaultScheme:
        default:
            // Default scheme is LimeGreen with Purple contrast border.
            label.SetColor(TEXT_LABEL_COLOR_LIME_GREEN);
            label.SetOutline(labelBorderSize, TEXT_LABEL_COLOR_PURPLE_TINT);
            break;

        case UITextLabelScheme::CougarScheme:
            // CougarScheme scheme is Crimson with Grey contrast border.
            label.SetColor(TEXT_LABEL_COLOR_COUGAR_CRIMSON);
            label.SetOutline(labelBorderSize, TEXT_LABEL_COLOR_COUGAR_GREY);
            break;

        case UITextLabelScheme::HuskyScheme:
            // HuskyScheme scheme is PurpleTint with MetallicGold contrast border.
            label.SetColor(TEXT_LABEL_COLOR_PURPLE_TINT);
            label.SetOutline(labelBorderSize, TEXT_LABEL_COLOR_METALLIC_GOLD);
            break;

        case UITextLabelScheme::BlueSteelScheme:
            // BlueSteelScheme scheme is BlueSteel with CoolGrey contrast border.
            label.SetColor(TEXT_LABEL_COLOR_BLUE_STEEL);
            label.SetOutline(labelBorderSize, TEXT_LABEL_COLOR_COOL_GREY);
            break;

        case UITextLabelScheme::MintyHerbScheme:
            // MintyHerbScheme scheme is TealMint with Dark Green contrast border.
            label.SetColor(TEXT_LABEL_COLOR_TEAL_MINT);
            label.SetOutline(labelBorderSize, TEXT_LABEL_COLOR_MUTE_GREEN);
            break;
    }
}
