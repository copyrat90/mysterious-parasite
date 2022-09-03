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

#include "game/Direction9.hpp"
#include "typedefs.hpp"

namespace bn
{
class camera_ptr;
}
namespace mp::game::mob
{
class Monster;
}

namespace mp::game
{

class DungeonFloor;
class MetaTileset;
class ShadowTileset;

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
    const ShadowTileset& _shadowTileset;

    bn::regular_bg_map_cell _dunCells[CELLS_COUNT];
    bn::regular_bg_map_cell _shadowCells[CELLS_COUNT];
    bn::regular_bg_map_cell _darkCells[CELLS_COUNT];

    // dungeon tiles, including walls and floors.
    bn::regular_bg_map_item _dunMapItem;
    bn::regular_bg_item _dunBgItem;
    bn::regular_bg_ptr _dunBg;
    bn::regular_bg_map_ptr _dunBgMap;

    // dim shadow, with blending enabled.
    bn::regular_bg_map_item _shadowMapItem;
    bn::regular_bg_item _shadowBgItem;
    bn::regular_bg_ptr _shadowBg;
    bn::regular_bg_map_ptr _shadowBgMap;

    // dark (pitch black) undiscovered area.
    bn::regular_bg_map_item _darkMapItem;
    bn::regular_bg_item _darkBgItem;
    bn::regular_bg_ptr _darkBg;
    bn::regular_bg_map_ptr _darkBgMap;

    bool _cellsReloadRequired = false;

    s32 _bgScrollCountdown = 0;
    Direction9 _bgScrollDirection;

public:
    DungeonBg(const bn::camera_ptr&);

    void update(const DungeonFloor&, const mob::Monster& player);

    void redrawAll(const DungeonFloor&, const mob::Monster& player);

    void startBgScroll(Direction9);
    bool isBgScrollOngoing() const;

    bool isVisible() const;
    void setVisible(bool isVisible);

    void setMetaTileset(const MetaTileset&);

private:
    void _initGraphics(const bn::camera_ptr&);

    void _updateBgScroll(const DungeonFloor&, const mob::Monster& player);

    /**
     * @brief Redraw scrolled cells on screen boundary, by 2 pass.
     * @param scrollPhase [0..1] determine which pass this redraw is.
     */
    void _redrawScrolledCells(s32 scrollPhase, const DungeonFloor&, const mob::Monster& player);

    const bn::camera_ptr& _getCamera() const;
};

} // namespace mp::game
