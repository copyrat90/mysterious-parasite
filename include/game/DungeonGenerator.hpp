/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_algorithm.h"
#include "bn_array.h"
#include "bn_bitset.h"
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

    struct BoardPos
    {
        s8 x, y;
        BoardPos operator+(const BoardPos& other) const;
        BoardPos operator-(const BoardPos& other) const;
        BoardPos operator-() const;
        BoardPos operator*(s8 multiply) const;
        BoardPos& operator+=(const BoardPos& other);
        BoardPos& operator-=(const BoardPos& other);
        bool operator==(const BoardPos& other) const;
    };

    static constexpr s32 CELLULAR_ROOM_MIN_CELLS_COUNT = 16;
    static constexpr s32 CELLULAR_ROOM_MAX_LEN = 16;
    static constexpr s32 SQUARE_ROOM_MIN_LEN = 4;
    static constexpr s32 SQUARE_ROOM_MAX_LEN = 12;
    static constexpr s32 CROSS_ROOM_MIN_LEN = 4;
    static constexpr s32 CROSS_ROOM_MAX_LEN = 12;

    static constexpr s32 ROOM_MAX_LEN =
        bn::max(bn::max(CELLULAR_ROOM_MAX_LEN, SQUARE_ROOM_MAX_LEN), CROSS_ROOM_MAX_LEN);

    struct Room
    {
        bn::vector<bn::vector<FloorType, ROOM_MAX_LEN>, ROOM_MAX_LEN> floors;

        /**
         * @brief local positions of doors.
         * up, down, left, right order.
         */
        bn::array<BoardPos, 4> doors;

        /**
         * @brief Where this room is on the global board.
         */
        BoardPos boardOffset;

        bool isBoardOOB() const;
        bool isBoardFloorOverlap(const Board& board) const;
    };

    static constexpr bn::fixed CELLULAR_INIT_WALL_RATIO = 0.45;

    static constexpr bn::fixed CELLULAR_ROOM_RATIO = 0.4;
    static constexpr bn::fixed SQUARE_ROOM_RATIO = 0.3;
    static constexpr bn::fixed CROSS_ROOM_RATIO = 0.3;

    static_assert(CELLULAR_ROOM_RATIO + SQUARE_ROOM_RATIO + CROSS_ROOM_RATIO <= bn::fixed(1));

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
    bn::vector<BoardPos, ROWS * COLUMNS / 2 + 4> _wallsNearFloor;
    bn::bitset<ROWS * COLUMNS> _wallsNearFloorAdded;

private:
    /**
     * @brief Add adjacent walls from newly created floor to `_wallsNearFloor`.
     *
     * @param floorPos newly created floor position
     */
    void _addAdjacentWallsFromFloor(const BoardPos& floorPos, const Board& board);

    /**
     * @brief Fill whole board with walls.
     */
    void _clearWithWalls(Board& board) const;

    /**
     * @brief Get the direction that the hallway should stretch to.
     * @return `{0,0}` can be returned for more than 1 adjacent wall.
     */
    BoardPos _getHallwayDirection(const BoardPos& wallNearFloor, const Board& board) const;

    /**
     * @brief Add `room` to `board`, by connecting the room to an existing wall near a floor.
     * If it's the first room, it's placed on the center of the board.
     */
    bool _placeRoom(Room& room, Board& board, iso_bn::random& rng);

    Room _createCellularRoom(iso_bn::random& rng) const;
    Room _createSquareRoom(iso_bn::random& rng) const;
    Room _createCrossRoom(iso_bn::random& rng) const;

    /**
     * @brief Get the shortest path length of 2 points on the board.
     * Valid diagonal movements are also considered.
     *
     * @return s32 length of shortest path if exists, `-1` otherwise.
     */
    s32 _shortestPathLen(const BoardPos& p1, const BoardPos& p2, const Board& board) const;
};

} // namespace mp::game
