// ============================================================================
//  File        : ShipType.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-09-14
//  Description : Common ship base of supported types, and string helper.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

/// @brief Base Score for AlienShip provided before scaling.
constexpr int ALIEN_SCORE_VALUE = 100;

/// @brief Base Score for BasicShip provided before scaling.
constexpr int BASIC_SHIP_SCORE_VALUE = 50;

/// @brief Base Score for BerserkerShip provided before scaling.
constexpr int BERSERKER_SCORE_VALUE = 250;

/// @brief Base Score for CrusaderShip provided before scaling.
constexpr int CRUSADER_SCORE_VALUE = 200;

/// @brief Base Score for GruntShip provided before scaling.
constexpr int GRUNT_SCORE_VALUE = 150;

/// @brief Base Score for InvaderShip provided before scaling.
constexpr int INVADER_SCORE_VALUE = 200;

/// @brief Enum class representing the supported types of ships.
enum class ShipType
{
    /// @brief Safe default of unknown ship.
    Unknown = 0,

    /// @brief Player ship.
    Player,

    /// @brief Alien ship.
    Alien,

    /// @brief Basic ship.
    Basic,

    /// @brief Berserker ship.
    Berserker,

    /// @brief Crusader ship.
    Crusader,

    /// @brief Grunt ship.
    Grunt,

    /// @brief Invader ship.
    Invader,

    /// @brief Jammer ship.
    Jammer,

    /// @brief Rammer ship.
    Rammer,

    /// @brief Scout ship.
    Scout,

    /// @brief Squid ship.
    Squid,
};

/// @brief Lightweight helper to return the string representation for the ship type.
/// @param t ShipType enum class.
/// @return The enum ShipType stringified.
inline const std::string ShipTypeToString(ShipType t)
{
    switch (t)
    {
        case ShipType::Player:
            return "Player";
        case ShipType::Alien:
            return "Alien";
        case ShipType::Basic:
            return "Basic";
        case ShipType::Berserker:
            return "Berserker";
        case ShipType::Crusader:
            return "Crusader";
        case ShipType::Grunt:
            return "Grunt";
        case ShipType::Invader:
            return "Invader";
        case ShipType::Jammer:
            return "Jammer";
        case ShipType::Rammer:
            return "Rammer";
        case ShipType::Scout:
            return "Scout";
        case ShipType::Squid:
            return "Squid";
        default:
            return "Unknown";
    }
}

/// @brief Returns the score that a type of ship would grant (prior to any difficulty scaling)
/// @param t ShipType.
/// @return Score value associated with the ships type
inline const std::string ShipScoreFromType(ShipType t)
{
    int val = 0;

    switch (t)
    {
        case ShipType::Alien:
            val = ALIEN_SCORE_VALUE;
            break;
        case ShipType::Basic:
            val = BASIC_SHIP_SCORE_VALUE;
            break;
        case ShipType::Berserker:
            val = BERSERKER_SCORE_VALUE;
            break;
        case ShipType::Crusader:
            val = CRUSADER_SCORE_VALUE;
            break;
        case ShipType::Grunt:
            val = GRUNT_SCORE_VALUE;
            break;
        case ShipType::Invader:
            val = INVADER_SCORE_VALUE;
            break;
        /// TODO: new ships added here.
        default:
            val = 0;
            break;
    }

    if (val <= 0)
    {
        return {};
    }

    // compose a string with "+x" to be used as a score string. where 0 < x <= 9999.
    // string::reserve(5) allows for the + and 4 user defined character lengths, not considering null.
    std::string ret;
    ret.reserve(5);
    ret.push_back('+');
    ret += std::to_string(val);

    return ret;
}
