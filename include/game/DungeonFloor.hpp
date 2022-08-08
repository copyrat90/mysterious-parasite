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

class DungeonFloor final
{
public:
    enum class Type : u8
    {
        FLOOR,
        WALL
    };

    static constexpr s32 ROWS = consts::DUNGEON_FLOOR_SIZE.height();
    static constexpr s32 COLUMNS = consts::DUNGEON_FLOOR_SIZE.width();

private:
    bn::array<u32, 3> _seeds;
    bn::array<bn::array<Type, COLUMNS>, ROWS> _board;

public:
    DungeonFloor();

    Type getTile(s32 x, s32 y);

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
