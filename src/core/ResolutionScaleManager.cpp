

#include "ResolutionScaleManager.h"
#include "WindowManager.h"
#include <cmath>

ResolutionScaleManager &ResolutionScaleManager::Instance()
{
    static ResolutionScaleManager instance;
    return instance;
}

void ResolutionScaleManager::SetReferenceResolution(ResolutionSetting setting)
{
    SetReferenceResolution(WindowManager::Instance().GetResolutionSize(setting));
}

void ResolutionScaleManager::SetReferenceResolution(const sf::Vector2u &size)
{
    m_referenceResolution = size;
}

void ResolutionScaleManager::SetCurrentResolution(ResolutionSetting setting)
{
    SetCurrentResolution(WindowManager::Instance().GetResolutionSize(setting));
}

void ResolutionScaleManager::SetCurrentResolution(const sf::Vector2u &size)
{
    m_currentResolution = size;
}

float ResolutionScaleManager::GetScaleX() const
{
    return static_cast<float>(m_currentResolution.x) / m_referenceResolution.x;
}

float ResolutionScaleManager::GetScaleY() const
{
    return static_cast<float>(m_currentResolution.y) / m_referenceResolution.y;
}

float ResolutionScaleManager::GetUniformScale() const
{
    return std::min(GetScaleX(), GetScaleY());
}

float ResolutionScaleManager::ScaleX(float value) const
{
    return value * GetScaleX();
}

float ResolutionScaleManager::ScaledReferenceX(float value) const
{
    return ScaleX(m_referenceResolution.x * value);
}

float ResolutionScaleManager::ScaleY(float value) const
{
    return value * GetScaleY();
}

float ResolutionScaleManager::ScaledReferenceY(float value) const
{
    return ScaleY(m_referenceResolution.y * value);
}

float ResolutionScaleManager::ScaleUniform(float value) const
{
    return value * GetUniformScale();
}

unsigned int ResolutionScaleManager::ScaleFont(unsigned int size) const
{
    return static_cast<unsigned int>(std::round(size * GetUniformScale()));
}
