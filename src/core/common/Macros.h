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

///////////////////////////////////////////////
/// Universal Constants unrelated to any module
///////////////////////////////////////////////
/// @brief Simple math constant for PI.
constexpr float PI = 3.14159f;
///////////////////////////////////////////////

/// @brief Useful namespace exposing somewhat advanced math functions.
namespace CT_MATH
{
/// @brief Dot product between two directional vectors. Projects one vector onto another and scales that projection by
/// the magnitude of the second vector
/// @param a Vector 1.
/// @param b Vector 2.
/// @return Dot product of Vector a and b.
inline float Dot(const sf::Vector2f &a, const sf::Vector2f &b)
{
    return a.x * b.x + a.y * b.y;
}

/// @brief Length or magnitude of component vector.
/// @param v Vector to perform calculation on.
/// @return The Square of the components squared.
inline float Length(const sf::Vector2f &v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

/// @brief Translates an angle degree into Radians for computation.
/// @param d The degree to translate.
/// @return Radian amount from degree.
inline float DegToRad(float d)
{
    return d * PI / 180.f;
}

/// @brief (Player normalization) Return v normalized to unit length (or zero vector if |v| == 0).
/// @param v Vector to normalize.
/// @return Normalized unit vector.
/// @note If 0, return a default direction that favors the player.
inline sf::Vector2f Norm(const sf::Vector2f &v)
{
    float m = Length(v);

    return (m > 0.f) ? sf::Vector2f(v.x / m, v.y / m) : sf::Vector2f(0.f, -1.f);
}

/// @brief (Enemy normalization) Return v normalized to unit length (or zero vector if |v| == 0).
/// @param v Vector to normalize.
/// @return Normalized unit vector.
/// @note If 0, return a default direction that favors the Enemy.
inline sf::Vector2f Normalize(const sf::Vector2f &v)
{
    float m = Length(v);

    return (m > 0.f) ? sf::Vector2f(v.x / m, v.y / m) : sf::Vector2f(0.f, 1.f);
}
}; // namespace CT_MATH
