/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/state/MovingState.hpp"

namespace mp::game::state
{

MovingState::MovingState(Dungeon& dungeon) : GameState(dungeon)
{
}

auto MovingState::update() -> bn::optional<GameStateArgs>
{
    // TODO
    return bn::nullopt;
}

void MovingState::onEnter(const GameStateArgs& args)
{
    // TODO: Set object push animation
}

} // namespace mp::game::state
