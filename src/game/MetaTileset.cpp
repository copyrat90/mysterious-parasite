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

namespace mp::game
{

namespace
{

constexpr s32 TOTAL_TILESETS = (s32)MetaTilesetKind::TOTAL_TILESETS;

constexpr MetaTileset _metaTilesets[TOTAL_TILESETS] = {
    MetaTileset(bn::regular_bg_items::bg_dungeon_tileset_placeholder),
};

} // namespace

auto MetaTileset::fromKind(MetaTilesetKind kind) -> const MetaTileset&
{
    BN_ASSERT(0 <= (s32)kind && (s32)kind < TOTAL_TILESETS, "Invalid MetaTileset::Kind(", (s32)kind, ")");

    return _metaTilesets[(s32)kind];
}

auto MetaTile::getCell(s32 bgTileX, s32 bgTileY) const -> bn::regular_bg_map_cell
{
    BN_ASSERT(0 <= bgTileX && bgTileX < COLUMNS, "MetaTile tileX(", bgTileX, ") OOB");
    BN_ASSERT(0 <= bgTileY && bgTileY < ROWS, "MetaTile tileY(", bgTileY, ") OOB");

    return cells[bgTileY * COLUMNS + bgTileX];
}

auto MetaTileset::getCell(const DungeonFloor::Neighbor3x3& neighbors, s32 bgTileX, s32 bgTileY) const
    -> bn::regular_bg_map_cell
{
    TileIndex idx = _calcMetaTileIndex(neighbors);
    return _metaTiles[idx].getCell(bgTileX, bgTileY);
}

auto MetaTileset::_calcMetaTileIndex(const DungeonFloor::Neighbor3x3& neighbors) -> TileIndex
{
    using FloorType = DungeonFloor::Type;

    // TODO: load wall/floor variations, instead of default one.

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
        BN_LOG("invalid neighbor wall flag(", wallFlags, ")");
    }
    return 0;
}

} // namespace mp::game
