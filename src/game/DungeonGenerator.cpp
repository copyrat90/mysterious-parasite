/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/DungeonGenerator.hpp"

#include "bn_array.h"
#include "bn_assert.h"
#include "bn_bitset.h"
#include "bn_deque.h"
#include "bn_log.h"
#include "bn_memory.h"
#include "bn_point.h"
#include "bn_profiler.h"

#include "iso_bn_random.h"

namespace mp::game
{

namespace
{

constexpr s32 SMOOTHING_COUNT = 5;
constexpr s32 CELLULAR_ROOM_FAIL_FALLBACK_COUNT = 3;

constexpr bn::point UDLR[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
constexpr bn::point DIAGONAL[4] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};

} // namespace

void DungeonGenerator::generate(Board& board, iso_bn::random& rng) const
{
    BN_PROFILER_START("dungeon_gen");

    _clearWithWalls(board);

#ifdef MP_DEBUG
    Room room = _createCellularRoom(rng);
    for (s32 y = 0; y < room.size(); ++y)
        for (s32 x = 0; x < room[0].size(); ++x)
            board[ROWS / 2 - room.size() / 2 + y][COLUMNS / 2 - room[0].size() + x] = room[y][x];
#endif

    BN_PROFILER_STOP();
}

void DungeonGenerator::_clearWithWalls(Board& board) const
{
    if constexpr ((u8)FloorType::WALL == 0)
        bn::memory::clear(ROWS * COLUMNS, board[0][0]);
    else
        bn::memory::set_bytes((u8)FloorType::WALL, ROWS * COLUMNS, &board[0][0]);
}

bool DungeonGenerator::_placeRoom(Room& room) const
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

static constexpr s32 _upperTwoPowOf(s32 num)
{
    BN_ASSERT(0 <= num && num <= (1 << 30));

    s32 result = 1;
    while (result < num)
        result <<= 1;
    return result;
}

/**
 * @brief BFS used in Cellular automata room generation.
 * @param removeMode if enabled, fill in the passed small blob with walls.
 * @return size of the blob.
 */
static s32 _bfsCellular(s32 x, s32 y, bool removeMode, DungeonGenerator::Room& room, DungeonGenerator::Room& visited)
{
    using Gen = DungeonGenerator;

    if (removeMode)
        room[y][x] = Gen::FloorType::WALL;

    bn::deque<bn::point, _upperTwoPowOf(2 * Gen::ROOM_MAX_LEN + 4)> queue;
    visited[y][x] = (Gen::FloorType) true;
    queue.push_back({x, y});
    s32 blobSize = 1;

    while (!queue.empty())
    {
        auto cur = queue.front();
        queue.pop_front();

        for (auto direction : UDLR)
        {
            const bn::point candidate = cur + direction;
            // check OOB
            if (candidate.x() < 0 || candidate.y() < 0 || candidate.x() >= room[0].size() ||
                candidate.y() >= room.size())
                continue;
            // check already visited
            if ((bool)visited[candidate.y()][candidate.x()])
                continue;
            // check wall
            if (room[candidate.y()][candidate.x()] == Gen::FloorType::WALL)
                continue;

            ++blobSize;
            visited[candidate.y()][candidate.x()] = (Gen::FloorType) true;
            queue.push_back(candidate);

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

                s32 blobSize = _bfsCellular(x, y, false, room, visited);
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
        if (blobPos != biggestBlobStartPos)
            _bfsCellular(blobPos.x(), blobPos.y(), true, room, visited);

    return biggestBlobSize >= Gen::CELLULAR_ROOM_MIN_CELLS_COUNT;
}

auto DungeonGenerator::_createCellularRoom(iso_bn::random& rng) const -> Room
{
    Room result(CELLULAR_ROOM_MAX_LEN, bn::vector<FloorType, ROOM_MAX_LEN>(CELLULAR_ROOM_MAX_LEN));
    Room temp(CELLULAR_ROOM_MAX_LEN, bn::vector<FloorType, ROOM_MAX_LEN>(CELLULAR_ROOM_MAX_LEN));

    bool success = false;
    for (s32 trial = 0; trial < CELLULAR_ROOM_FAIL_FALLBACK_COUNT; ++trial)
    {
        Room* prev = (SMOOTHING_COUNT % 2 == 0) ? &result : &temp;
        Room* cur = (SMOOTHING_COUNT % 2 == 0) ? &temp : &result;

        // Initialize room with random walls (CELLULAR_WALL_RATIO %)
        for (s32 y = 0; y < CELLULAR_ROOM_MAX_LEN; ++y)
            for (s32 x = 0; x < CELLULAR_ROOM_MAX_LEN; ++x)
                (*prev)[y][x] = rng.get_fixed(1) <= CELLULAR_WALL_RATIO ? FloorType::WALL : FloorType::FLOOR;

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
        {
            success = true;
            break;
        }
    }

    if (!success)
    {
        // If failed too many times, fallback to square room.
        BN_LOG("cellular room gen failed ", CELLULAR_ROOM_FAIL_FALLBACK_COUNT, " times, fallback to square room gen");
        result = _createSquareRoom(rng);
    }
    return result;
}

auto DungeonGenerator::_createSquareRoom(iso_bn::random& rng) const -> Room
{
    const s32 width = rng.get_int(SQUARE_ROOM_MIN_LEN, SQUARE_ROOM_MAX_LEN + 1);
    const s32 height = rng.get_int(SQUARE_ROOM_MIN_LEN, SQUARE_ROOM_MAX_LEN + 1);

    return Room(height, bn::vector<FloorType, ROOM_MAX_LEN>(width, FloorType::FLOOR));
}

static void _crossRoomPutSquare(s32 width, s32 height, DungeonGenerator::Room& room)
{
    using Gen = DungeonGenerator;

    for (s32 y = room.size() / 2 - height / 2; y < room.size() / 2 + height / 2; ++y)
        for (s32 x = room[y].size() / 2 - width / 2; x < room[y].size() / 2 + width / 2; ++x)
            room[y][x] = Gen::FloorType::FLOOR;
}

auto DungeonGenerator::_createCrossRoom(iso_bn::random& rng) const -> Room
{
    static_assert(CROSS_ROOM_MIN_LEN % 2 == 0);
    static_assert(CROSS_ROOM_MAX_LEN % 2 == 0);

    bn::array<s32, 4> dimensions;
    for (auto& d : dimensions)
    {
        d = rng.get_int(CROSS_ROOM_MIN_LEN / 2, CROSS_ROOM_MAX_LEN / 2 + 1);
        d *= 2;
    }

    bn::sort(dimensions.begin(), dimensions.end());

    const s32 width1 = dimensions[0];
    const s32 height2 = dimensions[1];
    const s32 width2 = dimensions[2];
    const s32 height1 = dimensions[3];

    Room room(bn::max(height1, height2), bn::vector<FloorType, ROOM_MAX_LEN>(bn::max(width1, width2), FloorType::WALL));
    _crossRoomPutSquare(width1, height1, room);
    _crossRoomPutSquare(width2, height2, room);

    return room;
}

static s32 _boardCellIndex(const bn::point& p)
{
    BN_ASSERT(0 <= p.y() && p.y() < DungeonGenerator::ROWS);
    BN_ASSERT(0 <= p.x() && p.x() < DungeonGenerator::COLUMNS);

    return p.y() * DungeonGenerator::COLUMNS + p.x();
}

bool _shortestPathBfsNextCellCheck(bool isDiagonal, const bn::point& candidate, const bn::point& cur,
                                   const bn::bitset<DungeonGenerator::COLUMNS * DungeonGenerator::ROWS>& visited,
                                   const DungeonGenerator::Board& board)
{
    using Gen = DungeonGenerator;
    // check OOB
    if (candidate.x() < 0 || candidate.y() < 0 || candidate.x() >= Gen::COLUMNS || candidate.y() >= Gen::ROWS)
        return false;
    // check already visited
    if (visited[_boardCellIndex(candidate)])
        return false;
    // check wall
    if (board[candidate.y()][candidate.x()] == Gen::FloorType::WALL)
        return false;
    // Diagonal movement only: check diagonal adjacent wall
    if (isDiagonal)
    {
        if (board[candidate.y()][cur.x()] == Gen::FloorType::WALL ||
            board[cur.y()][candidate.x()] == Gen::FloorType::WALL)
            return false;
    }

    return true;
}

s32 DungeonGenerator::_shortestPathLen(const bn::point& p1, const bn::point& p2, const Board& board) const
{
    BN_ASSERT(board[p1.y()][p1.x()] == FloorType::FLOOR, "p1(", p1.x(), ", ", p1.y(), ") is not a floor");
    BN_ASSERT(board[p2.y()][p2.x()] == FloorType::FLOOR, "p2(", p2.x(), ", ", p2.y(), ") is not a floor");

    if (p1 == p2)
        return 0;

    struct QElem
    {
        s32 len = 0;
        bn::point pos;
    };

    bn::deque<QElem, _upperTwoPowOf(2 * bn::min(ROWS, COLUMNS) + 4)> queue;
    bn::bitset<COLUMNS * ROWS> visited;

    visited[_boardCellIndex(p1)] = true;
    queue.push_back({0, p1});

    while (!queue.empty())
    {
        auto cur = queue.front();
        queue.pop_front();

        for (const auto& direction : UDLR)
        {
            const bn::point candidate = cur.pos + direction;
            if (!_shortestPathBfsNextCellCheck(false, candidate, cur.pos, visited, board))
                continue;

            // If new pos is `p2`, found a shortest path.
            if (candidate == p2)
                return cur.len + 1;

            visited[_boardCellIndex(candidate)] = true;
            queue.push_back({cur.len + 1, candidate});
        }
        for (const auto& direction : DIAGONAL)
        {
            const bn::point candidate = cur.pos + direction;
            if (!_shortestPathBfsNextCellCheck(true, candidate, cur.pos, visited, board))
                continue;

            // If new pos is `p2`, found a shortest path.
            if (candidate == p2)
                return cur.len + 1;

            visited[_boardCellIndex(candidate)] = true;
            queue.push_back({cur.len + 1, candidate});
        }
    }

    // cannot find any path.
    return -1;
}

} // namespace mp::game
