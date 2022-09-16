/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/state/EnemyActState.hpp"

namespace mp::game::state
{

EnemyActState::EnemyActState(Dungeon& dungeon) : GameState(dungeon)
{
}

auto EnemyActState::update() -> bn::optional<GameStateArgs>
{
    BN_ERROR("TODO");
    return bn::nullopt;
}

} // namespace mp::game::state
