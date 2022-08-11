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

namespace
{
constexpr s32 FLOOR_WIDTH = consts::DUNGEON_FLOOR_SIZE.width();
constexpr s32 FLOOR_HEIGHT = consts::DUNGEON_FLOOR_SIZE.height();
} // namespace

DungeonFloor::DungeonFloor() : _seeds({0, 0, 0})
{
}

DungeonFloor::Type DungeonFloor::getTile(s32 x, s32 y)
{
    BN_ASSERT(0 <= x && x < FLOOR_WIDTH, "Tile OOB for x(", x, ")");
    BN_ASSERT(0 <= y && y < FLOOR_HEIGHT, "Tile OOB for y(", y, ")");

    return _board[y][x];
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
