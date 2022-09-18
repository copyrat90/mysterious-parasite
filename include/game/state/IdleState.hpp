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

    GameStateKind getStateKind() const override
    {
        return GameStateKind::IDLE;
    }

    [[nodiscard]] auto handleInput() -> bn::optional<GameStateArgs> override;
    [[nodiscard]] auto update() -> bn::optional<GameStateArgs> override;

    void onEnter(const GameStateArgs&) override;

private:
    [[nodiscard]] auto _handleMovement() -> bn::optional<GameStateArgs>;
    [[nodiscard]] auto _handleItemUse() -> bn::optional<GameStateArgs>;
    [[nodiscard]] auto _handleItemToss() -> bn::optional<GameStateArgs>;

private:
    bn::optional<GameStateArgs::DeathReason> _gameOverReason;

#ifdef MP_DEBUG
private:
    void _testMapGen();
#endif
};

} // namespace mp::game::state
