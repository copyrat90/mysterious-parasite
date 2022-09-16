/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "game/state/GameState.hpp"

namespace mp::game::state
{

class IdleState final : public GameState
{
public:
    IdleState(Dungeon&);

    [[nodiscard]] auto handleInput() -> bn::optional<GameStateArgs> override;
};

} // namespace mp::game::state
