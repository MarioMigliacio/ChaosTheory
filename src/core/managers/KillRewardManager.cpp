// ============================================================================
//  File        : KillRewardManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-14
//  Description : Rewards on enemy kill (icon drop, toast, score)
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "KillRewardManager.h"
#include "BaseShip.h"
#include "CollectableIconManager.h"
#include "ResolutionScaleManager.h"
#include "ScoreManager.h"
#include "ShipManager.h"
#include "UIFactory.h"
#include "UIManager.h"
#include "WindowManager.h"

/// @brief Provide any default constants that are utilized in KillRewardManager
namespace
{
/// @brief Icon size is constant.
const sf::Vector2f STANDARD_ICON_SIZE = {32.f, 32.f};
} // namespace

/// @brief Pick a random element from a vector
/// @tparam T Template type, doesn't matter.
/// @param v Reference to vector.
/// @param out Element is stored in out upon success.
/// @return True / False, if true out stores the element.
template <typename T> static bool PickOne(const std::vector<T> &v, T &out)
{
    if (v.empty())
    {
        return false;
    }

    const float idxf = CT_MATH::RandRange(0.f, static_cast<float>(v.size())); // [0, size)
    const std::size_t idx = static_cast<std::size_t>(idxf);                   // trunc ok
    out = v[(idx >= v.size()) ? v.size() - 1 : idx];

    return true;
}

/// @brief Get the current Instance for this KillRewardManager singleton.
/// @return reference to existing KillRewardManager interface.
KillRewardManager &KillRewardManager::Instance()
{
    static KillRewardManager s;
    return s;
}

/// @brief Sets the configuration for this KillRewardManager instance.
/// @param cfg KillRewardConfig settings to set.
void KillRewardManager::Configure(const KillRewardConfig &cfg)
{
    m_cfg = cfg;
}

/// @brief Insert a rule into the unordered map collection of maintained rules.
/// @param tag Key to index at.
/// @param rule KillRewardDropRule to be inserted at Key index.
void KillRewardManager::SetDropRule(const std::string &tag, const KillRewardDropRule &rule)
{
    m_rules[tag] = rule;
}

/// @brief The main driving logic behind KillRewards, triggers rule for items, messages and score if rule exists.
/// @param victim Reference to the ship that has been killed.
/// @param killerAllegiance Only applies on killed logic when player is the killer.
void KillRewardManager::OnEnemyKilled(const BaseShip &victim, Allegiance killerAllegiance)
{
    if (!m_cfg.enabled)
    {
        return;
    }

    if (killerAllegiance != Allegiance::Player)
    {
        return;
    }

    const auto tag = ExtractVictimTag(victim);
    const auto &rule = ResolveRule(tag);

    // Precedence: try rare first; if it fails, try regular
    bool dropped = false;
    const float roll = CT_MATH::RandRange(0.f, 1.f);

    // Rare roll
    if (!rule.rareDrops.empty() && roll <= std::clamp(rule.rareDropChance, 0.f, 1.f))
    {
        std::tuple<IconType, IconEffectType> pick;

        if (PickOne(rule.rareDrops, pick))
        {
            const auto pos = Scatter(victim.GetPosition());
            SpawnIcon(pos, std::get<0>(pick), std::get<1>(pick));
            dropped = true;
        }
    }

    // Regular roll (only if nothing dropped yet)
    if (!dropped && !rule.regularDrops.empty() && roll <= std::clamp(rule.regularDropChance, 0.f, 1.f))
    {
        std::tuple<IconType, IconEffectType> pick;
        if (PickOne(rule.regularDrops, pick))
        {
            const auto pos = Scatter(victim.GetPosition());
            SpawnIcon(pos, std::get<0>(pick), std::get<1>(pick));
            dropped = true;
        }
    }

    // Guarantee path: force-pick from the tag's pools; if empty, from "default"
    if (!dropped && m_cfg.guaranteeDrop)
    {
        const KillRewardDropRule &src =
            (!rule.regularDrops.empty() || !rule.rareDrops.empty()) ? rule : m_rules.at("default");

        std::tuple<IconType, IconEffectType> pick;
        bool ok = false;

        // Try to pick from whichever pool is available (favor regular for guarantee, tweak as desired)
        if (!src.regularDrops.empty())
        {
            ok = PickOne(src.regularDrops, pick);
        }

        else if (!src.rareDrops.empty())
        {
            ok = PickOne(src.rareDrops, pick);
        }

        if (ok)
        {
            const auto pos = Scatter(victim.GetPosition());
            SpawnIcon(pos, std::get<0>(pick), std::get<1>(pick));
            dropped = true;
        }
    }

    // Toast (e.g., score text)
    if (m_cfg.showToast && rule.toast.has_value())
    {
        ShowToast(victim.GetPosition(), *rule.toast);
    }

    GrantScore(victim.GetScoreValue());
}

/// @brief Acquire the victims string representation based on ship.
/// @param ship Type of ship that was killed.
/// @return String of the ship type.
std::string KillRewardManager::ExtractVictimTag(const BaseShip &ship)
{
    return std::string(ShipTypeToString(ship.GetShipType()));
}

/// @brief Constructs this KillRewardManager service.
KillRewardManager::KillRewardManager()
{
    m_rules.emplace("default", BuildDefaultRule());
}

/// @brief Safe default rule to ensure that no crashes happen if misconfigured.
/// @return KillRewardDropRule that is a safe default rule to emplace in collection.
KillRewardDropRule KillRewardManager::BuildDefaultRule() const
{
    KillRewardDropRule def;
    def.regularDropChance = 0.20f;
    def.rareDropChance = 0.10f;

    def.regularDrops = {
        {IconType::UpgradeIcon, IconEffectType::GunUpgradeBoost},
        {IconType::VelocityIcon, IconEffectType::GunVelocityBoost},
        {IconType::PowerIcon, IconEffectType::GunDamageBoost},
        {IconType::FireRateIcon, IconEffectType::GunFireRateBoost},
        {IconType::HealthRestoreIcon, IconEffectType::HealthRestore},
        {IconType::GasRestoreIcon, IconEffectType::GasRestore},
    };
    def.rareDrops = {
        {IconType::LifeIcon, IconEffectType::LifeIncrease},
        {IconType::AtomicIcon, IconEffectType::BombQuantityBoost},
        {IconType::HealthBoostIcon, IconEffectType::HealthBoost},
        {IconType::GasBoostIcon, IconEffectType::GasBoost},
    };

    return def;
}

/// @brief Locate the rule at the index of the tag.
/// @param tag String index to search.
/// @return Rule associated with the tag key index.
const KillRewardDropRule &KillRewardManager::ResolveRule(const std::string &tag)
{
    auto it = m_rules.find(tag);

    if (it != m_rules.end())
    {
        return it->second;
    }

    // Always retrun safe default if no rule exists.
    return m_rules.at("default");
}

/// @brief Spawns an upgrade icon at the location upon enemy death and passing drop check.
/// @param at World coordinates to drop at.
/// @param icon IconType type.
/// @param effect IconEffectType type.
void KillRewardManager::SpawnIcon(const sf::Vector2f &at, IconType icon, IconEffectType effect) const
{
    CollectableIconManager::Instance().SpawnIcon(IconConfig{.position = at,
                                                            .size = STANDARD_ICON_SIZE,
                                                            .textureKey = SpriteKeyFor(icon),
                                                            .type = icon,
                                                            .effectType = effect});
}

/// @brief Display a brief toast message at the area of impact.
/// @param at World coordinates to drop at.
/// @param text Text to be displayed at the location of impact.
void KillRewardManager::ShowToast(const sf::Vector2f &at, const std::string &text) const
{
    if (!m_cfg.showToast || text.empty())
    {
        return;
    }

    auto &window = WindowManager::Instance().GetWindow();
    const sf::Vector2i pixel = window.mapCoordsToPixel(at);
    sf::Vector2f uiPos(static_cast<float>(pixel.x), static_cast<float>(pixel.y));
    uiPos += m_cfg.toastOffset;

    auto toast =
        UIFactory::Instance().CreateToastMessage(ToastMessageConfig{.text = text,
                                                                    .position = uiPos,
                                                                    .duration = m_cfg.toastLifetime,
                                                                    .baseFontSize = m_cfg.toastFontSize,
                                                                    .centerOrigin = true,
                                                                    .scheme = UITextLabelScheme::MintyHerbScheme});

    UIManager::Instance().AddElement(toast);
}

/// @brief Applies score to the player ship, based on amount.
/// @param amount Score to grant the player.
void KillRewardManager::GrantScore(int amount) const
{
    if (amount <= 0)
    {
        return;
    }

    if (ScoreManager::Instance().IsInitialized())
    {
        ScoreManager::Instance().Add(amount);
    }
}

/// @brief Scatter the world coordinate location just a little bit to add some variance spice.
/// @param at World coordinate to adjust very slightly in a radius nearby.
/// @return New Vector2f coordinate pair to use.
sf::Vector2f KillRewardManager::Scatter(const sf::Vector2f &at) const
{
    if (m_cfg.scatterRadius <= 0.f)
    {
        return at;
    }

    const float r = m_cfg.scatterRadius * std::sqrt(CT_MATH::RandRange(0.f, 1.f));
    const float ang = 2.f * PI * CT_MATH::RandRange(0.f, 1.f);

    return {at.x + r * std::cos(ang), at.y + r * std::sin(ang)};
}
