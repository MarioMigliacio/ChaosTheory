// ============================================================================
//  File        : BaseShip.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-17
//  Description : Abstract base extraction for a Ship type object interface.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "BaseShip.h"
#include "WindowManager.h"

/// @brief Implements BaseCollidable::IsAlive().
/// @return m_alive state, true / false.
bool BaseShip::IsAlive() const
{
    return m_alive;
}

/// @brief Implements BaseCollidable::GetBounds().
/// @return Bounds for this ships rectangle.
sf::FloatRect BaseShip::GetBounds() const
{
    return m_sprite.getGlobalBounds();
}

/// @brief Implements BaseCollidable::GetCollisionCategory().
/// @return CollisionCategory for which this ship will obey collision rules to.
CollisionCategory BaseShip::GetCollisionCategory() const
{
    return (m_allegiance == Allegiance::Player) ? CollisionCategory::Player : CollisionCategory::Enemy;
}

/// @brief Draw the ship if it is alive.
/// @param target Render target to draw to.
void BaseShip::Draw(sf::RenderTarget &target)
{
    if (m_alive)
    {
        target.draw(m_sprite);
    }
}

/// @brief Return the health of the ship.
/// @return m_health.
int BaseShip::GetHealth() const
{
    return m_health;
}

/// @brief Set ship health.
/// @param maxHealth health to set the current ship health to.
void BaseShip::SetHealth(const int maxHealth)
{
    m_health = maxHealth;
}

/// @brief Gets the max health.
/// @return m_maxHealth.
int BaseShip::GetMaxHealth() const
{
    return m_maxHealth;
}

/// @brief Apply damage and check for death.
/// @param amount Damage to receive.
void BaseShip::TakeDamage(const float amount)
{
    m_health -= amount;

    if (m_health <= 0)
    {
        m_health = 0;
        m_alive = false;

        CT_LOG_DEBUG("Ship has taken catastrophic damage and died.");
    }
}

/// @brief If the bounds of this Ship are outside the screen window, mark it for cleanup.
void BaseShip::CullIfOffscreen()
{
    const auto windowSize = WindowManager::Instance().GetWindow().getSize();
    const auto bounds = GetBounds();
    const float centerY = bounds.top + bounds.height * 0.5f;

    if (centerY > static_cast<float>(windowSize.y))
    {
        m_alive = false;

        CT_LOG_DEBUG("Ship went out of bounds: ({}, {}). Marked for cleanup.", GetPosition().x, GetPosition().y);
    }
}

/// @brief Commandable kill for this Ship.
void BaseShip::Kill()
{
    m_alive = false;
}

/// @brief Return allegiance (player, enemy, etc.).
/// @return Allegiance for the ship.
Allegiance BaseShip::GetAllegiance() const
{
    return m_allegiance;
}

/// @brief Set allegiance.
/// @param allegiance new m_allegiance to set.
void BaseShip::SetAllegiance(const Allegiance allegiance)
{
    m_allegiance = allegiance;
}

/// @brief Return world position.
/// @return vector for this ships position.
sf::Vector2f BaseShip::GetPosition() const
{
    return m_sprite.getPosition();
}

/// @brief Set world position.
/// @param pos New position to set for ship.
void BaseShip::SetPosition(const sf::Vector2f &pos)
{
    m_sprite.setPosition(pos);
}

/// @brief Move the ship by an offset.
/// @param offset offset for which to move this ship.
void BaseShip::Move(const sf::Vector2f &offset)
{
    m_sprite.move(offset);
}

/// @brief Set render scale.
/// @param scaleX scalar value to use for this ships x size.
/// @param scaleY scalar value to use for this ships y size.
void BaseShip::SetScale(float scaleX, float scaleY)
{
    m_sprite.setScale(scaleX, scaleY);
}

/// @brief Return current speed vector.
/// @return Speed vector for this ship.
sf::Vector2f BaseShip::GetSpeed() const
{
    return m_speed;
}

/// @brief Set speed vector.
/// @param speed new Speed vector to use for this ships speed component.
void BaseShip::SetSpeed(const sf::Vector2f &speed)
{
    m_speed = speed;
}

/// @brief Get the current Gun for this Ship.
/// @return Pointer to this ships equipped gun interface.
BaseGun *BaseShip::GetGun()
{
    return m_gun.get();
}

/// @brief Const version for Gun safety, get gun for readonly.
/// @return Const pointer for this ships equipped gun interface.
const BaseGun *BaseShip::GetGun() const
{
    return m_gun.get();
}
