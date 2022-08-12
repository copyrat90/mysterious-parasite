/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_algorithm.h"
#include "bn_fixed.h"
#include "bn_vector.h"

#include "game/DungeonFloor.hpp"

namespace iso_bn
{
class random;
}

namespace mp::game
{

class DungeonGenerator final
{
public:
    using Board = DungeonFloor::Board;
    using FloorType = DungeonFloor::Type;

    static constexpr s32 ROWS = DungeonFloor::ROWS;
    static constexpr s32 COLUMNS = DungeonFloor::COLUMNS;

    static constexpr s32 CELLULAR_ROOM_MIN_CELLS_COUNT = 16;
    static constexpr s32 CELLULAR_ROOM_MAX_LEN = 16;
    static constexpr s32 SQUARE_ROOM_MIN_LEN = 6;
    static constexpr s32 SQUARE_ROOM_MAX_LEN = 12;
    static constexpr s32 CROSS_ROOM_MIN_LEN = 4;
    static constexpr s32 CROSS_ROOM_MAX_LEN = 12;

    static constexpr s32 ROOM_MAX_LEN =
        bn::max(bn::max(CELLULAR_ROOM_MAX_LEN, SQUARE_ROOM_MAX_LEN), CROSS_ROOM_MAX_LEN);

    using Room = bn::vector<bn::vector<FloorType, ROOM_MAX_LEN>, ROOM_MAX_LEN>;

    static constexpr bn::fixed CELLULAR_WALL_RATIO = 0.45;

    enum CandidateFlag : u8
    {
        DOOR = 200,
    };

public:
    /**
     * @brief Generate random dungeon floor.
     * Inspired by algorithm described in https://www.rockpapershotgun.com/how-do-roguelikes-generate-levels
     */
    void generate(Board& board, iso_bn::random& rng);

private:
    /**
     * @brief Fill whole board with walls.
     */
    void _clearWithWalls(Board& board);

    bool _placeRoom(Room& room);

    Room _createCellularRoom(iso_bn::random& rng);
    Room _createSquareRoom(iso_bn::random& rng);
    Room _createCrossRoom(iso_bn::random& rng);
};

} // namespace mp::game
