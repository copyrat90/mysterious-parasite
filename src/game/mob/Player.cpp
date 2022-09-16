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

namespace
{
constexpr s32 INITIAL_MAX_BELLY = 100;
constexpr s32 INITIAL_BELLY_DECREASE_TURNS = 10;
} // namespace

Player::Player(const BoardPos& boardPos, const bn::camera_ptr& camera, Hud& hud)
    : Monster(MonsterSpecies::PLAYER, boardPos, camera),
      _belly(INITIAL_MAX_BELLY, INITIAL_MAX_BELLY, INITIAL_BELLY_DECREASE_TURNS, hud)
{
}

bool Player::actPlayer(const MonsterAction& action)
{
    _act(action);
    bool isAlive = true;
    if (action.isSpendTurn())
        isAlive = isAlive && _belly.progressTurn();
    return isAlive;
}

PlayerBelly& Player::getBelly()
{
    return _belly;
}

} // namespace mp::game::mob
