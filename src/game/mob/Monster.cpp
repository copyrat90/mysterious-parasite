/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/mob/Monster.hpp"

#include "bn_assert.h"

#include "constants.hpp"
#include "game/mob/MonsterAction.hpp"
#include "game/mob/MonsterInfo.hpp"
#include "game/mob/MonsterSpecies.hpp"

namespace mp::game::mob
{

Monster::Monster(MonsterSpecies species, const BoardPos& pos)
    : _info(MonsterInfo::fromSpecies(species)), _sprite(_info.spriteItem.create_sprite(0, 0)), _pos(pos)
{
    setVisible(false);
}

void Monster::actPlayer(const MonsterAction& action)
{
    switch (action.getType())
    {
        using Action = mob::MonsterAction::Type;
    case Action::MOVE:
        _pos += action.getDirectionPos();
        break;
    case Action::DO_NOTHING:
        break;
    default:
        BN_ERROR("Invalid MonsterAction::Type(", (s32)action.getType(), ")");
    }
}

bool Monster::isVisible() const
{
    return _sprite.visible();
}

void Monster::setVisible(bool isVisible)
{
    _sprite.set_visible(isVisible);
}

const BoardPos& Monster::getBoardPos() const
{
    return _pos;
}

void Monster::setBoardPos(u8 x, u8 y)
{
    BN_ASSERT(x < consts::DUNGEON_FLOOR_SIZE.width() && y < consts::DUNGEON_FLOOR_SIZE.height());
    _pos.x = x;
    _pos.y = y;
}

void Monster::setBoardPos(const BoardPos& pos)
{
    setBoardPos(pos.x, pos.y);
}

} // namespace mp::game::mob
