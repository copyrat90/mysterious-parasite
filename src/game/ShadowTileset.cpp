/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */
#include "game/ShadowTileset.hpp"

#include "bn_regular_bg_items_bg_shadow_tileset.h"

namespace mp::game
{

namespace
{
constexpr ShadowTileset _shadowTileset(bn::regular_bg_items::bg_shadow_tileset);
}

auto ShadowTileset::get() -> const ShadowTileset&
{
    return _shadowTileset;
}

auto ShadowTileset::getCell(const DungeonFloor::NeighborBrightness3x3& neighbors, s32 bgTileX, s32 bgTileY) const
    -> bn::regular_bg_map_cell
{
    TileIndex idx = _calcShadowTileIndex(neighbors);
    return _shadowTiles[idx].getCell(bgTileX, bgTileY);
}

auto ShadowTileset::getCell(const DungeonFloor::NeighborDiscover3x3& neighbors, s32 bgTileX, s32 bgTileY) const
    -> bn::regular_bg_map_cell
{
    TileIndex idx = _calcDarkTileIndex(neighbors);
    return _shadowTiles[idx].getCell(bgTileX, bgTileY);
}

auto ShadowTileset::_calcShadowTileIndex(const DungeonFloor::NeighborBrightness3x3& neighbors) -> TileIndex
{
    // TODO: Replace with actual shadow graphics, instead of 2 tile placeholder.
    if (neighbors[1][1] > 0)
        return 0; // light

    return 1; // shadow
}

auto ShadowTileset::_calcDarkTileIndex(const DungeonFloor::NeighborDiscover3x3& neighbors) -> TileIndex
{
    return 0; // [TEST] light only

    // // TODO: Replace with actual shadow graphics, instead of 2 tile placeholder.
    // if (neighbors[1][1])
    //     return 0; // light

    // return 1; // dark
}

} // namespace mp::game
