/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/mob/MonsterAction.hpp"

#include "bn_assert.h"

namespace mp::game::mob
{

MonsterAction::MonsterAction(Direction9 direction, Type type, s32 actionIdx)
    : _direction(direction), _type(type), _actionIdx(actionIdx)
{
    BN_ASSERT(type == Type::ABILITY || type == Type::ITEM, "actionIndex(", actionIdx, ") provided with type(",
              (s32)type, ")");
}

MonsterAction::MonsterAction(Direction9 direction, Type type) : _direction(direction), _type(type)
{
    BN_ASSERT(type != Type::ABILITY && type != Type::ITEM, "actionIndex not provided with type(", (s32)type, ")");
}

auto MonsterAction::getType() const -> Type
{
    return _type;
}

s32 MonsterAction::getActionIdx() const
{
    BN_ASSERT(_type == Type::ABILITY || _type == Type::ITEM, "_type(", (s32)_type, ") doesn't have actionIndex");
    return _actionIdx;
}

auto MonsterAction::getDirection() const -> Direction9
{
    return _direction;
}

BoardPos MonsterAction::getDirectionPos() const
{
    return convertDir9ToPos(_direction);
}

} // namespace mp::game::mob
