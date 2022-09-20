/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "game/state/GameState.hpp"

#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"

namespace mp::game
{
class MetaTileset;
}

namespace mp::game::state
{

class PickPosState final : public GameState
{
public:
    PickPosState(Dungeon&);

    GameStateKind getStateKind() const override
    {
        return GameStateKind::PICK_POS;
    }

    [[nodiscard]] auto handleInput() -> bn::optional<GameStateArgs> override;

    void onEnter(const GameStateArgs&) override;
    void onExit() override;

private:
    void _initGraphics();

    void _redrawCells(const GameStateArgs&);

    /**
     * @brief Redraw the toss hint BG cell.
     *
     * @param relativePos relative pos from player (i.e. player pos is {0, 0})
     */
    void _redrawCell(const BoardPos& relativePos);

public:
    static constexpr s32 ROWS = 256 / 8;
    static constexpr s32 COLUMNS = 256 / 8;
    static constexpr s32 CELLS_COUNT = ROWS * COLUMNS;

private:
    const MetaTileset& _tileset;

    alignas(4) bn::regular_bg_map_cell _cells[CELLS_COUNT];
    bn::regular_bg_map_item _mapItem;
    bn::regular_bg_item _bgItem;
    bn::regular_bg_ptr _bg;
    bn::regular_bg_map_ptr _bgMap;
};

} // namespace mp::game::state
