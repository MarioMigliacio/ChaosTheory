// ============================================================================
//  File        : Macros.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-13
//  Description : Useful macros to reduce repetitiveness
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "LogManager.h"
#include "UIArrow.h"
#include "UIButton.h"
#include "UIChatBox.h"
#include "UIFillableGauge.h"
#include "UIGroupBox.h"
#include "UIHUDPanel.h"
#include "UIIcon.h"
#include "UISelectableButton.h"
#include "UISkinnableButton.h"
#include "UISlider.h"
#include "UITextLabel.h"
#include "UIToastMessage.h"

///////////////////////////////////////////////////////////////////////////////
//                          Logger related macros                            //
///////////////////////////////////////////////////////////////////////////////

#define CT_LOG_TRACE(...) LogManager::Instance().GetLogger()->trace(__VA_ARGS__)
#define CT_LOG_DEBUG(...) LogManager::Instance().GetLogger()->debug(__VA_ARGS__)
#define CT_LOG_INFO(...) LogManager::Instance().GetLogger()->info(__VA_ARGS__)
#define CT_LOG_WARN(...) LogManager::Instance().GetLogger()->warn(__VA_ARGS__)
#define CT_LOG_ERROR(...) LogManager::Instance().GetLogger()->error(__VA_ARGS__)
#define CT_LOG_CRITICAL(...) LogManager::Instance().GetLogger()->critical(__VA_ARGS__)

#define CF_EXIT_EARLY_IF_ALREADY_INITIALIZED()                                                                         \
    if (m_isInitialized)                                                                                               \
    {                                                                                                                  \
        return;                                                                                                        \
    }

#define CT_WARN_IF_UNINITIALIZED(context, task)                                                                        \
    if (!m_isInitialized)                                                                                              \
    {                                                                                                                  \
        CT_LOG_WARN("{}: Attempted to {} without initialization!", context, task);                                     \
        return;                                                                                                        \
    }

#define CT_WARN_IF_UNINITIALIZED_RET(context, task, retval)                                                            \
    if (!m_isInitialized)                                                                                              \
    {                                                                                                                  \
        CT_LOG_WARN("{}: Attempted to {} without initialization!", context, task);                                     \
        return retval;                                                                                                 \
    }

///////////////////////////////////////////////////////////////////////////////
//                              UI related macros                            //
///////////////////////////////////////////////////////////////////////////////

/// @brief Initializes an ArrowConfig structure to be handled by UIFactory for construction.
/// @param pos Vector2f position for Arrow.
/// @param size Vector2f size for Arrow.
/// @param textureKey string texture key for Arrow.
/// @param directionEnum ArrowDirection direction for Arrow.
/// @param callbackFunc function callback to trigger for Arrow.
#define INIT_ARROW_CONFIG(pos, size, textureKey, directionEnum, callbackFunc)                                          \
    ArrowConfig                                                                                                        \
    {                                                                                                                  \
        pos, size, textureKey, directionEnum, callbackFunc                                                             \
    }

/// @brief Initializes a Button structure to be handled by UIFactory for construction.
/// @param pos Vector2f position for Button.
/// @param size Vector2f size for Button.
/// @param label string Label for Button.
/// @param callbackFunc function callback to trigger for Button.
#define INIT_BUTTON_CONFIG(pos, size, label, callbackFunc)                                                             \
    ButtonConfig                                                                                                       \
    {                                                                                                                  \
        pos, size, label, callbackFunc                                                                                 \
    }

/// @brief Initializes a default fillable gauge structure to be handled by UIFactory for construction.
/// @note Default configuration does not use a Title, has no border, and doesn't display percentage.
/// @param pos Vector2f relative screen position for the gauge.
/// @param size Vector2f relative screen size for the gauge.
/// @param orientation Uses [default horizontal]
/// @param colorScheme Uses [default default color scheme]
/// @param initialValue Uses [default 1.f]
/// @param borderThickness Uses [default 0.f (none)]
/// @param borderColor Uses [default transparent (none)]
/// @param showPercentage Uses [default false]
/// @param showTitle Uses [default false]
/// @param titleText Uses [default (empty)]
/// @param titleFontSize Uses [default 14U]
/// @param titleScheme Uses [default default color scheme]
/// @param titlePositionEnum Uses [default left position]
/// @param titlePadding Uses [default 8.f]
#define INIT_DEFAULT_FILLABLE_GAUGE_CONFIG(pos, size)                                                                  \
    FillableGaugeConfig                                                                                                \
    {                                                                                                                  \
        pos, size                                                                                                      \
    }

/// @brief Initializes a fully customized fillable gauge structure to be handled by UIFactory for construction.
/// @note Custom configuration Allows for a Title, border, and display percentage.
/// @param pos Vector2f relative screen position for the gauge.
/// @param size Vector2f relative screen size for the gauge.
/// @param orientation LayoutMode layout mode (horizontal or vertical) for gauge fill direction. [default horizontal]
/// @param colorScheme GaugeColorScheme specifying the visual color scheme of the gauge. [default default color scheme]
/// @param initialValue Float representing the starting fill level of the gauge (range: 0.0–1.0). [default 1.f]
/// @param borderThickness Float representing thickness of the gauge border. [default 0.f (none)]
/// @param borderColor sf::Color value for the border color of the gauge. [default transparent (none)]
/// @param showPercentage Boolean toggle to render percentage overlay on the gauge. [default false]
/// @param showTitle Boolean toggle to render a title label alongside the gauge. [default false]
/// @param titleText String label displayed when title is enabled. [default (empty)]
/// @param titleFontSize Font size used when title is enabled. [default 14U]
/// @param titleScheme UITextLabelScheme defining the text color style for the title. [default default color scheme]
/// @param titlePosition GaugeTitlePosition defining title appearance. [default left position]
/// @param titlePadding Float amount of padding between title and gauge body. [default 8.f]
#define INIT_CUSTOM_FILLABLE_GAUGE_CONFIG(pos, size, orientation, colorScheme, initialValue, borderThickness,          \
                                          borderColor, showPercentage, showTitle, titleText, titleFontSize,            \
                                          titleScheme, titlePosition, titlePadding)                                    \
    FillableGaugeConfig                                                                                                \
    {                                                                                                                  \
        pos, size, orientation, colorScheme, initialValue, borderThickness, borderColor, showPercentage, showTitle,    \
            titleText, titleFontSize, titleScheme, titlePosition, titlePadding                                         \
    }

/// @brief Initializes a Group Box structure to be handled by UIFactory for construction.
/// @param pos Vector2f position for Group Box.
/// @param size Vector2f size for Group Box.
/// @param useTitle bool should title be used for Group Box [default false].
/// @param title string title label for Group Box [default = empty].
/// @param centerOrigin bool should center origin for Group Box title [default true].
/// @param schemeEnum UITextLabelScheme Color Scheme for Group Box [default default color scheme].
#define INIT_GROUPBOX_CONFIG(pos, size, useTitle, title, centerOrigin, schemeEnum)                                     \
    GroupBoxConfig                                                                                                     \
    {                                                                                                                  \
        pos, size, useTitle, title, centerOrigin, schemeEnum                                                           \
    }

/// @brief Initializes a HUD Panel structure to be handled by UIFactory for construction.
/// @param pos Vector2f position for HUD Panel.
/// @param size Vector2f size for HUD Panel.
/// @param fillColor Color Label for HUD Panel [default DEFAULT_HUD_COLOR grey].
/// @param outlineColor Color Label for HUD Panel [default DEFAULLT_HUD_BACKGROUND_COLOR transparent].
/// @param outlineThickness float outline thickness for HUD Panel [default 0.f].
#define INIT_HUDPANEL_CONFIG(pos, size, fillColor, outlineColor, outlineThickness)                                     \
    HUDPanelConfig                                                                                                     \
    {                                                                                                                  \
        pos, size, fillColor, outlineColor, outlineThickness                                                           \
    }

/// @brief Initializes a Icon Config structure to be handled by UIFactory for construction.
/// @param pos Vector2f position for Icon Config.
/// @param size Vector2f size for Icon Config.
/// @param textureKey String texture key for Icon Config.
/// @param iconTypeEnum IconType type for Icon Config.
#define INIT_ICON_CONFIG(pos, size, textureKey, iconTypeEnum)                                                          \
    IconConfig                                                                                                         \
    {                                                                                                                  \
        pos, size, textureKey, iconTypeEnum                                                                            \
    }

/// @brief Initializes a Selectable Button structure to be handled by UIFactory for construction.
/// @param pos Vector2f position for Selectable Button.
/// @param size Vector2f size for Selectable Button.
/// @param label string Label for Selectable Button.
/// @param callbackFunc function callback to trigger for Selectable Button.
#define INIT_SELECTABLE_BUTTON_CONFIG(pos, size, label, callbackFunc)                                                  \
    SelectableButtonConfig                                                                                             \
    {                                                                                                                  \
        pos, size, label, callbackFunc                                                                                 \
    }

/// @brief Initializes a SinnableButtonConfig structure to be handled by UIFactory for construction.
/// @param pos Vector2f position for Skinnable Button.
/// @param size Vector2f size for Skinnable Button.
/// @param label string Label for Skinnable Button.
/// @param idleTexture string idle texture for Skinnable Button.
/// @param hoverTexture string hover texture for Skinnable Button.
/// @param schemeEnum UISkinnableButtonColorScheme color scheme combination for Skinnable Button.
/// @param callbackFunc function callback to trigger for Skinnable Button.
#define INIT_SKINNABLE_BUTTON_CONFIG(pos, size, label, idleTexture, hoverTexture, schemeEnum, callbackFunc)            \
    SkinnableButtonConfig                                                                                              \
    {                                                                                                                  \
        pos, size, label, idleTexture, hoverTexture, schemeEnum, callbackFunc                                          \
    }

/// @brief Initializes a SliderConfig structure to be handled by UIFactory for construction.
/// @param label string Label for Slider.
/// @param pos Vector2f position for Slider.
/// @param size Vector2f size for Slider.
/// @param minVal float minimum value for Slider.
/// @param maxVal float maximum value for Slider.
/// @param initialVal float initial value for Slider.
/// @param callbackFunc function callback to trigger for Slider.
#define INIT_SLIDER_CONFIG(label, pos, size, minVal, maxVal, initialVal, callbackFunc)                                 \
    SliderConfig                                                                                                       \
    {                                                                                                                  \
        label, pos, size, minVal, maxVal, initialVal, callbackFunc                                                     \
    }

/// @brief Initializes a TextLabelConfig structure to be handled by UIFactory for construction.
/// @param text String Label for Text Label.
/// @param pos Vector2f position for Text Label.
/// @param fontSize unsigned int font size for Text Label.
/// @param centerOrigin bool should center text origin for Text Label [default true].
/// @param schemeEnum UITextLabelScheme Color Scheme for Text Label [default default color scheme].
#define INIT_TEXTLABEL_CONFIG(text, pos, fontSize, centerOrigin, schemeEnum)                                           \
    TextLabelConfig                                                                                                    \
    {                                                                                                                  \
        text, pos, fontSize, centerOrigin, schemeEnum                                                                  \
    }

/// @brief Initializes a ToastConfig structure to be handled by UIFactory for construction.
/// @param text String Label for Toast.
/// @param pos Vector2f position for Toast.
/// @param durationSec float duration for Toast.
/// @param fontSize unsigned int original font size for Toast.
/// @param centerOrigin bool should center text origin for Toast.
/// @param schemeEnum UITextLabelScheme Color Scheme for Toast.
#define INIT_TOAST_CONFIG(text, pos, durationSec, fontSize, centerOrigin, schemeEnum)                                  \
    ToastMessageConfig                                                                                                 \
    {                                                                                                                  \
        text, pos, durationSec, fontSize, centerOrigin, schemeEnum                                                     \
    }

/// @brief Initializes a ChatBox structure to be handled by UIFactory for construction.
/// @param pos Vector2f position for ChatBox.
/// @param size Vector2f size for ChatBox.
/// @param showTitle Bool to toggle title [default false (title disabled)].
/// @param title Optional string for title label [default empty string].
/// @param typeSpeed float Seconds per character for typewriter effect [default 0.05f].
/// @param textScheme Scheme for chat text [default default color scheme].
/// @param titleScheme Scheme for title text [default default color scheme].
/// @param useIcon bool to toggle usage of Icon for ChatBox speaker.
/// @param iconKey string path to asset key for icon if applicable.
/// @param iconType IconType enum for type of icon.
#define INIT_CHATBOX_CONFIG(pos, size, showTitle, title, typeSpeed, textScheme, titleScheme, useIcon, iconKey,         \
                            iconType)                                                                                  \
    ChatBoxConfig                                                                                                      \
    {                                                                                                                  \
        pos, size, showTitle, title, typeSpeed, textScheme, titleScheme, useIcon, iconKey, iconType                    \
    }
