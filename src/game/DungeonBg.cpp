/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/DungeonBg.hpp"

#include "bn_algorithm.h"
#include "bn_assert.h"
#include "bn_camera_ptr.h"
#include "bn_display.h"
#include "bn_fixed_rect.h"
#include "bn_math.h"
#include "bn_point.h"
#include "bn_utility.h"

#include "constants.hpp"
#include "game/DungeonFloor.hpp"
#include "game/MetaTileset.hpp"
#include "game/mob/Monster.hpp"

namespace mp::game
{

// TODO: Pass MetaTilesetKind parameter, and init `_metaTileset` with it.
DungeonBg::DungeonBg(const bn::camera_ptr& camera)
    : _metaTileset(&MetaTileset::fromKind(MetaTilesetKind::PLACEHOLDER)), _cells{},
      _mapItem(_cells[0], bn::size(DungeonBg::COLUMNS, DungeonBg::ROWS)),
      _bgItem(_metaTileset->getBgItem().tiles_item(), _metaTileset->getBgItem().palette_item(), _mapItem),
      _bg(_bgItem.create_bg(0, 0)), _bgMap(_bg.map())
{
    _initGraphics(camera);
}

void DungeonBg::update(const DungeonFloor& dungeonFloor, const mob::Monster& player)
{
    if (isBgScrollOngoing())
        _updateBgScroll(dungeonFloor, player);

    if (_cellsReloadRequired)
    {
        _cellsReloadRequired = false;
        _bgMap.reload_cells_ref();
    }
}

void DungeonBg::redrawAll()
{
}

bool DungeonBg::isBgScrollOngoing() const
{
    return _bgScrollCountdown > 0;
}

void DungeonBg::startBgScroll(Direction9 dir9)
{
    _bgScrollCountdown = consts::ACTOR_MOVE_FRAMES;
    _bgScrollDirection = dir9;
}

void DungeonBg::_updateBgScroll(const DungeonFloor& dungeonFloor, const mob::Monster& player)
{
    switch (--_bgScrollCountdown)
    {
    case consts::ACTOR_MOVE_FRAMES - 1:
        _redrawScrolledCells(0, dungeonFloor, player);
        break;
    case consts::ACTOR_MOVE_FRAMES - 5:
        _redrawScrolledCells(1, dungeonFloor, player);
        break;
    default:
        break;
    }
}

bool DungeonBg::isVisible() const
{
    return _bg.visible();
}

void DungeonBg::setVisible(bool isVisible)
{
    _bg.set_visible(isVisible);
}

void DungeonBg::_initGraphics(const bn::camera_ptr& camera)
{
    setVisible(false);

    _bg.set_camera(camera);
}

const bn::camera_ptr& DungeonBg::_getCamera() const
{
    BN_ASSERT(_bg.camera(), "Camera was not attached to DungeonBg::_bg");
    return *_bg.camera();
}

/**
 * @brief Get camera rect slightly smaller than 256x176, with the center position.
 */
static bn::fixed_rect _getCamRect(const bn::fixed_point& center)
{
    return bn::fixed_rect(center, {bn::display::width() + 8 * 2 - 0.5, bn::display::height() + 8 * 2 - 0.5});
}

/**
 * @brief Return two `bn::fixed_rect` that is moved to positive number range, while preserving relative position.
 */
static auto _getMovedRectToPositive(bn::fixed_rect r1, bn::fixed_rect r2) -> bn::pair<bn::fixed_rect, bn::fixed_rect>
{
    while (r1.left() < 0 || r2.left() < 0)
    {
        const s32 diff = (bn::max(bn::abs(r1.left()), bn::abs(r2.left())) / (DungeonBg::COLUMNS * 8)).ceil_integer() *
                         (DungeonBg::COLUMNS * 8);
        r1.set_x(r1.x() + diff);
        r2.set_x(r2.x() + diff);
    }
    while (r1.top() < 0 || r2.top() < 0)
    {
        const s32 diff = (bn::max(bn::abs(r1.top()), bn::abs(r2.top())) / (DungeonBg::ROWS * 8)).ceil_integer() *
                         (DungeonBg::ROWS * 8);
        r1.set_y(r1.y() + diff);
        r2.set_y(r2.y() + diff);
    }
    return {r1, r2};
}

/**
 * @brief Clamp `bn::fixed_point` to the range [0..256)
 */
static bn::fixed_point _clampedPosToBg(bn::fixed_point point)
{
    while (point.x() < 0)
    {
        const s32 diff = (bn::abs(point.x()) / (DungeonBg::COLUMNS * 8)).ceil_integer() * (DungeonBg::COLUMNS * 8);
        point.set_x(point.x() + diff);
    }
    while (point.x() >= DungeonBg::COLUMNS * 8)
    {
        const s32 diff = (point.x() / (DungeonBg::COLUMNS * 8)).floor_integer() * (DungeonBg::COLUMNS * 8);
        point.set_x(point.x() - diff);
    }
    while (point.y() < 0)
    {
        const s32 diff = (bn::abs(point.y()) / (DungeonBg::ROWS * 8)).ceil_integer() * (DungeonBg::ROWS * 8);
        point.set_y(point.y() + diff);
    }
    while (point.y() >= DungeonBg::ROWS * 8)
    {
        const s32 diff = (point.y() / (DungeonBg::ROWS * 8)).floor_integer() * (DungeonBg::ROWS * 8);
        point.set_y(point.y() - diff);
    }
    BN_ASSERT(0 <= point.x() && point.x() < DungeonBg::COLUMNS * 8, "clamp faild! point.x(): ", point.x());
    BN_ASSERT(0 <= point.y() && point.y() < DungeonBg::ROWS * 8, "clamp faild! point.y(): ", point.y());
    return point;
}

static bn::point _convertPosToCellPos(const bn::fixed_point& point)
{
    BN_ASSERT(0 <= point.x() && point.x() < DungeonBg::COLUMNS * 8, "point.x(): ", point.x(), ", OOB");
    BN_ASSERT(0 <= point.y() && point.y() < DungeonBg::ROWS * 8, "point.y(): ", point.y(), ", OOB");

    const bn::point result = {(point.x() / 8).floor_integer(), (point.y() / 8).floor_integer()};

    BN_ASSERT(0 <= result.x() && result.x() < DungeonBg::COLUMNS, "result.x(): ", result.x(), ", OOB");
    BN_ASSERT(0 <= result.y() && result.y() < DungeonBg::ROWS, "result.y(): ", result.y(), ", OOB");
    return result;
}

void DungeonBg::_redrawScrolledCells(s32 scrollPhase, const DungeonFloor& dungeonFloor, const mob::Monster& player)
{
    // BN_ERROR("WIP: redraw scrolled cells");

    BN_ASSERT(0 <= scrollPhase && scrollPhase < 2, "Invalid scrollPhase(", scrollPhase, ")");
    _cellsReloadRequired = true;

    constexpr bn::fixed_point BG_SIZE_HALF = bn::fixed_point{COLUMNS * 8 / 2, ROWS * 8 / 2};

    auto camRect = _getCamRect(_getCamera().position());
    const BoardPos playerBoardPos = player.getBoardPos();

    // TODO: Clean up messy & copy-pasta code

    // update the camera boundary cells
    switch (_bgScrollDirection)
    {
        using Dir9 = Direction9;
        using FloorType = DungeonFloor::Type;
        using Neighbor3x3 = DungeonFloor::Neighbor3x3;
    // update the right column
    case Dir9::RIGHT: {
        const s32 x = _convertPosToCellPos(_clampedPosToBg(camRect.top_right() + BG_SIZE_HALF)).x();
        const s32 startCellY = _convertPosToCellPos(_clampedPosToBg(camRect.top_right() + BG_SIZE_HALF)).y();
        const s32 endCellY = _convertPosToCellPos(_clampedPosToBg(camRect.bottom_right() + BG_SIZE_HALF)).y();
        s32 updatedTileCount = 0;
        for (s32 y = startCellY;; y = (y + 1) % ROWS, ++updatedTileCount)
        {
            auto& cell = _cells[_mapItem.cell_index(x, y)];
            const Neighbor3x3 neighbors = dungeonFloor.getNeighborsOf(
                playerBoardPos + BoardPos{(s8)(scrollPhase == 1 ? 8 : 7), (s8)(-5 + updatedTileCount / 2)});
            cell = _metaTileset->getCell(neighbors, (scrollPhase == 0 ? 1 : 0), updatedTileCount % 2);

            if (y == endCellY)
                break;
        }
        BN_ASSERT(updatedTileCount + 1 == 22, "updatedTileCount+1 is ", updatedTileCount + 1, ", instead of 22");
    }
    break;
    // update the left column
    case Dir9::LEFT: {
        const s32 x = _convertPosToCellPos(_clampedPosToBg(camRect.top_left() + BG_SIZE_HALF)).x();
        const s32 startCellY = _convertPosToCellPos(_clampedPosToBg(camRect.top_left() + BG_SIZE_HALF)).y();
        const s32 endCellY = _convertPosToCellPos(_clampedPosToBg(camRect.bottom_left() + BG_SIZE_HALF)).y();
        s32 updatedTileCount = 0;
        for (s32 y = startCellY;; y = (y + 1) % ROWS, ++updatedTileCount)
        {
            auto& cell = _cells[_mapItem.cell_index(x, y)];
            const Neighbor3x3 neighbors = dungeonFloor.getNeighborsOf(
                playerBoardPos + BoardPos{(s8)(scrollPhase == 1 ? -8 : -7), (s8)(-5 + updatedTileCount / 2)});
            cell = _metaTileset->getCell(neighbors, (scrollPhase == 0 ? 0 : 1), updatedTileCount % 2);

            if (y == endCellY)
                break;
        }
        BN_ASSERT(updatedTileCount + 1 == 22, "updatedTileCount+1 is ", updatedTileCount + 1, ", instead of 22");
    }
    break;
    // update the top row
    case Dir9::UP: {
        const s32 y = _convertPosToCellPos(_clampedPosToBg(camRect.top_left() + BG_SIZE_HALF)).y();
        const s32 startCellX = _convertPosToCellPos(_clampedPosToBg(camRect.top_left() + BG_SIZE_HALF)).x();
        const s32 endCellX = _convertPosToCellPos(_clampedPosToBg(camRect.top_right() + BG_SIZE_HALF)).x();
        s32 updatedTileCount = 0;
        for (s32 x = startCellX;; x = (x + 1) % COLUMNS, ++updatedTileCount)
        {
            auto& cell = _cells[_mapItem.cell_index(x, y)];
            const Neighbor3x3 neighbors =
                dungeonFloor.getNeighborsOf(playerBoardPos + BoardPos{(s8)(-8 + (updatedTileCount + 1) / 2), -5});
            cell = _metaTileset->getCell(neighbors, (updatedTileCount + 1) % 2, (scrollPhase == 0 ? 1 : 0));

            if (x == endCellX)
                break;
        }
        BN_ASSERT(updatedTileCount + 1 == 32, "updatedTileCount+1 is ", updatedTileCount + 1, ", instead of 32");
    }
    break;
    // update the bottom row
    case Dir9::DOWN: {
        const s32 y = _convertPosToCellPos(_clampedPosToBg(camRect.bottom_left() + BG_SIZE_HALF)).y();
        const s32 startCellX = _convertPosToCellPos(_clampedPosToBg(camRect.bottom_left() + BG_SIZE_HALF)).x();
        const s32 endCellX = _convertPosToCellPos(_clampedPosToBg(camRect.bottom_right() + BG_SIZE_HALF)).x();
        s32 updatedTileCount = 0;
        for (s32 x = startCellX;; x = (x + 1) % COLUMNS, ++updatedTileCount)
        {
            auto& cell = _cells[_mapItem.cell_index(x, y)];
            const Neighbor3x3 neighbors =
                dungeonFloor.getNeighborsOf(playerBoardPos + BoardPos{(s8)(-8 + (updatedTileCount + 1) / 2), 5});
            cell = _metaTileset->getCell(neighbors, (updatedTileCount + 1) % 2, (scrollPhase == 0 ? 0 : 1));

            if (x == endCellX)
                break;
        }
        BN_ASSERT(updatedTileCount + 1 == 32, "updatedTileCount+1 is ", updatedTileCount + 1, ", instead of 32");
    }
    break;

    default:
        BN_ERROR("Invalid scroll direction(", (s32)_bgScrollDirection, ")");
    }
}

} // namespace mp::game
