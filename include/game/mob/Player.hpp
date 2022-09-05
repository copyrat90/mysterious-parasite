/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "game/mob/Monster.hpp"

namespace mp::game::mob
{

class Player final : public Monster
{
public:
    Player(const BoardPos&, const bn::camera_ptr&);

    /**
     * @brief Take a turn, with the player's order.
     */
    void actPlayer(const MonsterAction& action);
};

} // namespace mp::game::mob
