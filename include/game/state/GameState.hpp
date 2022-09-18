/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_optional.h"

#include "game/state/GameStateArgs.hpp"

namespace mp::game
{
class Dungeon;
}

namespace mp::game::state
{

class GameState
{
public:
    virtual ~GameState() = default;

    GameState(Dungeon& dungeon) : _dungeon(dungeon){};

    virtual GameStateKind getStateKind() const = 0;

public:
    /**
     * @brief Handle user key input.
     *
     * @return bn::optional<GameStateArgs> next game state args (if any).
     */
    [[nodiscard]] virtual auto handleInput() -> bn::optional<GameStateArgs>
    {
        return bn::nullopt;
    }

    /**
     * @brief update the state.
     *
     * @return bn::optional<GameStateArgs> next game state args (if any).
     */
    [[nodiscard]] virtual auto update() -> bn::optional<GameStateArgs>
    {
        return bn::nullopt;
    }

    /**
     * @brief Fires on entering this state.
     */
    virtual void onEnter(const GameStateArgs&){};

    /**
     * @brief Fires on exiting this state.
     */
    virtual void onExit(){};

protected:
    Dungeon& _dungeon;
};

} // namespace mp::game::state
