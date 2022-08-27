/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"

#include "game/MetaTileset.hpp"
#include "typedefs.hpp"

namespace mp::game
{

class DungeonFloor;

/**
 * @brief Manages dungeon background scrolling with tileset.
 */
class DungeonBg final
{
public:
    static constexpr s32 ROWS = 256 / 8;
    static constexpr s32 COLUMNS = 256 / 8;
    static constexpr s32 CELLS_COUNT = ROWS * COLUMNS;

private:
    const MetaTileset* _metaTileset;

    bn::regular_bg_map_cell _cells[CELLS_COUNT];
    bn::regular_bg_map_item _mapItem;
    bn::regular_bg_item _bgItem;
    bn::regular_bg_ptr _bg;
    bn::regular_bg_map_ptr _bgMap;

    bool _cellsReloadRequired = false;

public:
    DungeonBg();

    void update();

    void redrawAll(const DungeonFloor&);
    void redrawCell(s32 x, s32 y, const DungeonFloor&);

    bool isVisible() const;
    void setVisible(bool isVisible);

private:
    void _initGraphics();
};

} // namespace mp::game
