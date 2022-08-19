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

static auto _getAnimation(const bn::sprite_ptr& sprite, const bn::sprite_item& spriteItem, bool isForever,
                          Direction9 direction) -> bn::sprite_animate_action<consts::MOB_ANIM_MAX_KEYFRAMES>
{
    // fallback is down animation
    bn::array<u16, 2> graphicsIndexes = {8, 9};
    if (direction != Direction9::NONE)
    {
        graphicsIndexes[0] = (u16)direction * 2 - 2;
        graphicsIndexes[1] = (u16)direction * 2 - 1;
    }

    return bn::sprite_animate_action<consts::MOB_ANIM_MAX_KEYFRAMES>::forever(sprite, consts::MOB_ANIM_WAIT_UPDATE,
                                                                              spriteItem.tiles_item(), graphicsIndexes);
}

Monster::Monster(MonsterSpecies species, const BoardPos& pos)
    : _info(MonsterInfo::fromSpecies(species)), _sprite(_info.spriteItem.create_sprite(0, 0)),
      _animation(_getAnimation(_sprite, _info.spriteItem, true, Direction9::DOWN)), _pos(pos)
{
    _initGraphics();
}

void Monster::update()
{
    if (isVisible())
    {
        if (!_animation.done())
            _animation.update();
    }
}

void Monster::actPlayer(const MonsterAction& action)
{
    switch (action.getType())
    {
        using Action = mob::MonsterAction::Type;
    case Action::CHANGE_DIRECTION:
        _startIdleAnimation(action.getDirection());
        break;
    case Action::MOVE:
        _startWalkAnimation(action.getDirection());
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

void Monster::_startIdleAnimation(Direction9 direction)
{
    // TODO: Add idle animation check
    if (_direction == direction)
        return;

    _direction = direction;
    _animation = _getAnimation(_sprite, _info.spriteItem, true, direction);
}

void Monster::_startWalkAnimation(Direction9 direction)
{
    // TODO: Implement walking animation, instead of fallback to idle
    return _startIdleAnimation(direction);
}

void Monster::_initGraphics()
{
    setVisible(false);
    _sprite.set_bg_priority(consts::DUNGEON_BG_PRIORITY);
}

} // namespace mp::game::mob
