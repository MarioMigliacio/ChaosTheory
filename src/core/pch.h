// ============================================================================
//  File        : pch.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Precompiled header to speed up compilation
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

// ==== Standard Library Includes ====
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// ==== SFML ====
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// ==== External Libraries ====
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// ==== Common Project Headers ====
#include "Assets.h"
#include "Macros.h"
#include "Settings.h"
#include "UIPresets.h"
