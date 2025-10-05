// ============================================================================
//  File        : KillRewardManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-14
//  Description : Rewards on enemy kill (icon drop, toast, score)
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Allegiance.h"
#include "UIIcon.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <random>
#include <string>
#include <unordered_map>

class BaseShip; // forward declare is sufficient, only using pointers.

/// @brief KillRewardDropRule is used to setup per-unit drop rates and allowed items to drop from enemies.
/// @param regularDrops Vector collection of a IconType+IconEffect identifier.
/// @param rareDrops Vector collection of a IconType+IconEffect identifier.
/// @param toast Toast string message to appear upon OnDeath event.
/// @param regularDropChance Represents a common drop upon OnDeath event.
/// @param rareDropChance Represents a rare drop upon OnDeath event.
struct KillRewardDropRule
{
    std::vector<std::tuple<IconType, IconEffectType>> regularDrops;
    std::vector<std::tuple<IconType, IconEffectType>> rareDrops;

    std::optional<std::string> toast;

    float regularDropChance = 0.20f;
    float rareDropChance = 0.10f;
};

/// @brief KillRewardConfig is used to stage the global KillRewardManager with enabled states, and toast options.
/// @param enabled Master switch to allow service OnEnemyDeath effects and drops.
/// @param guaranteeDrop Switch to allow drops to always happen on EnemyDeath.
/// @param showToast Whether or not to show a toast message OnEnemyDeath.
/// @param toastLifetime Duration for enabled toast messages.
/// @param toastFontSize Size of toast messages when enabled.
/// @param toastOffset Distance offset for toast message from impact spot if toast enabled.
/// @param scatterRadius Distance around impact spot from OnEnemyDeath for drops to scatter towards.
struct KillRewardConfig
{
    bool enabled = true;
    bool guaranteeDrop = false;
    bool showToast = true;
    float toastLifetime = 1.0f;
    unsigned int toastFontSize = 20;
    sf::Vector2f toastOffset{0.f, -28.f};

    float scatterRadius = 0.f;
};

// ============================================================================
//  Class       : KillRewardManager
//  Purpose     : KillRewardManager class supports configured 'Rules' which
//                govern which power ups enemies are allowed to drop, and
//                at what rate upon death.
//
//  Responsibilities:
//      - OnEnemyKilled event which allows for:
//        - Granting score to player
//        - Showing brief toast message of score gained.
//        - Spawn an Icon in a scattered location near impact for added effect.
//
// ============================================================================
class KillRewardManager
{
  public:
    static KillRewardManager &Instance();

    void Configure(const KillRewardConfig &cfg);
    void SetDropRule(const std::string &victimTag, const KillRewardDropRule &rule);
    void OnEnemyKilled(const BaseShip &victim, Allegiance killerAllegiance);

    std::string ExtractVictimTag(const BaseShip &ship);

  private:
    KillRewardManager();

  private:
    KillRewardDropRule BuildDefaultRule() const;
    const KillRewardDropRule &ResolveRule(const std::string &tag);
    void SpawnIcon(const sf::Vector2f &at, IconType icon, IconEffectType effect) const;
    void ShowToast(const sf::Vector2f &at, const std::string &text) const;
    void GrantScore(int amount) const;
    sf::Vector2f Scatter(const sf::Vector2f &at) const;

  private:
    KillRewardConfig m_cfg{};
    std::unordered_map<std::string, KillRewardDropRule> m_rules;
};
