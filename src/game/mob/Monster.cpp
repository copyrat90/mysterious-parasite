/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/mob/Monster.hpp"

#include "bn_array.h"
#include "bn_assert.h"
#include "bn_span.h"

#include "constants.hpp"
#include "game/mob/MonsterAction.hpp"
#include "game/mob/MonsterInfo.hpp"
#include "game/mob/MonsterSpecies.hpp"

namespace mp::game::mob
{

Monster::Monster(MonsterSpecies species, const BoardPos& pos, const bn::camera_ptr& camera)
    : _info(MonsterInfo::fromSpecies(species)), _animation(_info, camera), _pos(pos)
{
}

void Monster::update(const Dungeon& dungeon)
{
    _animation.update(dungeon);
}

bool Monster::isVisible() const
{
    return _animation.isVisible();
}

void Monster::setVisible(bool isVisible)
{
    _animation.setVisible(isVisible);
}

const MonsterAnimation& Monster::getAnimation() const
{
    return _animation;
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

void Monster::_act(const MonsterAction& action)
{
    switch (action.getType())
    {
        using Action = mob::MonsterAction::Type;
        using AnimType = MonsterAnimation::Type;
    case Action::CHANGE_DIRECTION:
        _animation.startActions(AnimType::IDLE, action.getDirection());
        break;
    case Action::MOVE:
        _animation.startActions(AnimType::WALK, action.getDirection());
        _pos += action.getDirectionPos();
        break;
    case Action::DO_NOTHING:
        break;
    default:
        BN_ERROR("Invalid MonsterAction::Type(", (s32)action.getType(), ")");
    }
}

} // namespace mp::game::mob
