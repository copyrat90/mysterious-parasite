/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "game/state/GameState.hpp"

#include "bn_camera_actions.h"

#include "game/Direction9.hpp"

namespace mp::game::state
{

class MovingState final : public GameState
{
public:
    MovingState(Dungeon&);

    GameStateKind getStateKind() const override
    {
        return GameStateKind::MOVING;
    }

    [[nodiscard]] auto update() -> bn::optional<GameStateArgs> override;

    void onEnter(const GameStateArgs&) override;
    void onExit() override;

private:
    void _startBgScroll(Direction9 moveDir);
    void _updateBgScroll();

    void _pickUpItem();

    /**
     * @brief Check if no one moves on this turn.
     */
    bool _noOneMoves(Direction9 playerMoveDir) const;

private:
    bn::optional<bn::camera_move_to_action> _camMoveAction;

    s32 _moveCountdown;
};

} // namespace mp::game::state
