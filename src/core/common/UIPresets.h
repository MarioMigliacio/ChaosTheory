// ============================================================================
//  File        : UIPresets.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-07
//  Description : Contains useful constants for UI management
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// ============================================================================
// Titles:
// ============================================================================

/// @brief Default Main Menu Title string.
constexpr auto DEFAULT_TITLE_STR = "Chaos Theory";

/// @brief Default Settings page title string.
constexpr auto DEFAULT_SETTINGS_STR = "Settings";

/// @brief Default Title string color.
const sf::Color DEFAULT_TITLE_COLOR = sf::Color(102, 255, 102);

/// @brief Default Title font size.
constexpr unsigned int DEFAULT_TITLE_FONT_SIZE = 48;

/// @brief Default relative positioning for Title string.
constexpr float DEFAULT_TITLE_HEIGHT_PERCENT = .1f;

// ============================================================================
// Generic Button uses:
// ============================================================================

/// @brief Color impacts the idle buttons color.
const sf::Color BUTTON_DEFAULT_IDLE_COLOR = sf::Color(200, 200, 200);

/// @brief Color impacts the hovered buttons color.
const sf::Color BUTTON_DEFAULT_HOVER_COLOR = sf::Color(160, 160, 255);

/// @brief Color impacts the buttons color while in active state.
const sf::Color BUTTON_DEFAULT_ACTIVE_COLOR = sf::Color(100, 100, 255);

/// @brief Color impacts the idle buttons color while in disabled state.
const sf::Color BUTTON_DEFAULT_DISABLED_IDLE_COLOR = sf::Color(100, 100, 100);

/// @brief Color impacts the hovered buttons color while in disabled state.
const sf::Color BUTTON_DEFAULT_DISABLED_HOVER_COLOR = sf::Color(100, 100, 100);

/// @brief Color impacts the buttons text color.
const sf::Color BUTTON_DEFAULT_TEXT_COLOR = sf::Color(0, 0, 0);

/// @brief Color impacts the selectable buttons color while in selected state.
const sf::Color BUTTON_DEFAULT_SELECTED_COLOR = sf::Color(100, 180, 255);

/// @brief Color impacts the selectable buttons text color while in selected state.
const sf::Color BUTTON_DEFAULT_SELECTED_TEXT_COLOR = sf::Color(255, 255, 255);

/// @brief Color impacts the buttons text color while in disabled state.
const sf::Color BUTTON_DEFAULT_DISABLED_TEXT_COLOR = sf::Color(200, 200, 200);

/// @brief Default button font size.
constexpr unsigned int BUTTON_DEFAULT_FONT_SIZE = 24;

/// @brief Main Menu button width in pixels.
constexpr float MAIN_MENU_BUTTON_WIDTH_PIXEL = 180.f;

/// @brief Main Menu button height in pixels.
constexpr float MAIN_MENU_BASE_BUTTON_HEIGHT_PIXEL = 45.f;

/// @brief Generic button width in pixels.
constexpr float BASE_BUTTON_WIDTH_PIXEL = 140.f;

/// @brief Generic button height in pixels.
constexpr float BASE_BUTTON_HEIGHT_PIXEL = 30.f;

/// @brief Generic button width in relative screen sizing, 20%.
constexpr float BASE_BUTTON_WIDTH_20_PERCENT = .20f;

/// @brief Generic button height in relative screen sizing, 5%.
constexpr float BASE_BUTTON_HEIGHT_5_PERCENT = .05f;

/// @brief Generic button spacing in relative screen sizing, 25%.
constexpr float BASE_BUTTON_SPACING_PERCENT = .25f;

/// @brief Generic button spacing in pixels.
constexpr float BASE_BUTTON_SPACING_PIXEL = 20.f;

/// @brief Footer location at relative 85% of screen height.
constexpr float BASE_FOOTER_HEIGHT_85_PERCENT = .85f;

/// @brief Footer location at relative 75% of screen width.
constexpr float BASE_FOOTER_WIDTH_75_PERCENT = .75f;

// ============================================================================
// Generic Slider uses:
// ============================================================================

/// @brief Generic slider width in pixels.
constexpr float BASE_SLIDER_WIDTH_PIXEL = 300.f;

/// @brief Generic slider height in pixels.
constexpr float BASE_SLIDER_HEIGHT_PIXEL = 20.f;

/// @brief Generic slider spacing in pixels.
constexpr float BASE_SLIDER_SPACING_PIXEL = 50.f;

/// @brief Generic slider spacing in relative screen sizing, 5%.
constexpr float BASE_SLIDER_SPACING_PERCENT = .05f;

/// @brief Generic slider width in relative screen sizing, 45%.
constexpr float BASE_SLIDER_WIDTH_PERCENT = .45f;

/// @brief Generic slider knob radius for default constructing.
constexpr float BASE_SLIDER_KNOB_RADIUS = 6.f;

/// @brief Generic fill color for defaullt constructing.
const sf::Color BASE_SLIDER_FILL_COLOR = sf::Color(200, 200, 200);

/// @brief Generic color for the background for default constructing.
const sf::Color BASE_SLIDER_BACK_COLOR = sf::Color(100, 100, 100);

/// @brief Generic color for the knob for default constructing.
const sf::Color BASE_SLIDER_KNOB_COLOR = sf::Color(102, 255, 102);

// ============================================================================
// Generic GroupBox uses:
// ============================================================================

/// @brief Default constructed groupbox fill color.
const sf::Color DEFAULT_GROUPBOX_FILL_COLOR = sf::Color(25, 25, 25, 160);

/// @brief Default constructed groupbox outline color.
const sf::Color DEFAULT_GROUPBOX_OUTLINE_COLOR = sf::Color(150, 255, 150, 200);

/// @brief Default groupbox padding between elements in pixels.
constexpr float DEFAULT_GROUPBOX_INTERNAL_PAD = 12.f;

/// @brief Default groupbox spacing between edge in pixels.
constexpr float DEFAULT_GROUPBOX_EDGE_PAD = 10.f;

/// @brief Default constructed groupbox outline edge thickness.
constexpr float DEFAULT_GROUPBOX_OUTLINE_THICKNESS = 1.f;

/// @brief When constructed in the factory, placeholder internal padding is safe.
constexpr float BASE_GROUPBOX_INTERNAL_PAD_RATIO = 0.1f;

/// @brief When constructed in the factory, placeholder edge pad is safe.
constexpr float BASE_GROUPBOX_EDGE_PAD_RATIO = 0.075f;

/// @brief When constructed in the factory, placeholder font is safe.
constexpr unsigned int BASE_GROUPBOX_FONT_SIZE = 24;

// ============================================================================
// Generic Toast uses:
// ============================================================================

/// @brief General use default time for a toast message to expire.
constexpr float TOAST_DEFAULT_DURATION = 2.f;

/// @brief General use default time for a toast message fade effect.
constexpr float TOAST_DEFAULT_FADE_DURATION = 1.f;

/// @brief General use distance a toast message will travel while active.
constexpr float TOAST_DEFAULT_DRIFT_PERCENTAGE = .05f;

/// @brief General use default color for a toast message.
const sf::Color TOAST_DEFAULT_COLOR(102, 255, 102);

// ============================================================================
// Generic Arrow uses:
// ============================================================================

/// @brief Default relative space from left boundary for an Arrow, 5%.
constexpr float DEFAULT_ARROW_LEFT_CENTER_PERCENT = .05f;

/// @brief Default relative space from top boundary for an Arrow, 5%.
constexpr float DEFAULT_ARROW_TOP_CENTER_PERCENT = .05f;

/// @brief Default relative space from right boundary for an Arrow, 5%.
constexpr float DEFAULT_ARROW_RIGHT_CENTER_PERCENT = .95f;

/// @brief Default relative space from left boundary for an Arrow, 5%.
constexpr float DEFAULT_ARROW_BOTTOM_CENTER_PERCENT = .95f;
