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

constexpr float GROUPBOX_DEFAULT_INTERNAL_PAD = 12.f;
constexpr float GROUPBOX_DEFAULT_EDGE_PAD = 10.f;