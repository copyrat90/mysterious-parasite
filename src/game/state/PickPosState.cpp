/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/state/PickPosState.hpp"

namespace mp::game::state
{

PickPosState::PickPosState(Dungeon& dungeon) : GameState(dungeon)
{
    _initGraphics();
}

auto PickPosState::handleInput() -> bn::optional<GameStateArgs>
{
    BN_ERROR("TODO");
    return bn::nullopt;
}

auto PickPosState::update() -> bn::optional<GameStateArgs>
{
    BN_ERROR("TODO");
    return bn::nullopt;
}

void PickPosState::onEnter(const GameStateArgs& args)
{
    BN_ERROR("TODO: Show cursor & toss range BG");
}

void PickPosState::onExit()
{
    BN_ERROR("TODO: Hide cursor & toss range BG");
}

void PickPosState::_initGraphics()
{
    // TODO: Initialize cursor & toss range BG
}

} // namespace mp::game::state
