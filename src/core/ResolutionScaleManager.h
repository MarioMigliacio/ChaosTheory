

#pragma once

#include "Settings.h"
#include <SFML/System.hpp>

class ResolutionScaleManager
{
  public:
    static ResolutionScaleManager &Instance();

    // Set the reference design-time resolution (e.g., 1080p)
    void SetReferenceResolution(ResolutionSetting setting);
    void SetReferenceResolution(const sf::Vector2u &size);

    // Set the current actual resolution (e.g., 720p window)
    void SetCurrentResolution(ResolutionSetting setting);
    void SetCurrentResolution(const sf::Vector2u &size);

    // Retrieve scale factors
    float GetScaleX() const;
    float GetScaleY() const;
    float GetUniformScale() const;

    // Helpers to scale values
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
    sf::Vector2u m_referenceResolution{1920, 1080}; // default design-time baseline
    sf::Vector2u m_currentResolution{1920, 1080};
};
