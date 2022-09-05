/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/mob/Player.hpp"

#include "game/mob/MonsterSpecies.hpp"

namespace mp::game::mob
{

Player::Player(const BoardPos& boardPos, const bn::camera_ptr& camera)
    : Monster(MonsterSpecies::PLAYER, boardPos, camera)
{
}

void Player::actPlayer(const MonsterAction& action)
{
    _act(action);
}

} // namespace mp::game::mob
