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
#include "bn_vector.h"

#include "constants.hpp"
#include "game/BoardPos.hpp"
#include "game/Direction9.hpp"
#include "game/item/Item.hpp"
#include "typedefs.hpp"

namespace mp::game::mob
{

enum class MonsterSpecies;
class MonsterInfo;
class MonsterAction;

class Monster final
{
public:
    Monster(MonsterSpecies, const BoardPos&);

    /**
     * @brief Frame update, mostly for animations.
     */
    void update();

    /**
     * @brief Take a turn, based on their AI.
     */
    void actAI();

    /**
     * @brief Take a turn, with the player's order.
     */
    void actPlayer(const MonsterAction& action);

    bool isVisible() const;
    void setVisible(bool);

    const BoardPos& getBoardPos() const;
    void setBoardPos(u8 x, u8 y);
    void setBoardPos(const BoardPos&);

private:
    void _act(MonsterAction& action);

    /**
     * @brief Check if the same animation is ongoing, and if not, start the idle animation.
     */
    void _startIdleAnimation(Direction9);

    /**
     * @brief Check if the same animation is ongoing, and if not, start the walk animation.
     */
    void _startWalkAnimation(Direction9);

    void _initGraphics();

private:
    const MonsterInfo& _info;

    bn::sprite_ptr _sprite;
    bn::sprite_animate_action<consts::MOB_ANIM_MAX_KEYFRAMES> _animation;
    BoardPos _pos;
    Direction9 _direction = Direction9::DOWN;
    bn::vector<item::Item, consts::MOB_ITEM_MAX_COUNT> _items;
};

} // namespace mp::game::mob
