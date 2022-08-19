
#pragma once

#include "bn_assert.h"

#include "BoardPos.hpp"

namespace mp::game
{

/**
 * @brief No direction + clockwise 8 directions, from UP to UP_LEFT.
 * DO NOT CHANGE THE ORDER, as it is used as graphics indexes on `bn::sprite_animatie_action`.
 */
enum Direction9
{
    NONE = 0,
    UP,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT,
    LEFT,
    UP_LEFT
};

Direction9 getDirectionFromKeyPress();
Direction9 getDirectionFromKeyHold();
Direction9 getDirectionFromKeyRelease();

inline constexpr BoardPos convertDir9ToPos(Direction9 direction)
{
    switch (direction)
    {
        using Dir9 = Direction9;
    case Dir9::NONE:
        return {0, 0};
    case Dir9::UP:
        return {0, -1};
    case Dir9::UP_RIGHT:
        return {1, -1};
    case Dir9::RIGHT:
        return {1, 0};
    case Dir9::DOWN_RIGHT:
        return {1, 1};
    case Dir9::DOWN:
        return {0, 1};
    case Dir9::DOWN_LEFT:
        return {-1, 1};
    case Dir9::LEFT:
        return {-1, 0};
    case Dir9::UP_LEFT:
        return {-1, -1};
    default:
        BN_ERROR("Invalid _direction(", (s32)direction, ")");
    }
    return {0, 0};
}

inline constexpr Direction9 convertPosToDir9(const BoardPos& pos)
{
    using Dir9 = Direction9;
    if (pos.x == 0 && pos.y == 0)
        return Dir9::NONE;
    if (pos.x == 0 && pos.y == -1)
        return Dir9::UP;
    if (pos.x == 1 && pos.y == -1)
        return Dir9::UP_RIGHT;
    if (pos.x == 1 && pos.y == 0)
        return Dir9::RIGHT;
    if (pos.x == 1 && pos.y == 1)
        return Dir9::DOWN_RIGHT;
    if (pos.x == 0 && pos.y == 1)
        return Dir9::DOWN;
    if (pos.x == -1 && pos.y == 1)
        return Dir9::DOWN_LEFT;
    if (pos.x == -1 && pos.y == 0)
        return Dir9::LEFT;
    if (pos.x == -1 && pos.y == -1)
        return Dir9::UP_LEFT;
    // no direction as fallback
    return Dir9::NONE;
}

} // namespace mp::game
