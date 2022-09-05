/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "game/MetaTileset.hpp"

namespace mp::game
{

struct ShadowTile : MetaTile
{
public:
    // TODO: Add shadow graphics, instead of 2 tile placeholder.
    static constexpr TileIndex SHADOW_TILES_COUNT = 2;

public:
    constexpr ShadowTile() = default;

    constexpr ShadowTile(const bn::regular_bg_map_item& mapItem, s32 shadowTileIndex)
        : MetaTile(mapItem, shadowTileIndex)
    {
        BN_ASSERT(0 <= shadowTileIndex && shadowTileIndex < SHADOW_TILES_COUNT, "Invalid shadowTileIndex(",
                  shadowTileIndex, ")");
    }
};

/**
 * @brief Manages shadow meta-tile tileset info, which consists of the fixed number of `ShadowTile`.
 */
class ShadowTileset
{
public:
    using TileIndex = ShadowTile::TileIndex;
    static constexpr TileIndex TILES_COUNT = ShadowTile::SHADOW_TILES_COUNT;

public:
    static auto get() -> const ShadowTileset&;

public:
    constexpr ShadowTileset() = delete;

    constexpr ShadowTileset(const bn::regular_bg_item& bgItem) : _bgItem(bgItem)
    {
        const auto& mapItem = bgItem.map_item();

        for (s32 i = 0; i < TILES_COUNT; ++i)
            _shadowTiles[i] = ShadowTile(mapItem, i);
    }

    constexpr auto getBgItem() const -> const bn::regular_bg_item&
    {
        return _bgItem;
    }

    /**
     * @brief Get cell by neighbors' brightness & 2x2 bg tile coordinate.
     *
     * @param neighbors neighbors' brightness (center is self)
     * @param bgTileX column coordinate in single ShadowTile [0..2)
     * @param bgTileY row coordinate in single ShadowTile [0..2)
     * @return bn::regular_bg_map_cell
     */
    auto getCell(const DungeonFloor::NeighborBrightness3x3& neighbors, s32 bgTileX, s32 bgTileY) const
        -> bn::regular_bg_map_cell;

private:
    /**
     * @brief Calculate shadow-tile index by looking at the neighbors' brightness.
     *
     * @param neighbors neighbors' brightness (center is self)
     * @return TileIndex
     */
    static TileIndex _calcShadowTileIndex(const DungeonFloor::NeighborBrightness3x3& neighbors);

private:
    const bn::regular_bg_item& _bgItem;
    bn::array<ShadowTile, TILES_COUNT> _shadowTiles;
};

} // namespace mp::game
