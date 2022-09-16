/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/state/PlayerActState.hpp"

namespace mp::game::state
{

PlayerActState::PlayerActState(Dungeon& dungeon) : GameState(dungeon)
{
}

auto PlayerActState::update() -> bn::optional<GameStateArgs>
{
    BN_ERROR("TODO");
    return bn::nullopt;
}

void PlayerActState::onEnter(const GameStateArgs& args)
{
    BN_ERROR("TODO: Set player item use/toss animation");
}

} // namespace mp::game::state
