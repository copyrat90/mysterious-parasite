/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/MiniMap.hpp"

#include "bn_affine_bg_map_cell_info.h"
#include "bn_assert.h"
#include "bn_fixed_point.h"
#include "bn_log.h"
#include "bn_profiler.h"

#include "game/DungeonFloor.hpp"
#include "game/mob/Monster.hpp"

#include "bn_affine_bg_items_bg_minimap.h"
#include "bn_sprite_items_spr_minimap_player_cursor.h"

namespace mp::game
{

namespace
{
constexpr s32 PLAYER_CURSOR_FLICKER_FRAMES = 8;
constexpr bn::fixed_point PLAYER_CURSOR_POS = {-40, 0};
} // namespace

enum MiniMap::TileIndex : u8
{
    EMPTY,
    EMPTY_DITHERED,
    NPC,
    ITEM,
    ENEMY,
    BOSS,
    SPECIAL_TILE,
    ELEVATOR,
    TRAP,
    YELLOW_TRIANGE_TILE,

    // Clear type
    WALL1_TOP,
    WALL1_RIGHT,
    WALL1_BOTTOM,
    WALL1_LEFT,
    WALL1_TOP_LEFT,
    WALL1_TOP_RIGHT,
    WALL1_BOTTOM_RIGHT,
    WALL1_BOTTOM_LEFT,
    WALL1_TOP_BOTTOM,
    WALL1_LEFT_RIGHT,
    WALL1_TOP_OPEN,
    WALL1_RIGHT_OPEN,
    WALL1_BOTTOM_OPEN,
    WALL1_LEFT_OPEN,
    WALL1_CLOSED,

    // Shaded type, yet to be explored.
    WALL2_TOP,
    WALL2_RIGHT,
    WALL2_BOTTOM,
    WALL2_LEFT,
    WALL2_TOP_LEFT,
    WALL2_TOP_RIGHT,
    WALL2_BOTTOM_RIGHT,
    WALL2_BOTTOM_LEFT,
    WALL2_TOP_BOTTOM,
    WALL2_LEFT_RIGHT,
    WALL2_TOP_OPEN,
    WALL2_RIGHT_OPEN,
    WALL2_BOTTOM_OPEN,
    WALL2_LEFT_OPEN,
    WALL2_CLOSED,

    // Dithered background type
    WALL3_TOP,
    WALL3_RIGHT,
    WALL3_BOTTOM,
    WALL3_LEFT,
    WALL3_TOP_LEFT,
    WALL3_TOP_RIGHT,
    WALL3_BOTTOM_RIGHT,
    WALL3_BOTTOM_LEFT,
    WALL3_TOP_BOTTOM,
    WALL3_LEFT_RIGHT,
    WALL3_TOP_OPEN,
    WALL3_RIGHT_OPEN,
    WALL3_BOTTOM_OPEN,
    WALL3_LEFT_OPEN,
    WALL3_CLOSED,
};

MiniMap::MiniMap()
    : _cells{}, _mapItem(_cells[0], bn::size(MiniMap::COLUMNS, MiniMap::ROWS)),
      _bgItem(bn::affine_bg_items::bg_minimap.tiles_item(), bn::affine_bg_items::bg_minimap.palette_item(), _mapItem),
      _bg(_bgItem.create_bg(0, 0)), _bgMap(_bg.map()),
      _playerCursor(bn::sprite_items::spr_minimap_player_cursor.create_sprite(PLAYER_CURSOR_POS)),
      _playerCursorFlickerAction(_playerCursor, PLAYER_CURSOR_FLICKER_FRAMES)
{
    _initGraphics();
}

void MiniMap::update()
{
    if (isVisible())
    {
        _playerCursorFlickerAction.update();
    }

    if (_cellsReloadRequired)
    {
        _cellsReloadRequired = false;
        _bgMap.reload_cells_ref();
    }
}

void MiniMap::updateBgPos(const mob::Monster& player)
{
    const auto& playerPos = player.getBoardPos();

    _bg.set_position(PLAYER_CURSOR_POS.x() + COLUMNS * 4 / 2 - 2 - 4 * playerPos.x,
                     PLAYER_CURSOR_POS.y() + ROWS * 4 / 2 - 2 - 4 * playerPos.y);
}

void MiniMap::redrawAll(const DungeonFloor& dungeonFloor)
{
    BN_PROFILER_START("minimap_redraw_all");

    _cellsReloadRequired = true;
    for (s32 y = 0; y < ROWS; ++y)
        for (s32 x = 0; x < COLUMNS; ++x)
            redrawCell(x, y, dungeonFloor);

    BN_PROFILER_STOP();
}

void MiniMap::redrawCell(s32 x, s32 y, const DungeonFloor& dungeonFloor)
{
    _cellsReloadRequired = true;

    bn::affine_bg_map_cell& cell = _cells[_mapItem.cell_index(x, y)];
    bn::affine_bg_map_cell_info cellInfo(cell);

    cellInfo.set_tile_index(_calculateTileIndex(x, y, dungeonFloor));
    cell = cellInfo.cell();
}

bool MiniMap::isVisible() const
{
    return _bg.visible();
}

void MiniMap::setVisible(bool isVisible)
{
    _bg.set_visible(isVisible);
    _playerCursor.set_visible(isVisible);
}

void MiniMap::_initGraphics()
{
    setVisible(false);

    _bg.set_scale(0.5);
    _bg.set_wrapping_enabled(false);
}

MiniMap::TileIndex MiniMap::_calculateTileIndex(s32 x, s32 y, const DungeonFloor& dungeonFloor) const
{
    BN_ASSERT(0 <= x && x < COLUMNS, "Index x(", x, ") OOB");
    BN_ASSERT(0 <= y && y < ROWS, "Index y(", y, ") OOB");

    using Cell = DungeonFloor::Type;

    auto cur = dungeonFloor.getTile(x, y);
    auto up = (y - 1 >= 0) ? dungeonFloor.getTile(x, y - 1) : Cell::WALL;
    auto down = (y + 1 < ROWS) ? dungeonFloor.getTile(x, y + 1) : Cell::WALL;
    auto left = (x - 1 >= 0) ? dungeonFloor.getTile(x - 1, y) : Cell::WALL;
    auto right = (x + 1 < COLUMNS) ? dungeonFloor.getTile(x + 1, y) : Cell::WALL;

    // TODO: Player의 시야 고려하여 dithering 여부 결정
    // TODO: 해당 cell 위에 있는 Actor/Item도 고려하여 그릴 타일 결정
    // TODO: 특수 능력으로 본 지형은 회색 타일로 그림

    TileIndex result = TileIndex::EMPTY;

    if (cur == Cell::FLOOR)
    {
        u32 wallDirectionFlags = ((u32)(up == Cell::WALL) << 3) + ((u32)(down == Cell::WALL) << 2) +
                                 ((u32)(left == Cell::WALL) << 1) + ((u32)(right == Cell::WALL) << 0);

        BN_ASSERT(wallDirectionFlags < 16, "wallDirectionFlags(", wallDirectionFlags, ") OOB");

        constexpr TileIndex WALL_TILE_LUT[16] = {
            EMPTY,
            WALL1_RIGHT,
            WALL1_LEFT,
            WALL1_LEFT_RIGHT,
            WALL1_BOTTOM,
            WALL1_BOTTOM_RIGHT,
            WALL1_BOTTOM_LEFT,
            WALL1_TOP_OPEN,
            WALL1_TOP,
            WALL1_TOP_RIGHT,
            WALL1_TOP_LEFT,
            WALL1_BOTTOM_OPEN,
            WALL1_TOP_BOTTOM,
            WALL1_LEFT_OPEN,
            WALL1_RIGHT_OPEN,
            WALL1_CLOSED,
        };

        result = WALL_TILE_LUT[wallDirectionFlags];
    }

    return result;
}

} // namespace mp::game
