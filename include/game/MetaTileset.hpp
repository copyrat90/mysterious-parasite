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

    static constexpr bn::size SIZE_IN_PIXELS = {16, 16};

    /**
     * @brief Number of meta-tiles in a tileset.
     * Includes empty tile as index 0.
     */
    static constexpr TileIndex TILES_COUNT = 33;

    static constexpr s32 ROWS = 2;
    static constexpr s32 COLUMNS = 2;
    static constexpr s32 CELLS_COUNT = ROWS * COLUMNS;

public:
    constexpr MetaTile() = default;

    constexpr MetaTile(const bn::regular_bg_map_item& mapItem, s32 metaTileIndex)
    {
        BN_ASSERT(0 <= metaTileIndex && metaTileIndex < TILES_COUNT, "Invalid metaTileIndex(", metaTileIndex, ")");

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
 * @brief Manages dungeon meta-tile tileset info, which consists of the fixed number of `MetaTile`s.
 */
class MetaTileset
{
public:
    using TileIndex = MetaTile::TileIndex;
    static constexpr TileIndex TILES_COUNT = MetaTile::TILES_COUNT;

public:
    static auto fromKind(MetaTilesetKind) -> const MetaTileset&;

public:
    constexpr MetaTileset() = delete;

    constexpr MetaTileset(const bn::regular_bg_item& bgItem) : _bgItem(bgItem)
    {
        const auto& mapItem = bgItem.map_item();

        for (s32 i = 0; i < TILES_COUNT; ++i)
            _metaTiles[i] = MetaTile(mapItem, i);
    }

    constexpr auto getBgItem() const -> const bn::regular_bg_item&
    {
        return _bgItem;
    }

    /**
     * @brief Get cell by neighbor floors & 2x2 bg tile coordinate.
     *
     * @param neighbors neighbor floors (center is self)
     * @param bgTileX column coordinate in single MetaTile [0..2)
     * @param bgTileY row coordinate in single MetaTile [0..2)
     * @return bn::regular_bg_map_cell
     */
    auto getCell(const bn::array<bn::array<DungeonFloor::Type, 3>, 3>& neighbors, s32 bgTileX, s32 bgTileY) const
        -> bn::regular_bg_map_cell;

private:
    /**
     * @brief Calculate meta-tile index by looking at the floor's neighbors.
     *
     * @param neighbors neighbor floors (center is self)
     * @return TileIndex
     */
    static TileIndex _calcMetaTileIndex(const bn::array<bn::array<DungeonFloor::Type, 3>, 3>& neighbors);

private:
    const bn::regular_bg_item& _bgItem;
    bn::array<MetaTile, TILES_COUNT> _metaTiles;
};

} // namespace mp::game
