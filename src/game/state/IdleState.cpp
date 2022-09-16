/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/state/IdleState.hpp"

namespace mp::game::state
{

IdleState::IdleState(Dungeon& dungeon) : GameState(dungeon)
{
}

auto IdleState::handleInput() -> bn::optional<GameStateArgs>
{
    // TODO
    return bn::nullopt;
}

} // namespace mp::game::state
