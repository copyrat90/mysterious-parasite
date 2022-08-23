/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_vector.h"

#include "constants.hpp"
#include "game/BoardPos.hpp"
#include "game/item/Item.hpp"
#include "game/mob/MonsterAction.hpp"
#include "game/mob/MonsterAnimation.hpp"
#include "typedefs.hpp"

namespace mp::game
{
class Dungeon;
}

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
    void update(const Dungeon&);

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

private:
    const MonsterInfo& _info;

    MonsterAnimation _animation;
    BoardPos _pos;
    bn::vector<item::Item, consts::MOB_ITEM_MAX_COUNT> _items;
};

} // namespace mp::game::mob
