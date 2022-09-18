/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "game/mob/Monster.hpp"

#include "game/mob/PlayerBelly.hpp"

namespace mp::game
{
class Hud;
}

namespace mp::game::mob
{

class Player final : public Monster
{
public:
    Player(const BoardPos&, const bn::camera_ptr&, Hud&);

    /**
     * @brief Take a turn, with the player's order.
     */
    void actPlayer(const MonsterAction& action);

    PlayerBelly& getBelly();

private:
    PlayerBelly _belly;
};

} // namespace mp::game::mob
