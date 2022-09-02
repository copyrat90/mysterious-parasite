/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/MetaTileset.hpp"

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

auto MetaTileset::getCell(const bn::array<bn::array<DungeonFloor::Type, 3>, 3>& neighbors, s32 bgTileX,
                          s32 bgTileY) const -> bn::regular_bg_map_cell
{
    TileIndex idx = _calcMetaTileIndex(neighbors);
    return _metaTiles[idx].getCell(bgTileX, bgTileY);
}

auto MetaTileset::_calcMetaTileIndex(const bn::array<bn::array<DungeonFloor::Type, 3>, 3>& neighbors) -> TileIndex
{
    BN_ERROR("WIP: should be changed from 3x3 to 2x2 meta-tile!");

    using FloorType = DungeonFloor::Type;

    // if center tile is a floor, just return the floor tile.
    // this can be changed in the future, if you want a various floors with different neighbor walls.
    if (neighbors[1][1] == FloorType::FLOOR)
        return 1;

    // top-left to bottom-right wall flags
    s32 wallFlags = 0;
    for (s32 y = 0; y < 3; ++y)
        for (s32 x = 0; x < 3; ++x)
            if (neighbors[y][x] == FloorType::WALL)
                wallFlags += (1 << (3 * y + x));

    switch (wallFlags)
    {
    case (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8):
        return 2;
    case (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7) + (1 << 8):
        return 3;
    case (1 << 3) + (1 << 4) + (1 << 6) + (1 << 7):
        return 4;
    case (1 << 1) + (1 << 2) + (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8):
        return 5;
    case 511:
        return 6;
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 6) + (1 << 7):
        return 7;
    case (1 << 1) + (1 << 2) + (1 << 4) + (1 << 5):
        return 8;
    case (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5):
        return 9;
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4):
        return 10;
    case (1 << 4) + (1 << 5) + (1 << 7):
        return 11;
    case (1 << 3) + (1 << 4) + (1 << 5):
        return 12;
    case (1 << 3) + (1 << 4) + (1 << 7):
        return 13;
    case (1 << 1) + (1 << 4) + (1 << 7):
        return 14;
    case (1 << 4):
        return 15;
    case (1 << 1) + (1 << 4) + (1 << 5):
        return 16;
    case (1 << 1) + (1 << 3) + (1 << 4):
        return 17;
    case (1 << 4) + (1 << 7):
        return 18;
    case (1 << 4) + (1 << 5):
        return 19;
    case (1 << 1) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 7):
        return 20;
    case (1 << 3) + (1 << 4):
        return 21;
    case (1 << 1) + (1 << 4):
        return 22;
    case (1 << 3) + (1 << 4) + (1 << 5) + (1 << 7):
        return 23;
    case (1 << 1) + (1 << 4) + (1 << 5) + (1 << 7):
        return 24;
    case (1 << 1) + (1 << 3) + (1 << 4) + (1 << 7):
        return 25;
    case (1 << 1) + (1 << 3) + (1 << 4) + (1 << 5):
        return 26;
    case (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 7):
        return 27;
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7):
        return 28;
    case (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8):
        return 29;
    case (1 << 1) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7) + (1 << 8):
        return 30;
    case (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7):
        return 31;
    case (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8):
        return 32;
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7) + (1 << 8):
        return 33;
    case (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7) + (1 << 8):
        return 34;
    case (1 << 1) + (1 << 2) + (1 << 4) + (1 << 5) + (1 << 7):
        return 35;
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 7):
        return 36;
    case (1 << 1) + (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8):
        return 37;
    case (1 << 1) + (1 << 3) + (1 << 4) + (1 << 6) + (1 << 7):
        return 38;
    case (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7):
        return 39;
    case (1 << 3) + (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8):
        return 40;
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 5):
        return 41;
    case (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5):
        return 42;
    case (1 << 1) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8):
        return 43;
    case (1 << 1) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7):
        return 44;
    case (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 7):
        return 45;
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 7):
        return 46;
    case (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7):
        return 47;
    case (1 << 0) + (1 << 1) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 7) + (1 << 8):
        return 48;
    default:
        BN_ERROR("invalid neighbor wall flag(", wallFlags, ")");
    }
    return 0;
}

} // namespace mp::game
