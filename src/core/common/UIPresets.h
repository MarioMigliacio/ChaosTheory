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

// Magic Strings:

// Titles:
const std::string DEFAULT_TITLE_STR = "Chaos Theory";
const sf::Color DEFAULT_TITLE_COLOR(102, 255, 102);
constexpr unsigned int DEFAULT_TITLE_FONT_SIZE = 48;
constexpr float DEFAULT_TITLE_HEIGHT_PERCENT = .1f;

// Generic Button uses:
const sf::Color BUTTON_DEFAULT_IDLE_COLOR(200, 200, 200);
const sf::Color BUTTON_DEFAULT_HOVER_COLOR(160, 160, 255);
const sf::Color BUTTON_DEFAULT_ACTIVE_COLOR(100, 100, 255);
const sf::Color BUTTON_DEFAULT_DISABLED_IDLE_COLOR(100, 100, 100);
const sf::Color BUTTON_DEFAULT_DISABLED_HOVER_COLOR(100, 100, 100);
const sf::Color BUTTON_DEFAULT_TEXT_COLOR(0, 0, 0);
const sf::Color BUTTON_DEFAULT_SELECTED_COLOR(100, 180, 255);
const sf::Color BUTTON_DEFAULT_SELECTED_TEXT_COLOR(255, 255, 255);
const sf::Color BUTTON_DEFAULT_DISABLED_TEXT_COLOR(200, 200, 200);
constexpr unsigned int BUTTON_DEFAULT_FONT_SIZE = 24;

constexpr float BASE_BUTTON_WIDTH_PIXEL = 140.f;
constexpr float BASE_BUTTON_HEIGHT_PIXEL = 30.f;
constexpr float BASE_BUTTON_SPACING_PERCENT = .25f;
constexpr float BASE_BUTTON_SPACING_PIXEL = 20.f;

constexpr float BASE_FOOTER_HEIGHT_85_PERCENT = .85f;
constexpr float BASE_FOOTER_WIDTH_75_PERCENT = .75f;

// Generic Slider uses:
constexpr float BASE_SLIDER_WIDTH_PIXEL = 300.f;
constexpr float BASE_SLIDER_HEIGHT_PIXEL = 20.f;
constexpr float SLIDER_SPACING_PIXEL = 50.f;
constexpr float SLIDER_SPACING_PERCENT = .05f;
constexpr float BASE_SLIDER_WIDTH_PERCENT = .45f;

// Generic GroupBox uses:
constexpr float GROUPBOX_DEFAULT_INTERNAL_PAD = 12.f;
constexpr float GROUPBOX_DEFAULT_EDGE_PAD = 10.f;

// Generic Toast uses:
constexpr float TOAST_DURATION = 2.f;
