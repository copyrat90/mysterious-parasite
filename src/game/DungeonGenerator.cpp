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
#include "bn_deque.h"
#include "bn_limits.h"
#include "bn_log.h"
#include "bn_memory.h"
#include "bn_profiler.h"

#include "iso_bn_random.h"

namespace mp::game
{

namespace
{

using Gen = DungeonGenerator;

constexpr s32 INF = bn::numeric_limits<s32>::max();

constexpr s32 SMOOTHING_COUNT = 5;
constexpr s32 CELLULAR_ROOM_FAIL_FALLBACK_COUNT = 3;

constexpr s32 PLACE_ROOM_RETRY_COUNT = 5;
constexpr s32 REGEN_ROOM_RETRY_COUNT = 5;
constexpr s32 HALLWAY_MAX_LEN = 7;

constexpr BoardPos UDLR[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
constexpr BoardPos DIAGONAL[4] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};

} // namespace

static void _debugLogRoom(const Gen::Room& room)
{
#if BN_CFG_LOG_ENABLED
    BN_LOG("=== room status ===");
    for (const auto& row : room.floors)
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

static void _debugLogBoard(const Gen::Board& board)
{
#if BN_CFG_LOG_ENABLED
    BN_LOG("=== board status ===");
    for (const auto& row : board)
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

static bool _isBoardOOB(const BoardPos& cellPos)
{
    return cellPos.y < 0 || cellPos.x < 0 || cellPos.y >= Gen::ROWS || cellPos.x >= Gen::COLUMNS;
}

bool Gen::Room::isBoardOOB() const
{
    // top-left or bottom-right check OOB check
    return _isBoardOOB(boardOffset) ||
           _isBoardOOB(boardOffset + BoardPos{(s8)(floors[0].size() - 1), (s8)(floors.size() - 1)});
}

bool Gen::Room::isBoardFloorOverlap(const Board& board) const
{
    for (s32 y = 0; y < floors.size(); ++y)
    {
        for (s32 x = 0; x < floors[y].size(); ++x)
        {
            const s32 yGlobal = y + boardOffset.y;
            const s32 xGlobal = x + boardOffset.x;
            if (board[yGlobal][xGlobal] == FloorType::FLOOR && floors[y][x] == FloorType::FLOOR)
                return true;
        }
    }
    return false;
}

void Gen::generate(Board& board, iso_bn::random& rng)
{
    BN_PROFILER_START("dungeon_gen");

    _clearWithWalls(board);

    s32 regenRoomRetryRemain = REGEN_ROOM_RETRY_COUNT;
    while (true)
    {
        bn::fixed randNum = rng.get_fixed(1);
        Room room = (randNum <= CELLULAR_ROOM_RATIO)                       ? _createCellularRoom(rng)
                    : (randNum <= SQUARE_ROOM_RATIO + CELLULAR_ROOM_RATIO) ? _createSquareRoom(rng)
                                                                           : _createCrossRoom(rng);
        if (!_placeRoom(room, board, rng))
        {
            if (regenRoomRetryRemain-- <= 0)
                break;
        }
    }

    // BN_LOG("_wallsNearFloor.size(): ", _wallsNearFloor.size());

    // _debugLogBoard(board);

    BN_PROFILER_STOP();
}

void Gen::_clearWithWalls(Board& board) const
{
    if constexpr ((u8)FloorType::WALL == 0)
        bn::memory::clear(ROWS * COLUMNS, board[0][0]);
    else
        bn::memory::set_bytes((u8)FloorType::WALL, ROWS * COLUMNS, &board[0][0]);
}

static s32 _boardCellIndex(const BoardPos& p)
{
    BN_ASSERT(0 <= p.y && p.y < Gen::ROWS);
    BN_ASSERT(0 <= p.x && p.x < Gen::COLUMNS);

    return p.y * Gen::COLUMNS + p.x;
}

void Gen::_addAdjacentWallsFromFloor(const BoardPos& floorPos, const Board& board)
{
    BN_ASSERT(board[floorPos.y][floorPos.x] == FloorType::FLOOR);

    for (const auto& direction : UDLR)
    {
        const BoardPos candidate = floorPos + direction;
        if (_isBoardOOB(candidate))
            continue;
        if (board[candidate.y][candidate.x] != FloorType::WALL)
            continue;
        // if `candidate` is already in the `_wallsNearFloor`
        if (_wallsNearFloorAdded[_boardCellIndex(candidate)])
            continue;

        _wallsNearFloorAdded[_boardCellIndex(candidate)] = true;
        _wallsNearFloor.push_back(candidate);
    }
}

auto Gen::_getHallwayDirection(const BoardPos& wallNearFloor, const Board& board) const -> BoardPos
{
    BN_ASSERT(_wallsNearFloorAdded[_boardCellIndex(wallNearFloor)]);

    s32 nearFloorCount = 0;
    BoardPos result;
    for (const auto& direction : UDLR)
    {
        const auto adjacent = wallNearFloor + direction;
        if (!_isBoardOOB(adjacent) && board[adjacent.y][adjacent.x] == FloorType::FLOOR)
        {
            ++nearFloorCount;
            result = -direction;
        }
    }
    return (nearFloorCount == 1) ? result : BoardPos{0, 0};
}

static s32 _udlrDirToIdx(const BoardPos& direction)
{
    if (direction == BoardPos{0, -1})
        return 0;
    if (direction == BoardPos{0, 1})
        return 1;
    if (direction == BoardPos{-1, 0})
        return 2;
    if (direction == BoardPos{1, 0})
        return 3;
    BN_ERROR("Invalid direction {x=", direction.x, ", y=", direction.y, "}");
}

bool Gen::_placeRoom(Room& room, Board& board, iso_bn::random& rng)
{
    bool success = false;
    // If this is the first room to place, put it on the center of the board.
    if (_wallsNearFloor.empty())
    {
        room.boardOffset = {(s8)(COLUMNS / 2 - room.floors[0].size() / 2), (s8)(ROWS / 2 - room.floors.size() / 2)};
        success = true;
    }
    // Else, add the room by connecting it to an existing wall near a floor.
    else
    {
        for (s32 trial = 0; trial < PLACE_ROOM_RETRY_COUNT; ++trial)
        {
            // BN_LOG("add room trial #", trial);

            const auto& wallNearFloor = _wallsNearFloor[rng.get_int(_wallsNearFloor.size())];
            BoardPos direction = _getHallwayDirection(wallNearFloor, board);
            if (direction == BoardPos{0, 0})
                continue;

            // BN_LOG("wallNearFloor {x=", wallNearFloor.x, ", y=", wallNearFloor.y, "}");
            // BN_LOG("direction: ", _udlrDirToIdx(direction));

            // _debugLogBoard(board);
            // _debugLogRoom(room);

            // First, move the room so that the door on the opposite `direction` is on the `wallNearFloor`.
            const s32 udlrIdx = _udlrDirToIdx(-direction);
            room.boardOffset = wallNearFloor;
            room.boardOffset -= room.doors[udlrIdx];

            for (s32 hallwayLen = 1; hallwayLen <= HALLWAY_MAX_LEN; ++hallwayLen)
            {
                // Move the room by one step
                room.boardOffset += direction;

                // BN_LOG("boardOffset {x=", room.boardOffset.x, ", y=", room.boardOffset.y, "}");

                // If the room is OOB of board, cannot place it further.
                if (room.isBoardOOB())
                    break;
                // If the room overlaps with board floors, cannot place it this time.
                if (room.isBoardFloorOverlap(board))
                    continue;

                success = true;
                // add hallway
                for (s32 h = hallwayLen; h > 0; --h)
                {
                    const auto hallwayPos = room.boardOffset + room.doors[udlrIdx] - direction * h;
                    board[hallwayPos.y][hallwayPos.x] = FloorType::FLOOR;
                }
                // add adjacent walls from this hallway to `_wallsNearFloor`
                for (s32 h = hallwayLen; h > 0; --h)
                {
                    const auto hallwayPos = room.boardOffset + room.doors[udlrIdx] - direction * h;
                    _addAdjacentWallsFromFloor(hallwayPos, board);
                }
                break;
            }

            if (success)
                break;
        }
    }

    // copy the room's floors to the global board
    if (success)
    {
        for (s8 y = 0; y < room.floors.size(); ++y)
            for (s8 x = 0; x < room.floors[y].size(); ++x)
            {
                const s8 yGlobal = y + room.boardOffset.y;
                const s8 xGlobal = x + room.boardOffset.x;
                // DO NOT overwrite floor with walls!
                if (board[yGlobal][xGlobal] != DungeonFloor::Type::FLOOR)
                    board[yGlobal][xGlobal] = room.floors[y][x];
            }

        // add adjacent walls from this room to `_wallsNearFloor`
        for (s8 y = 0; y < room.floors.size(); ++y)
            for (s8 x = 0; x < room.floors[0].size(); ++x)
            {
                const s8 yGlobal = y + room.boardOffset.y;
                const s8 xGlobal = x + room.boardOffset.x;
                if (board[yGlobal][xGlobal] == FloorType::FLOOR)
                    _addAdjacentWallsFromFloor(BoardPos{xGlobal, yGlobal}, board);
            }
    }

    return success;
}

static void _addAdjCountToNeighbors(const Gen::Room& prev, Gen::Room& cur, s32 x, s32 y)
{
    for (s32 cy = bn::max(y - 1, 0); cy <= bn::min(y + 1, prev.floors.size() - 1); ++cy)
        for (s32 cx = bn::max(x - 1, 0); cx <= bn::min(x + 1, prev.floors[cy].size() - 1); ++cx)
            if (!(cy == y && cx == x))
                cur.floors[cy][cx] = (Gen::FloorType)((u8)cur.floors[cy][cx] + 1);
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
static s32 _bfsCellular(s8 x, s8 y, bool removeMode, Gen::Room& room, Gen::Room& visited)
{
    using Gen = Gen;

    if (removeMode)
        room.floors[y][x] = Gen::FloorType::WALL;

    bn::deque<BoardPos, _upperTwoPowOf(2 * Gen::ROOM_MAX_LEN + 4)> queue;
    visited.floors[y][x] = (Gen::FloorType) true;
    queue.push_back({x, y});
    s32 blobSize = 1;

    while (!queue.empty())
    {
        auto cur = queue.front();
        queue.pop_front();

        for (auto direction : UDLR)
        {
            const BoardPos candidate = cur + direction;
            // check OOB
            if (candidate.x < 0 || candidate.y < 0 || candidate.x >= room.floors[0].size() ||
                candidate.y >= room.floors.size())
                continue;
            // check already visited
            if ((bool)visited.floors[candidate.y][candidate.x])
                continue;
            // check wall
            if (room.floors[candidate.y][candidate.x] == Gen::FloorType::WALL)
                continue;

            ++blobSize;
            visited.floors[candidate.y][candidate.x] = (Gen::FloorType) true;
            queue.push_back(candidate);

            // Remove the floor cell by filling it with wall, when `removeMode` is enabled.
            if (removeMode)
                room.floors[candidate.y][candidate.x] = Gen::FloorType::WALL;
        }
    }

    return blobSize;
}

/**
 * @brief Find the biggest connected blob, and remove the other small blobs.
 * @return `false` if resulting room is smaller than `CELLULAR_ROOM_MIN_CELLS_COUNT`.
 */
static bool _removeSmallBlobs(Gen::Room& room, Gen::Room& visited)
{
    // clear visited
    for (auto& row : visited.floors)
        for (auto& elem : row)
            elem = (Gen::FloorType) false;

    bn::vector<BoardPos, Gen::ROOM_MAX_LEN / 2 * Gen::ROOM_MAX_LEN> blobStartPositions;
    s32 biggestBlobSize = -1;
    BoardPos biggestBlobStartPos = {-1, -1};

    // find the biggest blob
    for (s8 y = 0; y < room.floors.size(); ++y)
        for (s8 x = 0; x < room.floors[y].size(); ++x)
            if (room.floors[y][x] == Gen::FloorType::FLOOR && (bool)visited.floors[y][x] == false)
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
    for (auto& row : visited.floors)
        for (auto& elem : row)
            elem = (Gen::FloorType) false;

    // remove blobs other than the biggest blob
    for (const auto& blobPos : blobStartPositions)
        if (blobPos != biggestBlobStartPos)
            _bfsCellular(blobPos.x, blobPos.y, true, room, visited);

    return biggestBlobSize >= Gen::CELLULAR_ROOM_MIN_CELLS_COUNT;
}

static void _makeDoorsWithBoundary(Gen::Room& room, s32 xMin, s32 xMax, s32 yMin, s32 yMax, iso_bn::random& rng)
{
    bn::vector<BoardPos, Gen::ROOM_MAX_LEN> boundaryFloors;

    // top boundary
    for (s32 x = xMin; x <= xMax; ++x)
    {
        const s32 y = yMin;
        if (room.floors[y][x] == Gen::FloorType::FLOOR)
            boundaryFloors.push_back({(s8)x, (s8)y});
    }
    room.doors[0] = boundaryFloors[rng.get_int(boundaryFloors.size())];

    // bottom boundary
    boundaryFloors.clear();
    for (s32 x = xMin; x <= xMax; ++x)
    {
        const s32 y = yMax;
        if (room.floors[y][x] == Gen::FloorType::FLOOR)
            boundaryFloors.push_back({(s8)x, (s8)y});
    }
    room.doors[1] = boundaryFloors[rng.get_int(boundaryFloors.size())];

    // left boundary
    boundaryFloors.clear();
    for (s32 y = yMin; y <= yMax; ++y)
    {
        const s32 x = xMin;
        if (room.floors[y][x] == Gen::FloorType::FLOOR)
            boundaryFloors.push_back({(s8)x, (s8)y});
    }
    room.doors[2] = boundaryFloors[rng.get_int(boundaryFloors.size())];

    // right boundary
    boundaryFloors.clear();
    for (s32 y = yMin; y <= yMax; ++y)
    {
        const s32 x = xMax;
        if (room.floors[y][x] == Gen::FloorType::FLOOR)
            boundaryFloors.push_back({(s8)x, (s8)y});
    }
    room.doors[3] = boundaryFloors[rng.get_int(boundaryFloors.size())];
}

static void _makeDoorsForGenericRoom(Gen::Room& room, iso_bn::random& rng)
{
    s32 xMin = INF, xMax = -INF, yMin = INF, yMax = -INF;

    // find the actual boundary of the room
    for (s32 y = 0; y < room.floors.size(); ++y)
        for (s32 x = 0; x < room.floors[y].size(); ++x)
            if (room.floors[y][x] == Gen::FloorType::FLOOR)
            {
                xMin = bn::min(xMin, x), xMax = bn::max(xMax, x);
                yMin = bn::min(yMin, y), yMax = bn::max(yMax, y);
            }

    BN_ASSERT(xMin != INF && xMax != -INF && yMin != INF && yMax != -INF, "Room is empty");

    _makeDoorsWithBoundary(room, xMin, xMax, yMin, yMax, rng);
}

auto Gen::_createCellularRoom(iso_bn::random& rng) const -> Room
{
    Room result, temp;
    result.floors.resize(CELLULAR_ROOM_MAX_LEN, bn::vector<FloorType, ROOM_MAX_LEN>(CELLULAR_ROOM_MAX_LEN));
    temp.floors.resize(CELLULAR_ROOM_MAX_LEN, bn::vector<FloorType, ROOM_MAX_LEN>(CELLULAR_ROOM_MAX_LEN));

    bool success = false;
    for (s32 trial = 0; trial < CELLULAR_ROOM_FAIL_FALLBACK_COUNT; ++trial)
    {
        Room* prev = (SMOOTHING_COUNT % 2 == 0) ? &result : &temp;
        Room* cur = (SMOOTHING_COUNT % 2 == 0) ? &temp : &result;

        // Initialize room with random walls (CELLULAR_WALL_RATIO %)
        for (s32 y = 0; y < CELLULAR_ROOM_MAX_LEN; ++y)
            for (s32 x = 0; x < CELLULAR_ROOM_MAX_LEN; ++x)
                prev->floors[y][x] = rng.get_fixed(1) <= CELLULAR_INIT_WALL_RATIO ? FloorType::WALL : FloorType::FLOOR;

        // Running 5 rounds of smoothing.
        // smoothing: adjacent floor < 4 becomes a wall, adj floor >= 6 becomes a floor.
        for (s32 i = 0; i < SMOOTHING_COUNT; ++i)
        {
            // clear `cur` with zero
            for (auto& row : cur->floors)
                for (auto& elem : row)
                    elem = (FloorType)0;

            // count adj floor count with `cur`
            for (s32 y = 0; y < prev->floors.size(); ++y)
                for (s32 x = 0; x < prev->floors[y].size(); ++x)
                    if (prev->floors[y][x] == FloorType::FLOOR)
                        _addAdjCountToNeighbors(*prev, *cur, x, y);

            // actually make tiles in `cur` a wall or floor
            for (s32 y = 0; y < cur->floors.size(); ++y)
                for (s32 x = 0; x < cur->floors[y].size(); ++x)
                {
                    const u8 adjFloorCnt = (u8)cur->floors[y][x];
                    if (adjFloorCnt < 4)
                        cur->floors[y][x] = FloorType::WALL;
                    else if (adjFloorCnt >= 6)
                        cur->floors[y][x] = FloorType::FLOOR;
                    else
                        cur->floors[y][x] = prev->floors[y][x];
                }

            // swap buffer
            bn::swap(prev, cur);
        }

        // Find the biggest connected blob, and remove the other small blobs.
        if (_removeSmallBlobs(result, temp))
        {
            _makeDoorsForGenericRoom(result, rng);
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

auto Gen::_createSquareRoom(iso_bn::random& rng) const -> Room
{
    const s32 width = rng.get_int(SQUARE_ROOM_MIN_LEN, SQUARE_ROOM_MAX_LEN + 1);
    const s32 height = rng.get_int(SQUARE_ROOM_MIN_LEN, SQUARE_ROOM_MAX_LEN + 1);

    Room result;
    result.floors.resize(height, bn::vector<FloorType, ROOM_MAX_LEN>(width, FloorType::FLOOR));
    _makeDoorsWithBoundary(result, 0, width - 1, 0, height - 1, rng);
    return result;
}

static void _crossRoomPutSquare(s32 width, s32 height, Gen::Room& room)
{
    for (s32 y = room.floors.size() / 2 - height / 2; y < room.floors.size() / 2 + height / 2; ++y)
        for (s32 x = room.floors[y].size() / 2 - width / 2; x < room.floors[y].size() / 2 + width / 2; ++x)
            room.floors[y][x] = Gen::FloorType::FLOOR;
}

auto Gen::_createCrossRoom(iso_bn::random& rng) const -> Room
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

    Room room;
    room.floors.resize(bn::max(height1, height2),
                       bn::vector<FloorType, ROOM_MAX_LEN>(bn::max(width1, width2), FloorType::WALL));
    _crossRoomPutSquare(width1, height1, room);
    _crossRoomPutSquare(width2, height2, room);

    _makeDoorsWithBoundary(room, 0, bn::max(width1, width2) - 1, 0, bn::max(height1, height2) - 1, rng);
    return room;
}

bool _shortestPathBfsNextCellCheck(bool isDiagonal, const BoardPos& candidate, const BoardPos& cur,
                                   const bn::bitset<Gen::COLUMNS * Gen::ROWS>& visited, const Gen::Board& board)
{
    // check OOB
    if (_isBoardOOB(candidate))
        return false;
    // check already visited
    if (visited[_boardCellIndex(candidate)])
        return false;
    // check wall
    if (board[candidate.y][candidate.x] == Gen::FloorType::WALL)
        return false;
    // Diagonal movement only: check diagonal adjacent wall
    if (isDiagonal)
    {
        if (board[candidate.y][cur.x] == Gen::FloorType::WALL || board[cur.y][candidate.x] == Gen::FloorType::WALL)
            return false;
    }

    return true;
}

s32 Gen::_shortestPathLen(const BoardPos& p1, const BoardPos& p2, const Board& board) const
{
    BN_ASSERT(board[p1.y][p1.x] == FloorType::FLOOR, "p1(", p1.x, ", ", p1.y, ") is not a floor");
    BN_ASSERT(board[p2.y][p2.x] == FloorType::FLOOR, "p2(", p2.x, ", ", p2.y, ") is not a floor");

    if (p1 == p2)
        return 0;

    struct QElem
    {
        s32 len = 0;
        BoardPos pos;
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
            const BoardPos candidate = cur.pos + direction;
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
            const BoardPos candidate = cur.pos + direction;
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
