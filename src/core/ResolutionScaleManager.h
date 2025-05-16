// ============================================================================
//  File        : ResolutionScaleManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-05
//  Description : A singleton helper class to work with the windows object,
//                and scale x, y references dynamically during runtime.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Settings.h"
#include <SFML/System.hpp>

// ============================================================================
//  Class       : ResolutionScaleManager
//  Purpose     : Singleton class that manages the scaling references during
//                window setting changes.
//
//  Responsibilities:
//      - Singleton permanence which updates scaleX and scaleY.
//      - Remembers previous and target window resolutions.
//
// ============================================================================
class ResolutionScaleManager
{
  public:
    static ResolutionScaleManager &Instance();

    void SetReferenceResolution(ResolutionSetting setting);
    void SetReferenceResolution(const sf::Vector2u &size);

    void SetCurrentResolution(ResolutionSetting setting);
    void SetCurrentResolution(const sf::Vector2u &size);

    float GetScaleX() const;
    float GetScaleY() const;
    float GetUniformScale() const;

    float ScaleX(float value) const;
    float ScaledReferenceX(float value) const;
    float ScaleY(float value) const;
    float ScaledReferenceY(float value) const;
    float ScaleUniform(float value) const;

    unsigned int ScaleFont(unsigned int size) const;

  private:
    ResolutionScaleManager() = default;
    ~ResolutionScaleManager() = default;

    ResolutionScaleManager(const ResolutionScaleManager &) = delete;
    ResolutionScaleManager &operator=(const ResolutionScaleManager &) = delete;

  private:
    sf::Vector2u m_referenceResolution{1920, 1080};
    sf::Vector2u m_currentResolution{1920, 1080};
};
