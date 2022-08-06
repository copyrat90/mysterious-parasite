#pragma once

#include "bn_affine_bg_item.h"
#include "bn_affine_bg_map_item.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_ptr.h"

#include "typedefs.hpp"

namespace mp::game
{

class MiniMap final
{
public:
    static constexpr s32 ROWS = 128, COLUMNS = 128;
    static constexpr s32 CELLS_COUNT = ROWS * COLUMNS;

private:
    bn::affine_bg_map_cell _cells[CELLS_COUNT];
    bn::affine_bg_map_item _mapItem;
    bn::affine_bg_item _bgItem;
    bn::affine_bg_ptr _bg;
    bn::affine_bg_map_ptr _bgMap;

    bn::sprite_ptr _playerCursor;
    bn::sprite_visible_toggle_action _playerCursorFlickerAction;

public:
    MiniMap();

    void update();

    bool isVisible();
    void setVisible(bool isVisible);

private:
    void _initGraphics();
};

} // namespace mp::game
