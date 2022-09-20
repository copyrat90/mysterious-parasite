/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/state/PickPosState.hpp"

#include "bn_array.h"
#include "bn_deque.h"
#include "bn_keypad.h"

#include "game/Dungeon.hpp"
#include "game/MetaTileset.hpp"
#include "game/item/ItemInfo.hpp"
#include "utils.hpp"

namespace mp::game::state
{

enum TossTileIndex : u8
{
    EMPTY = 0,
    CURSOR,
    HINT
};

namespace
{

constexpr s32 MAX_TOSS_DIST = item::ItemInfo::MAX_TOSS_DISTANCE;
constexpr s32 DIMENSION = 2 * MAX_TOSS_DIST + 1;
constexpr s32 DEQUE_SIZE = utils::upperTwoPowOf(DIMENSION * DIMENSION);
constexpr BoardPos UDLR[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

using VisitArray = bn::array<bn::array<s8, DIMENSION>, DIMENSION>;

} // namespace

PickPosState::PickPosState(Dungeon& dungeon)
    : GameState(dungeon), _tileset(MetaTileset::fromKind(MetaTilesetKind::TOSS_HINT)), _cells{},
      _mapItem(_cells[0], bn::size(COLUMNS, ROWS)),
      _bgItem(_tileset.getBgItem().tiles_item(), _tileset.getBgItem().palette_item(), _mapItem),
      _bg(_bgItem.create_bg(0, 0)), _bgMap(_bg.map())
{
    _initGraphics();
}

auto PickPosState::handleInput() -> bn::optional<GameStateArgs>
{
    // TODO
    if (bn::keypad::b_pressed())
        return GameStateArgs(getStateKind(), GameStateKind::IDLE);

    return bn::nullopt;
}

void PickPosState::onEnter(const GameStateArgs& args)
{
    _dungeon.getHud().redrawItemHintText(true);

    _redrawCells(args);

    // TODO: Show cursor & toss range BG
}

void PickPosState::onExit()
{
    _dungeon.getHud().redrawItemHintText(false);

    // TODO: Hide cursor & toss range BG
}

void PickPosState::_initGraphics()
{
    _bg.set_visible(false);
    _bg.set_priority(consts::DUNGEON_BG_PRIORITY);
    _bg.set_z_order(consts::DUNGEON_BG_Z_ORDER_TOSS_HINT);

    // START TEST
    const s32 x = 15, y = 15;
    for (s32 yy = 0; yy < 2; ++yy)
        for (s32 xx = 0; xx < 2; ++xx)
            _cells[_mapItem.cell_index(x + xx, y + yy)] = _tileset.getCell(TossTileIndex::CURSOR, xx, yy);
    _bgMap.reload_cells_ref();
    _bg.set_visible(true);
    // END TEST

    // TODO: Initialize cursor & toss range BG
}

u8& _visitedElem(VisitArray& visited, const BoardPos& relativePos)
{
    // TODO
}

void PickPosState::_redrawCells(const GameStateArgs& args)
{
    const auto& playerPos = _dungeon.getPlayer().getBoardPos();

    const auto& item = _dungeon.getItemUse().getInventoryItem();
    BN_ASSERT(item.has_value(), "there is no inventory item");
    const auto& itemInfo = item->getItemInfo();
    const s32 tossDistance = itemInfo.tossDistance;

    // start BFS to set the toss range BG
    VisitArray visited;
    for (s32 i = 0; i < visited.size(); ++i)
        visited[i].fill(-1);

    BoardPos playerZeroPos = {0, 0};
    bn::deque<BoardPos, DEQUE_SIZE> q;

    _visitedElem(visited, playerZeroPos) = 0;
    q.push_back(playerZeroPos);

    while (!q.empty())
    {
        const auto cur = q.front();
        q.pop_front();
        const u8 curDist = _visitedElem(visited, cur);
        for (const auto& dir : UDLR)
        {
            const auto candiPos = cur + dir;
            // TODO: Check if this tile is OOB
            // TODO: Check if this tile is wall, as you can't throw an item to a wall.

            // Check if this tile is visited
            u8& candiDist = _visitedElem(visited, candiPos);
            if (candiDist >= 0)
                continue;

            candiDist = curDist + 1;
            q.push_back(candiPos);
        }
    }
}

void PickPosState::_redrawCell(const BoardPos& relativePos)
{
    // TODO
}

} // namespace mp::game::state
