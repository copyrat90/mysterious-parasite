/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/DungeonGenerator.hpp"

#include "bn_assert.h"
#include "bn_memory.h"
#include "bn_point.h"
#include "bn_profiler.h"
#include "bn_vector.h"

#include "iso_bn_random.h"

namespace mp::game
{

namespace
{
constexpr s32 SMOOTHING_COUNT = 5;
constexpr s32 CELLULAR_ROOM_FAIL_FALLBACK_COUNT = 3;
} // namespace

void DungeonGenerator::generate(Board& board, iso_bn::random& rng)
{
    BN_PROFILER_START("dungeon_gen");

    _clearWithWalls(board);

    // test
    Room room = _createCellularRoom(rng);
    for (s32 y = 0; y < room.size(); ++y)
        for (s32 x = 0; x < room[0].size(); ++x)
            board[ROWS / 2 - room.size() / 2 + y][COLUMNS / 2 - room[0].size() + x] = room[y][x];
    // test

    BN_PROFILER_STOP();
}

void DungeonGenerator::_clearWithWalls(Board& board)
{
    if constexpr ((u8)FloorType::WALL == 0)
        bn::memory::clear(ROWS * COLUMNS, board[0][0]);
    else
        bn::memory::set_bytes((u8)FloorType::WALL, ROWS * COLUMNS, &board[0][0]);
}

bool DungeonGenerator::_placeRoom(Room& room)
{
}

static void _debugLogRoom(const DungeonGenerator::Room& room)
{
#if BN_CFG_LOG_ENABLED
    for (const auto& row : room)
    {
        char _bn_string[BN_CFG_LOG_MAX_SIZE];
        bn::istring_base _bn_istring(_bn_string);
        bn::ostringstream _bn_string_stream(_bn_istring);
        for (const auto& elem : row)
            _bn_string_stream.append_args((u8)elem, " ");
        bn::log(_bn_istring);
    }
#endif
}

static void _addAdjCountToNeighbors(const DungeonGenerator::Room& prev, DungeonGenerator::Room& cur, s32 x, s32 y)
{
    for (s32 cy = bn::max(y - 1, 0); cy <= bn::min(y + 1, prev.size() - 1); ++cy)
        for (s32 cx = bn::max(x - 1, 0); cx <= bn::min(x + 1, prev[cy].size() - 1); ++cx)
            if (!(cy == y && cx == x))
                cur[cy][cx] = (DungeonGenerator::FloorType)((u8)cur[cy][cx] + 1);
}

/**
 * @brief DFS used in Cellular automata room generation.
 * @param removeMode if enabled, fill in the passed small blob with walls.
 * @return size of the blob.
 */
static s32 _dfsCellular(s32 x, s32 y, bool removeMode, DungeonGenerator::Room& room, DungeonGenerator::Room& visited)
{
    using Gen = DungeonGenerator;
    constexpr bn::point UDLR[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    if (removeMode)
        room[y][x] = Gen::FloorType::WALL;

    bn::vector<bn::point, Gen::ROOM_MAX_LEN * Gen::ROOM_MAX_LEN> stack;
    visited[y][x] = (Gen::FloorType) true;
    stack.push_back({x, y});
    s32 blobSize = 1;

    while (!stack.empty())
    {
        auto cur = stack.back();
        stack.pop_back();

        for (auto direction : UDLR)
        {
            bn::point candidate{cur.x() + direction.x(), cur.y() + direction.y()};
            // check OOB
            if (candidate.x() < 0 || candidate.y() < 0 || candidate.x() >= room[0].size() ||
                candidate.y() >= room.size())
                continue;
            // check already visited
            if ((bool)visited[candidate.y()][candidate.x()])
                continue;
            // check not floor
            if (room[candidate.y()][candidate.x()] != Gen::FloorType::FLOOR)
                continue;

            ++blobSize;
            visited[candidate.y()][candidate.x()] = (Gen::FloorType) true;
            stack.push_back(candidate);

            // Remove the floor cell by filling it with wall, when `removeMode` is enabled.
            if (removeMode)
                room[candidate.y()][candidate.x()] = Gen::FloorType::WALL;
        }
    }

    return blobSize;
}

/**
 * @brief Find the biggest connected blob, and remove the other small blobs.
 * @return `false` if resulting room is smaller than `CELLULAR_ROOM_MIN_CELLS_COUNT`.
 */
static bool _removeSmallBlobs(DungeonGenerator::Room& room, DungeonGenerator::Room& visited)
{
    using Gen = DungeonGenerator;

    // clear visited
    for (auto& row : visited)
        for (auto& elem : row)
            elem = (Gen::FloorType) false;

    bn::vector<bn::point, Gen::ROOM_MAX_LEN / 2 * Gen::ROOM_MAX_LEN> blobStartPositions;
    s32 biggestBlobSize = -1;
    bn::point biggestBlobStartPos = {-1, -1};

    // find the biggest blob
    for (s32 y = 0; y < room.size(); ++y)
        for (s32 x = 0; x < room[y].size(); ++x)
            if (room[y][x] == Gen::FloorType::FLOOR && (bool)visited[y][x] == false)
            {
                blobStartPositions.push_back({x, y});

                s32 blobSize = _dfsCellular(x, y, false, room, visited);
                if (blobSize > biggestBlobSize)
                {
                    biggestBlobSize = blobSize;
                    biggestBlobStartPos = {x, y};
                }
            }

    // clear visited again
    for (auto& row : visited)
        for (auto& elem : row)
            elem = (Gen::FloorType) false;

    // remove blobs other than the biggest blob
    for (const auto& blobPos : blobStartPositions)
    {
        if (blobPos == biggestBlobStartPos)
            continue;
        _dfsCellular(blobPos.x(), blobPos.y(), true, room, visited);
    }

    return biggestBlobSize >= Gen::CELLULAR_ROOM_MIN_CELLS_COUNT;
}

auto DungeonGenerator::_createCellularRoom(iso_bn::random& rng) -> DungeonGenerator::Room
{
    for (s32 trial = 0; trial < CELLULAR_ROOM_FAIL_FALLBACK_COUNT; ++trial)
    {
        // Initialize room with random walls (CELLULAR_WALL_RATIO %)
        Room room1(CELLULAR_ROOM_MAX_LEN);
        for (s32 y = 0; y < CELLULAR_ROOM_MAX_LEN; ++y)
            for (s32 x = 0; x < CELLULAR_ROOM_MAX_LEN; ++x)
                room1[y].push_back(rng.get_fixed(1) <= CELLULAR_WALL_RATIO ? FloorType::WALL : FloorType::FLOOR);

        Room room2(CELLULAR_ROOM_MAX_LEN, bn::vector<FloorType, ROOM_MAX_LEN>(CELLULAR_ROOM_MAX_LEN, (FloorType)0));

        Room *prev = &room1, *cur = &room2;
        // Running 5 rounds of smoothing.
        // smoothing: adjacent floor < 4 becomes a wall, adj floor >= 6 becomes a floor.
        for (s32 i = 0; i < SMOOTHING_COUNT; ++i)
        {
            // clear `cur` with zero
            for (auto& row : *cur)
                for (auto& elem : row)
                    elem = (FloorType)0;

            // count adj floor count with `cur`
            for (s32 y = 0; y < prev->size(); ++y)
                for (s32 x = 0; x < (*prev)[y].size(); ++x)
                    if ((*prev)[y][x] == FloorType::FLOOR)
                        _addAdjCountToNeighbors(*prev, *cur, x, y);

            // actually make tiles in `cur` a wall or floor
            for (s32 y = 0; y < cur->size(); ++y)
                for (s32 x = 0; x < (*cur)[y].size(); ++x)
                {
                    const u8 adjFloorCnt = (u8)(*cur)[y][x];
                    if (adjFloorCnt < 4)
                        (*cur)[y][x] = FloorType::WALL;
                    else if (adjFloorCnt >= 6)
                        (*cur)[y][x] = FloorType::FLOOR;
                    else
                        (*cur)[y][x] = (*prev)[y][x];
                }

            // swap buffer
            bn::swap(prev, cur);
        }

        // Find the biggest connected blob, and remove the other small blobs.
        if (_removeSmallBlobs(*prev, *cur))
            return *prev;
    }

    // If failed too many times, fallback to square room.
    return _createSquareRoom(rng);
}

auto DungeonGenerator::_createSquareRoom(iso_bn::random& rng) -> DungeonGenerator::Room
{
    BN_ERROR("Not implemented yet");
}

} // namespace mp::game
