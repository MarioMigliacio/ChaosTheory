// ============================================================================
//  File        : ResolutionScaleManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-05-05
//  Description : A singleton helper class to work with the windows object,
//                and scale x, y references dynamically during runtime.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "ResolutionScaleManager.h"
#include "WindowManager.h"
#include <cmath>

/// @brief Get the current Instance for this ResolutionScaleManager singleton.
/// @return reference to existing ResolutionScaleManager interface.
ResolutionScaleManager &ResolutionScaleManager::Instance()
{
    static ResolutionScaleManager instance;
    return instance;
}

/// @brief Sets the internal referenceResolution from a ResolutionSetting enumeration object.
/// @param setting ResolutionSettings to set m_referenceResolution.
void ResolutionScaleManager::SetReferenceResolution(ResolutionSetting setting)
{
    SetReferenceResolution(WindowManager::Instance().GetResolutionSize(setting));
}

/// @brief Sets the internal referenceResolution from a coordinate pair.
/// @param size new m_referenceResolution.
void ResolutionScaleManager::SetReferenceResolution(const sf::Vector2u &size)
{
    m_referenceResolution = size;
}

/// @brief Sets the internal currentResolution from a ResolutionSetting enumeration object.
/// @param setting ResolutionSettings to set m_currentResolution.
void ResolutionScaleManager::SetCurrentResolution(ResolutionSetting setting)
{
    SetCurrentResolution(WindowManager::Instance().GetResolutionSize(setting));
}

/// @brief Sets the internal currentResolution from a coordinate pair.
/// @param size new m_currentResolution.
void ResolutionScaleManager::SetCurrentResolution(const sf::Vector2u &size)
{
    m_currentResolution = size;
}

/// @brief Returns a ratio of the current resolution with the target resolution.
/// @return scalar X value for window resize.
float ResolutionScaleManager::GetScaleX() const
{
    return static_cast<float>(m_currentResolution.x) / m_referenceResolution.x;
}

/// @brief Returns a ratio of the current resolution with the target resolution.
/// @return scalar Y value for window resize.
float ResolutionScaleManager::GetScaleY() const
{
    return static_cast<float>(m_currentResolution.y) / m_referenceResolution.y;
}

/// @brief Returns a minimal value of scalar x/y for entities that only need one way scaling.
/// @return minimum of (ScaleX, ScaleY).
float ResolutionScaleManager::GetUniformScale() const
{
    return std::min(GetScaleX(), GetScaleY());
}

/// @brief Multiplies value with the current Scalar X value.
/// @param value Value to recalculate.
/// @return Value scaled based on window resolution changes.
float ResolutionScaleManager::ScaleX(float value) const
{
    return value * GetScaleX();
}

/// @brief Used for window scalar reference values, i.e. those between 0.0 and 1.0 on a percentage based scale.
/// @param value reference X percentage.
/// @return An X coordinate on the window, based on the resolution and requested reference percentage.
float ResolutionScaleManager::ScaledReferenceX(float value) const
{
    return ScaleX(m_referenceResolution.x * value);
}

/// @brief Multiplies value with the current Scalar Y value.
/// @param value Value to recalculate.
/// @return Value scaled based on window resolution changes.
float ResolutionScaleManager::ScaleY(float value) const
{
    return value * GetScaleY();
}

/// @brief Used for window scalar reference values, i.e. those between 0.0 and 1.0 on a percentage based scale.
/// @param value reference Y percentage.
/// @return A Y coordinate on the window, based on the resolution and requested reference percentage.
float ResolutionScaleManager::ScaledReferenceY(float value) const
{
    return ScaleY(m_referenceResolution.y * value);
}

/// @brief Returns a uniformed scalar x/y value for entities that only need one way scaling.
/// @param value Value to recalculate.
/// @return Product of value and UniformScale.
float ResolutionScaleManager::ScaleUniform(float value) const
{
    return value * GetUniformScale();
}

/// @brief Useful when just scaling a font one direction.
/// @param size Current font size.
/// @return Uniformly scaled, and rounded font value.
unsigned int ResolutionScaleManager::ScaleFont(unsigned int size) const
{
    return static_cast<unsigned int>(std::round(size * GetUniformScale()));
}

/// @brief Provides easy access to the reference resolutions width.
/// @return m_referenceResolution.x
unsigned int ResolutionScaleManager::ReferenceResolutionX() const
{
    return m_referenceResolution.x;
}

/// @brief Provides easy access to the reference resolution height.
/// @return  m_referenceResolution.y
unsigned int ResolutionScaleManager::ReferenceResolutionY() const
{
    return m_referenceResolution.y;
}
