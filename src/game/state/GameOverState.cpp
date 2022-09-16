/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/state/GameOverState.hpp"

namespace mp::game::state
{

GameOverState::GameOverState(Dungeon& dungeon) : GameState(dungeon)
{
    _initGraphics();
}

auto GameOverState::handleInput() -> bn::optional<GameStateArgs>
{
    BN_ERROR("TODO");
    return bn::nullopt;
}

auto GameOverState::update() -> bn::optional<GameStateArgs>
{
    BN_ERROR("TODO");
    return bn::nullopt;
}

void GameOverState::onEnter(const GameStateArgs& args)
{
    BN_ERROR("TODO: Show game over animation");
}

void GameOverState::onExit()
{
    BN_ERROR("TODO: Hide game over animation");
}

void GameOverState::_initGraphics()
{
    // TODO: Initialize game over sprites
}

} // namespace mp::game::state
