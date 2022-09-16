/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "game/BoardPos.hpp"
#include "game/Direction9.hpp"
#include "typedefs.hpp"

namespace mp::game::mob
{

class MonsterAction
{
public:
    enum class Type
    {
        // do not spend a turn
        CHANGE_DIRECTION = 0,
        // spend a turn
        DO_NOTHING = 100,
        MOVE,
        ATTACK,
        ABILITY,
        ITEM
    };

public:
    /**
     * @brief Constructor for `Type::ABILITY` and `Type::ITEM`, as they need `actionIdx`.
     */
    MonsterAction(Direction9, Type, s32 actionIdx);

    /**
     * @brief Constructor for `Type` which doesn't need `actionIdx`.
     */
    MonsterAction(Direction9, Type);

    Type getType() const;
    s32 getActionIdx() const;
    Direction9 getDirection() const;
    BoardPos getDirectionPos() const;

    bool isSpendTurn() const;

private:
    Direction9 _direction;
    Type _type;
    // ability or item index
    s32 _actionIdx;
};

} // namespace mp::game::mob
