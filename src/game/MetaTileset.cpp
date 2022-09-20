/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/MetaTileset.hpp"

#include "bn_log.h"

#include "bn_regular_bg_items_bg_dungeon_tileset_placeholder.h"
#include "bn_regular_bg_items_bg_shadow_tileset.h"
#include "bn_regular_bg_items_bg_toss_hint_tileset.h"

namespace mp::game
{

namespace
{

constexpr s32 TOTAL_TILESETS = (s32)MetaTilesetKind::TOTAL_TILESETS;

constexpr MetaTileset _metaTilesets[TOTAL_TILESETS] = {
    // This order should be same on the `enum class MetaTilesetKind`.
    MetaTileset(MetaTilesetKind::DUNGEON, bn::regular_bg_items::bg_dungeon_tileset_placeholder),
    MetaTileset(MetaTilesetKind::SHADOW, bn::regular_bg_items::bg_shadow_tileset),
};

} // namespace

auto MetaTileset::fromKind(MetaTilesetKind kind) -> const MetaTileset&
{
    BN_ASSERT((s32)kind < TOTAL_TILESETS, "Invalid MetaTileset::Kind(", (s32)kind, ")");

    return _metaTilesets[(s32)kind];
}

auto MetaTile::getCell(s32 bgTileX, s32 bgTileY) const -> bn::regular_bg_map_cell
{
    BN_ASSERT(0 <= bgTileX && bgTileX < COLUMNS, "MetaTile tileX(", bgTileX, ") OOB");
    BN_ASSERT(0 <= bgTileY && bgTileY < ROWS, "MetaTile tileY(", bgTileY, ") OOB");

    return cells[bgTileY * COLUMNS + bgTileX];
}

auto MetaTileset::getCell(TileIndex metaTileIndex, s32 bgTileX, s32 bgTileY) const -> bn::regular_bg_map_cell
{
    return _metaTiles[metaTileIndex].getCell(bgTileX, bgTileY);
}

auto MetaTileset::getDungeonCell(const DungeonFloor::Neighbor3x3& neighbors,
                                 const DungeonFloor::NeighborDiscover3x3& discovers, s32 bgTileX, s32 bgTileY) const
    -> bn::regular_bg_map_cell
{
    BN_ASSERT(getKind() == MetaTilesetKind::DUNGEON, "MetaTilesetKind(", (s32)_kind, ") is not DUNGEON");

    TileIndex idx = _calcDungeonTileIndex(neighbors, discovers);
    return getCell(idx, bgTileX, bgTileY);
}

auto MetaTileset::getShadowCell(const DungeonFloor::NeighborBrightness3x3& neighbors, s32 bgTileX, s32 bgTileY) const
    -> bn::regular_bg_map_cell
{
    BN_ASSERT(getKind() == MetaTilesetKind::SHADOW, "MetaTilesetKind(", (s32)_kind, ") is not SHADOW");

    TileIndex idx = _calcShadowTileIndex(neighbors);
    return getCell(idx, bgTileX, bgTileY);
}

auto MetaTileset::_calcShadowTileIndex(const DungeonFloor::NeighborBrightness3x3& neighbors) -> TileIndex
{
    // TODO: Replace with actual shadow graphics, instead of 2 tile placeholder.
    if (neighbors[1][1] > 0)
        return 0; // light

    return 1; // shadow
}

auto MetaTileset::_calcDungeonTileIndex(const DungeonFloor::Neighbor3x3& neighbors,
                                        const DungeonFloor::NeighborDiscover3x3& discovers) -> TileIndex
{
    using FloorType = DungeonFloor::Type;

    // TODO: load wall/floor variations, instead of default one.
    // TODO: darken the floors when they are not discovered yet.

    // if center tile is a floor, just return the floor tile.
    if (neighbors[1][1] == FloorType::FLOOR)
        return 26;

    // top-left to bottom-right wall flags
    s32 wallFlags = 0;
    for (s32 y = 0; y < 3; ++y)
        for (s32 x = 0; x < 3; ++x)
            if (neighbors[y][x] == FloorType::WALL)
                wallFlags += (1 << (3 * y + x));

    switch (wallFlags)
    {
    // full wall
    case 511:
        return 1;

    // corner (inner) walls
    case (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7):
        return 6; // top-left
    case (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8):
        return 7; // top-right
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7) + (1 << 8):
        return 12; // bottom-left
    case (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7) + (1 << 8):
        return 13; // bottom-right

    // corner (outer) walls
    case (1 << 1) + (1 << 2) + (1 << 4) + (1 << 5):
    case (1 << 1) + (1 << 2) + (1 << 4) + (1 << 5) + (1 << 0):
    case (1 << 1) + (1 << 2) + (1 << 4) + (1 << 5) + (1 << 8):
    case (1 << 1) + (1 << 2) + (1 << 4) + (1 << 5) + (1 << 0) + (1 << 8):
        return 22; // bottom-left
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4):
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 2):
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 6):
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 2) + (1 << 6):
        return 23; // bottom-right
    case (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8):
    case (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8) + (1 << 2):
    case (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8) + (1 << 6):
    case (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8) + (1 << 2) + (1 << 6):
        return 24; // top-left
    case (1 << 3) + (1 << 4) + (1 << 6) + (1 << 7):
    case (1 << 3) + (1 << 4) + (1 << 6) + (1 << 7) + (1 << 0):
    case (1 << 3) + (1 << 4) + (1 << 6) + (1 << 7) + (1 << 8):
    case (1 << 3) + (1 << 4) + (1 << 6) + (1 << 7) + (1 << 0) + (1 << 8):
        return 25; // top-right

    // straight walls
    case (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5):
    case (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6):
    case (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 8):
    case (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 8):
        return 2; // top
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 6) + (1 << 7):
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 6) + (1 << 7) + (1 << 2):
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 6) + (1 << 7) + (1 << 8):
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 6) + (1 << 7) + (1 << 2) + (1 << 8):
        return 14; // left
    case (1 << 1) + (1 << 2) + (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8):
    case (1 << 1) + (1 << 2) + (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8) + (1 << 0):
    case (1 << 1) + (1 << 2) + (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8) + (1 << 6):
    case (1 << 1) + (1 << 2) + (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8) + (1 << 0) + (1 << 6):
        return 18; // right
    case (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7) + (1 << 8):
    case (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7) + (1 << 8) + (1 << 0):
    case (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7) + (1 << 8) + (1 << 2):
    case (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7) + (1 << 8) + (1 << 0) + (1 << 2):
        return 8; // bottom

    default:
        // BN_LOG("invalid neighbor wall flag(", wallFlags, ")");
        break;
    }
    return 0;
}

} // namespace mp::game
