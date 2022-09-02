/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_sprite_animate_actions.h"
#include "bn_sprite_ptr.h"

#include "constants.hpp"
#include "game/Direction9.hpp"

namespace mp::game
{
class Dungeon;
}

namespace mp::game::mob
{

class MonsterInfo;

class MonsterAnimation final
{
public:
    /**
     * @brief DO NOT CHANGE THE ORDER, as it is used as graphics indexes on `bn::sprite_animatie_action`.
     */
    enum Type
    {
        IDLE = 0,
        WALK
    };

public:
    MonsterAnimation(const MonsterInfo&, const bn::camera_ptr&);

    void update(const Dungeon&);

    bool isVisible() const;
    void setVisible(bool);

    /**
     * @brief Check if the same animation is ongoing, and if not, start the animation.
     */
    void startAnimation(Type animType, Direction9);

private:
    void _initGraphics(const bn::camera_ptr&);

    auto _getAnimation(Type, Direction9) -> bn::sprite_animate_action<consts::MOB_ANIM_MAX_KEYFRAMES>;

    bool _isSameAnimationOngoing(Type, Direction9) const;

    /**
     * @brief Indicate if spent the extra wait update on the last animation image.
     */
    bool _isFullyDone() const;

    void _restartAnimation();

private:
    const MonsterInfo& _mobInfo;

    bn::sprite_ptr _sprite;
    bn::sprite_animate_action<consts::MOB_ANIM_MAX_KEYFRAMES> _animateAction;

    Type _animType = Type::IDLE;
    Direction9 _direction = Direction9::DOWN;

    /**
     * @brief extra wait update on the last animation image
     */
    s32 _extraWaitUpdate = consts::MOB_ANIM_WAIT_UPDATE;
};

} // namespace mp::game::mob
