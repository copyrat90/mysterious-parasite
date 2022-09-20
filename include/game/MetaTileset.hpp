/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_array.h"
#include "bn_assert.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_regular_bg_map_item.h"
#include "bn_size.h"

#include "game/DungeonFloor.hpp"
#include "game/MetaTilesetKind.hpp"
#include "typedefs.hpp"

namespace mp::game
{

/**
 * @brief Holds 2x2 meta-tile info.
 * (A Meta-tile consists of 2x2 regular_bg tiles, which makes up a single dungeon tile.)
 */
struct MetaTile
{
public:
    using TileIndex = u8;

    static constexpr bn::size SIZE_IN_PIXELS = consts::DUNGEON_META_TILE_SIZE;

    /**
     * @brief Max number of meta-tiles in a tileset.
     */
    static constexpr s32 MAX_TILES_COUNT = 256;

    static constexpr s32 ROWS = 2;
    static constexpr s32 COLUMNS = 2;
    static constexpr s32 CELLS_COUNT = ROWS * COLUMNS;

public:
    constexpr MetaTile() = default;

    constexpr MetaTile(const bn::regular_bg_map_item& mapItem, s32 metaTileIndex)
    {
        BN_ASSERT(0 <= metaTileIndex && metaTileIndex < MAX_TILES_COUNT, "Invalid metaTileIndex(", metaTileIndex, ")");

        cells = mapItem.cells_ptr() + metaTileIndex * CELLS_COUNT;
    }

    /**
     * @brief Get cell by 2x2 bg tile coordinate.
     *
     * @param bgTileX column coordinate [0..2)
     * @param bgTileY row coordinate [0..2)
     * @return bn::regular_bg_map_cell
     */
    auto getCell(s32 bgTileX, s32 bgTileY) const -> bn::regular_bg_map_cell;

public:
    const bn::regular_bg_map_cell* cells;
};

/**
 * @brief Manages dungeon meta-tile tileset info, which consists of the fixed number of `MetaTile`.
 */
class MetaTileset
{
public:
    using TileIndex = MetaTile::TileIndex;
    static constexpr s32 MAX_TILES_COUNT = MetaTile::MAX_TILES_COUNT;

public:
    static auto fromKind(MetaTilesetKind) -> const MetaTileset&;

public:
    constexpr MetaTileset() = delete;

    constexpr MetaTileset(MetaTilesetKind kind, const bn::regular_bg_item& bgItem) : _kind(kind), _bgItem(bgItem)
    {
        const auto& mapItem = bgItem.map_item();

        for (s32 i = 0; i < MAX_TILES_COUNT; ++i)
            _metaTiles[i] = MetaTile(mapItem, i);
    }

    constexpr auto getKind() const -> MetaTilesetKind
    {
        return _kind;
    }

    constexpr auto getBgItem() const -> const bn::regular_bg_item&
    {
        return _bgItem;
    }

    /**
     * @brief Get cell by meta-tile index
     *
     * @param metaTileIndex
     * @param bgTileX column coordinate in single MetaTile [0..2)
     * @param bgTileY row coordinate in single MetaTile [0..2)
     * @return bn::regular_bg_map_cell
     */
    auto getCell(TileIndex metaTileIndex, s32 bgTileX, s32 bgTileY) const -> bn::regular_bg_map_cell;

    /**
     * @brief Get dungeon cell by neighbor floors, their discover status & 2x2 bg tile coordinate.
     *
     * @param neighbors neighbor floors (center is self)
     * @param discovers neighbors' discover status (center is self)
     * @param bgTileX column coordinate in single MetaTile [0..2)
     * @param bgTileY row coordinate in single MetaTile [0..2)
     * @return bn::regular_bg_map_cell
     */
    auto getDungeonCell(const DungeonFloor::Neighbor3x3& neighbors, const DungeonFloor::NeighborDiscover3x3& discovers,
                        s32 bgTileX, s32 bgTileY) const -> bn::regular_bg_map_cell;

    /**
     * @brief Get shadow cell by neighbors' brightness & 2x2 bg tile coordinate.
     *
     * @param neighbors neighbors' brightness (center is self)
     * @param bgTileX column coordinate in single ShadowTile [0..2)
     * @param bgTileY row coordinate in single ShadowTile [0..2)
     * @return bn::regular_bg_map_cell
     */
    auto getShadowCell(const DungeonFloor::NeighborBrightness3x3& neighbors, s32 bgTileX, s32 bgTileY) const
        -> bn::regular_bg_map_cell;

private:
    /**
     * @brief Calculate dungeon's meta-tile index by looking at the floor's neighbors.
     *
     * @param neighbors neighbor floors (center is self)
     * @param discovers neighbors' discover status (center is self)
     * @return TileIndex
     */
    static TileIndex _calcDungeonTileIndex(const DungeonFloor::Neighbor3x3& neighbors,
                                           const DungeonFloor::NeighborDiscover3x3& discovers);

    /**
     * @brief Calculate shadow-tile index by looking at the neighbors' brightness.
     *
     * @param neighbors neighbors' brightness (center is self)
     * @return TileIndex
     */
    static TileIndex _calcShadowTileIndex(const DungeonFloor::NeighborBrightness3x3& neighbors);

private:
    const MetaTilesetKind _kind;
    const bn::regular_bg_item& _bgItem;
    bn::array<MetaTile, MAX_TILES_COUNT> _metaTiles;
};

} // namespace mp::game
