/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "game/BoardPos.hpp"
#include "typedefs.hpp"

namespace mp::game::mob
{

class MonsterAction
{
public:
    enum class Direction
    {
        UP,
        UP_RIGHT,
        RIGHT,
        DOWN_RIGHT,
        DOWN,
        DOWN_LEFT,
        LEFT,
        UP_LEFT
    };

    enum class Type
    {
        DO_NOTHING,
        MOVE,
        ATTACK,
        ABILITY,
        ITEM
    };

public:
    /**
     * @brief Constructor for `Type::ABILITY` and `Type::ITEM`, as they need `actionIdx`.
     */
    MonsterAction(Direction, Type, s32 actionIdx);

    /**
     * @brief Constructor for `Type` which doesn't need `actionIdx`.
     */
    MonsterAction(Direction, Type);

    Type getType() const;
    s32 getActionIdx() const;
    Direction getDirection() const;
    BoardPos getDirectionPos() const;

private:
    Direction _direction;
    Type _type;
    // ability or item index
    s32 _actionIdx;
};

} // namespace mp::game::mob
