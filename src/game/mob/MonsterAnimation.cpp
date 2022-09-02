/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/mob/MonsterAnimation.hpp"

#include "bn_camera_ptr.h"

#include "game/Dungeon.hpp"
#include "game/mob/MonsterInfo.hpp"

namespace mp::game::mob
{

MonsterAnimation::MonsterAnimation(const MonsterInfo& mobInfo, const bn::camera_ptr& camera)
    : _mobInfo(mobInfo), _sprite(mobInfo.spriteItem.create_sprite(0, 0)),
      _animateAction(_getAnimation(Type::IDLE, Direction9::DOWN))
{
    _initGraphics(camera);
}

void MonsterAnimation::update(const Dungeon& dungeon)
{
    if (isVisible())
    {
        // switch to idle animation if
        //   1. current animation is fully done
        //   2. turn is done ongoing
        if (_animateAction.done())
        {
            if (!_isFullyDone())
                --_extraWaitUpdate;
            else
            {
                if (!dungeon.isTurnOngoing())
                    startAnimation(Type::IDLE, _direction);
            }
        }
        else
        {
            _animateAction.update();
        }
    }
}

bool MonsterAnimation::isVisible() const
{
    return _sprite.visible();
}

void MonsterAnimation::setVisible(bool isVisible)
{
    _sprite.set_visible(isVisible);
}

void MonsterAnimation::startAnimation(Type animType, Direction9 direction)
{
    if (_isSameAnimationOngoing(animType, direction))
    {
        if (_isFullyDone())
            _restartAnimation();
        return;
    }

    _extraWaitUpdate = consts::MOB_ANIM_WAIT_UPDATE;
    _animType = animType;
    _direction = direction;
    _animateAction = _getAnimation(animType, direction);
}

void MonsterAnimation::_initGraphics(const bn::camera_ptr& camera)
{
    setVisible(false);
    _sprite.set_bg_priority(consts::DUNGEON_BG_PRIORITY);
    _sprite.set_camera(camera);
}

auto MonsterAnimation::_getAnimation(Type animType, Direction9 direction)
    -> bn::sprite_animate_action<consts::MOB_ANIM_MAX_KEYFRAMES>
{
    // fallback is down animation
    bn::array<u16, 2> graphicsIndexes = {8, 9};
    if (direction != Direction9::NONE)
    {
        graphicsIndexes[0] = (u16)direction * 2 - 2;
        graphicsIndexes[1] = (u16)direction * 2 - 1;
    }
    graphicsIndexes[0] += (u16)animType * 16;
    graphicsIndexes[1] += (u16)animType * 16;

    const bool isForever = (animType == Type::IDLE);

    if (isForever)
        return bn::sprite_animate_action<consts::MOB_ANIM_MAX_KEYFRAMES>::forever(
            _sprite, consts::MOB_ANIM_WAIT_UPDATE, _mobInfo.spriteItem.tiles_item(), graphicsIndexes);
    // once (not forever)
    return bn::sprite_animate_action<consts::MOB_ANIM_MAX_KEYFRAMES>::once(
        _sprite, consts::MOB_ANIM_WAIT_UPDATE, _mobInfo.spriteItem.tiles_item(), graphicsIndexes);
}

bool MonsterAnimation::_isSameAnimationOngoing(Type animType, Direction9 direction) const
{
    return _animType == animType && _direction == direction;
}

bool MonsterAnimation::_isFullyDone() const
{
    return _animateAction.done() && _extraWaitUpdate <= 0;
}

void MonsterAnimation::_restartAnimation()
{
    _extraWaitUpdate = consts::MOB_ANIM_WAIT_UPDATE;
    _animateAction.reset();
}

} // namespace mp::game::mob
