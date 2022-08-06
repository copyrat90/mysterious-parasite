#include "game/MiniMap.hpp"

#include "bn_affine_bg_map_cell_info.h"

#include "bn_affine_bg_items_bg_minimap.h"
#include "bn_sprite_items_spr_minimap_player_cursor.h"

namespace mp::game
{

namespace
{

constexpr s32 PLAYER_CURSOR_FLICKER_FRAMES = 8;

enum TileIndex : u8
{
    EMPTY = 0,
    EMPTY_DITHERED,
    NPC,
    ITEM,
    ENEMY,
    BOSS,
    SPECIAL_TILE,
    STAIRS,
    TRAP,
    YELLOW_TRIANGE_TILE,

    // Clear type
    WALL1_TOP = 10,
    WALL1_RIGHT,
    WALL1_BOTTOM,
    WALL1_LEFT,
    WALL1_TOP_LEFT,
    WALL1_TOP_RIGHT,
    WALL1_BOTTOM_RIGHT,
    WALL1_BOTTOM_LEFT,
    WALL1_TOP_BOTTOM,
    WALL1_LEFT_RIGHT,

    // Shaded type, yet to be explored.
    WALL2_TOP = 20,
    WALL2_RIGHT,
    WALL2_BOTTOM,
    WALL2_LEFT,
    WALL2_TOP_LEFT,
    WALL2_TOP_RIGHT,
    WALL2_BOTTOM_RIGHT,
    WALL2_BOTTOM_LEFT,
    WALL2_TOP_BOTTOM,
    WALL2_LEFT_RIGHT,

    // Dithered background type
    WALL3_TOP = 30,
    WALL3_RIGHT,
    WALL3_BOTTOM,
    WALL3_LEFT,
    WALL3_TOP_LEFT,
    WALL3_TOP_RIGHT,
    WALL3_BOTTOM_RIGHT,
    WALL3_BOTTOM_LEFT,
    WALL3_TOP_BOTTOM,
    WALL3_LEFT_RIGHT,
};

} // namespace

MiniMap::MiniMap()
    : _cells{}, _mapItem(_cells[0], bn::size(MiniMap::COLUMNS, MiniMap::ROWS)),
      _bgItem(bn::affine_bg_items::bg_minimap.tiles_item(), bn::affine_bg_items::bg_minimap.palette_item(), _mapItem),
      _bg(_bgItem.create_bg(0, 0)), _bgMap(_bg.map()),
      _playerCursor(bn::sprite_items::spr_minimap_player_cursor.create_sprite(0, 0)),
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
}

bool MiniMap::isVisible()
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

    // TODO: 미니맵 _bg를 현재 플레이어 커서가 중앙이 되도록 위치 초기화
}

} // namespace mp::game
