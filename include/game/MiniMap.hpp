/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_affine_bg_item.h"
#include "bn_affine_bg_map_item.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_ptr.h"

#include "constants.hpp"
#include "typedefs.hpp"

namespace mp::game
{

namespace mob
{
class Monster;
}

class DungeonFloor;

class MiniMap final
{
private:
    enum TileIndex : u8;

public:
    static constexpr s32 ROWS = consts::DUNGEON_FLOOR_SIZE.height();
    static constexpr s32 COLUMNS = consts::DUNGEON_FLOOR_SIZE.width();
    static constexpr s32 CELLS_COUNT = consts::DUNGEON_FLOOR_CELLS_COUNT;

private:
    alignas(4) bn::affine_bg_map_cell _cells[CELLS_COUNT];
    bn::affine_bg_map_item _mapItem;
    bn::affine_bg_item _bgItem;
    bn::affine_bg_ptr _bg;
    bn::affine_bg_map_ptr _bgMap;

    bn::sprite_ptr _playerCursor;
    bn::sprite_visible_toggle_action _playerCursorFlickerAction;

    bool _cellsReloadRequired = false;

public:
    MiniMap();

    void update();
    void updateBgPos(const mob::Monster& player);

    void redrawAll(const DungeonFloor&);
    void redrawCell(s32 x, s32 y, const DungeonFloor&);

    bool isVisible() const;
    void setVisible(bool isVisible);

private:
    void _initGraphics();

    TileIndex _calculateTileIndex(s32 x, s32 y, const DungeonFloor&) const;
};

} // namespace mp::game
