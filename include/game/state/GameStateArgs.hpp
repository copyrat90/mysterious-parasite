/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_optional.h"

#include "game/BoardPos.hpp"
#include "game/Direction9.hpp"

namespace mp::game::state
{

enum class GameStateKind
{
    IDLE,
    PICK_POS,
    PLAYER_ACT,
    MOVING,
    ENEMY_ACT,
    GAME_OVER,
    // special state
    RESTART_GAME = 100,
};

/**
 * @brief Args to be passed on game state transition.
 */
struct GameStateArgs final
{
public:
    GameStateArgs(GameStateKind prevKind, GameStateKind nextKind);

public:
    enum class DeathReason : u8
    {
        STARVE,
        SPOTTED
    };
    enum class ItemAction : u8
    {
        USE,
        TOSS
    };

public:
    /**
     * @brief See if transition should take place.
     *
     * @return `true` if next state differ from previous state.
     */
    [[nodiscard]] bool shouldChangeState() const;

    auto getPrevStateKind() const -> GameStateKind;
    auto getNextStateKind() const -> GameStateKind;
    void setNextStateKind(GameStateKind);

    auto getBoardPos() const -> const BoardPos&;
    void setBoardPos(const BoardPos&);

    auto getDirection() const -> Direction9;
    void setDirection(Direction9);

    auto getDeathReason() const -> DeathReason;
    void setDeathReason(DeathReason);

    auto getItemAction() const -> ItemAction;
    void setItemAction(ItemAction);

private:
    GameStateKind _prevKind, _nextKind;
    bn::optional<BoardPos> _boardPos;
    bn::optional<Direction9> _direction;
    bn::optional<DeathReason> _deathReason;
    bn::optional<ItemAction> _itemAction;
};

} // namespace mp::game::state
