/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_array.h"

#include "constants.hpp"

namespace iso_bn
{
class random;
}

namespace mp::game
{

class BoardPos;

/**
 * @brief Manages dungeon terrain info.
 */
class DungeonFloor final
{
public:
    /**
     * @brief Dungeon floor type.
     * Number >= 200 is reserved for the temp flags on the generator.
     */
    enum class Type : u8
    {
        WALL = 0,
        FLOOR = 1,
    };

    static constexpr s32 ROWS = consts::DUNGEON_FLOOR_SIZE.height();
    static constexpr s32 COLUMNS = consts::DUNGEON_FLOOR_SIZE.width();

    using Board = bn::array<bn::array<Type, COLUMNS>, ROWS>;
    using Neighbor3x3 = bn::array<bn::array<Type, 3>, 3>;

private:
    bn::array<u32, 3> _seeds;
    Board _board;

public:
    DungeonFloor();

    Type getFloorTypeOf(s32 x, s32 y) const;
    Type getFloorTypeOf(const BoardPos& pos) const;

    Neighbor3x3 getNeighborsOf(s32 x, s32 y) const;
    Neighbor3x3 getNeighborsOf(const BoardPos& pos) const;

    /**
     * @brief Generate random dungeon floor.
     */
    void generate(iso_bn::random& rng);

    /**
     * @brief Generate random dungeon floor with custom seeds.
     * This is meant to be used for the loaded game.
     *
     * @param seed_x `iso_bn::random` internal seed x.
     * @param seed_y `iso_bn::random` internal seed y.
     * @param seed_z `iso_bn::random` internal seed z.
     */
    void generate(u32 seed_x, u32 seed_y, u32 seed_z);

    auto getSeeds() const
    {
        return _seeds;
    }
};

} // namespace mp::game
