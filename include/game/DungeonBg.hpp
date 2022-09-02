/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_fixed_point.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"

#include "typedefs.hpp"

namespace bn
{
class camera_ptr;
}

namespace mp::game
{

class DungeonFloor;
class MetaTileset;

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
    const DungeonFloor& _dungeonFloor;

    bn::regular_bg_map_cell _cells[CELLS_COUNT];
    bn::regular_bg_map_item _mapItem;
    bn::regular_bg_item _bgItem;
    bn::regular_bg_ptr _bg;
    bn::regular_bg_map_ptr _bgMap;

    bool _cellsReloadRequired = false;

    bn::fixed_point _prevCamPos;

public:
    DungeonBg(const bn::camera_ptr&, const DungeonFloor&);

    void update();

    void redrawAll();
    // void redrawCell(s32 x, s32 y);

    bool isVisible() const;
    void setVisible(bool isVisible);

    void setMetaTileset(const MetaTileset&);

private:
    void _initGraphics(const bn::camera_ptr&);

    const bn::camera_ptr& _getCamera() const;

    /**
     * @brief only redraw the update-required cells.
     */
    void _redrawNecessaryCells();
};

} // namespace mp::game
