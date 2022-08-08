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

class DungeonFloor;

class MiniMap final
{
public:
    enum TileIndex : u8;

    static constexpr s32 ROWS = consts::DUNGEON_FLOOR_SIZE.height();
    static constexpr s32 COLUMNS = consts::DUNGEON_FLOOR_SIZE.width();
    static constexpr s32 CELLS_COUNT = consts::DUNGEON_FLOOR_CELLS_COUNT;

private:
    bn::affine_bg_map_cell _cells[CELLS_COUNT];
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

    void redrawAll(DungeonFloor& dungeonFloor);
    void redrawCell(s32 x, s32 y, DungeonFloor& dungeonFloor);

    bool isVisible();
    void setVisible(bool isVisible);

private:
    void _initGraphics();

    TileIndex _calculateTileIndex(s32 x, s32 y, DungeonFloor& dungeonFloor);
};

} // namespace mp::game
