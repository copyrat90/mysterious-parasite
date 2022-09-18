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

class PlayerActState final : public GameState
{
public:
    PlayerActState(Dungeon&);

    GameStateKind getStateKind() const override
    {
        return GameStateKind::PLAYER_ACT;
    }

    [[nodiscard]] auto update() -> bn::optional<GameStateArgs> override;

    void onEnter(const GameStateArgs&) override;
    void onExit() override;

private:
    void _startItemUse();
    void _startItemToss();
};

} // namespace mp::game::state
