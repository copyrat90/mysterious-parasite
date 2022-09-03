/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/DungeonFloor.hpp"

#include "bn_assert.h"

#include "iso_bn_random.h"

#include "game/DungeonGenerator.hpp"

namespace mp::game
{

DungeonFloor::DungeonFloor() : _seeds({0, 0, 0}), _board{}, _brightnesses{}, _discoverBoard{}
{
}

auto DungeonFloor::getFloorTypeOf(s32 x, s32 y) const -> Type
{
    if (x < 0 || y < 0 || x >= COLUMNS || y >= ROWS)
        return Type::WALL;

    return _board[y][x];
}

auto DungeonFloor::getFloorTypeOf(const BoardPos& pos) const -> Type
{
    return getFloorTypeOf(pos.x, pos.y);
}

auto DungeonFloor::getNeighborsOf(s32 x, s32 y) const -> Neighbor3x3
{
    Neighbor3x3 result;
    for (s32 iy = 0; iy < 3; ++iy)
    {
        const s32 py = y + iy - 1;
        for (s32 ix = 0; ix < 3; ++ix)
        {
            const s32 px = x + ix - 1;
            result[iy][ix] = getFloorTypeOf(px, py);
        }
    }
    return result;
}

auto DungeonFloor::getNeighborsOf(const BoardPos& pos) const -> Neighbor3x3
{
    return getNeighborsOf(pos.x, pos.y);
}

s8 DungeonFloor::getBrightnessOf(s32 x, s32 y) const
{
    if (x < 0 || y < 0 || x >= COLUMNS || y >= ROWS)
        return 0;

    return _brightnesses[y][x];
}

s8 DungeonFloor::getBrightnessOf(const BoardPos& pos) const
{
    return getBrightnessOf(pos.x, pos.y);
}

auto DungeonFloor::getNeighborBrightnessOf(s32 x, s32 y) const -> NeighborBrightness3x3
{
    NeighborBrightness3x3 result;
    for (s32 iy = 0; iy < 3; ++iy)
    {
        const s32 py = y + iy - 1;
        for (s32 ix = 0; ix < 3; ++ix)
        {
            const s32 px = x + ix - 1;
            result[iy][ix] = getBrightnessOf(px, py);
        }
    }
    return result;
}

auto DungeonFloor::getNeighborBrightnessOf(const BoardPos& pos) const -> NeighborBrightness3x3
{
    return getNeighborBrightnessOf(pos.x, pos.y);
}

bool DungeonFloor::getDiscoverOf(s32 x, s32 y) const
{
    if (x < 0 || y < 0 || x >= COLUMNS || y >= ROWS)
        return true;

    return _discoverBoard[y][x];
}

bool DungeonFloor::getDiscoverOf(const BoardPos& pos) const
{
    return getDiscoverOf(pos.x, pos.y);
}

auto DungeonFloor::getNeighborDiscoverOf(s32 x, s32 y) const -> NeighborDiscover3x3
{
    NeighborDiscover3x3 result;
    for (s32 iy = 0; iy < 3; ++iy)
    {
        const s32 py = y + iy - 1;
        for (s32 ix = 0; ix < 3; ++ix)
        {
            const s32 px = x + ix - 1;
            result[iy][ix] = getDiscoverOf(px, py);
        }
    }
    return result;
}

auto DungeonFloor::getNeighborDiscoverOf(const BoardPos& pos) const -> NeighborDiscover3x3
{
    return getNeighborDiscoverOf(pos.x, pos.y);
}

void DungeonFloor::generate(iso_bn::random& rng)
{
    // Save current seed to generate this floor identically for the loaded game.
    _seeds = {rng.seed_x(), rng.seed_y(), rng.seed_z()};

    DungeonGenerator gen;
    gen.generate(_board, rng);
}

void DungeonFloor::generate(u32 seed_x, u32 seed_y, u32 seed_z)
{
    iso_bn::random rng;
    rng.set_seed(seed_x, seed_y, seed_z);

    generate(rng);
}

} // namespace mp::game
