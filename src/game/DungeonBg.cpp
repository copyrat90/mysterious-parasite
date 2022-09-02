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

#include "game/DungeonFloor.hpp"
#include "game/MetaTileset.hpp"

namespace mp::game
{

// TODO: Pass MetaTilesetKind parameter, and init `_metaTileset` with it.
DungeonBg::DungeonBg(const bn::camera_ptr& camera, const DungeonFloor& dungeonFloor)
    : _metaTileset(&MetaTileset::fromKind(MetaTilesetKind::PLACEHOLDER)), _dungeonFloor(dungeonFloor), _cells{},
      _mapItem(_cells[0], bn::size(DungeonBg::COLUMNS, DungeonBg::ROWS)),
      _bgItem(_metaTileset->getBgItem().tiles_item(), _metaTileset->getBgItem().palette_item(), _mapItem),
      _bg(_bgItem.create_bg(0, 0)), _bgMap(_bg.map()), _prevCamPos(camera.position())
{
    _initGraphics(camera);
}

void DungeonBg::update()
{
    if (_prevCamPos != _getCamera().position())
    {
        _redrawNecessaryCells();
        _prevCamPos = _getCamera().position();
    }

    if (_cellsReloadRequired)
    {
        _cellsReloadRequired = false;
        _bgMap.reload_cells_ref();
    }
}

void DungeonBg::redrawAll()
{
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
 * @brief Get 240x176 camera rect with the center position.
 */
static bn::fixed_rect _getCamRect(const bn::fixed_point& center)
{
    return bn::fixed_rect(center, {bn::display::width(), bn::display::height() + 2 * 8});
}

/**
 * @brief Return two `bn::fixed_rect`s that is moved to positive number range, while preserving relative position.
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
        const s32 diff = (point.x() / (DungeonBg::COLUMNS * 8)).ceil_integer() * (DungeonBg::COLUMNS * 8);
        point.set_x(point.x() - diff);
    }
    while (point.y() < 0)
    {
        const s32 diff = (bn::abs(point.y()) / (DungeonBg::ROWS * 8)).ceil_integer() * (DungeonBg::ROWS * 8);
        point.set_y(point.y() + diff);
    }
    while (point.y() >= DungeonBg::ROWS * 8)
    {
        const s32 diff = (point.y() / (DungeonBg::ROWS * 8)).ceil_integer() * (DungeonBg::ROWS * 8);
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

    const bn::point result = {(point.x() / 8).ceil_integer(), (point.y() / 8).ceil_integer()};

    BN_ASSERT(0 <= result.x() && result.x() < DungeonBg::COLUMNS, "result.x()", result.x(), ", OOB");
    BN_ASSERT(0 <= result.y() && result.y() < DungeonBg::ROWS, "result.y()", result.y(), ", OOB");
    return result;
}

void DungeonBg::_redrawNecessaryCells()
{
    BN_ERROR("WIP: Not implemented yet");

    const bn::fixed_point& curCamPos = _getCamera().position();

    auto prevCamRect = _getCamRect(_prevCamPos);
    auto curCamRect = _getCamRect(curCamPos);
    auto [prevPosiCamRect, curPosiCamRect] = _getMovedRectToPositive(prevCamRect, curCamRect);

    // update the right column (if move right enough)
    if ((curPosiCamRect.right() / 8).ceil_integer() > (prevPosiCamRect.right() / 8).ceil_integer())
    {
        _cellsReloadRequired = true;
        const s32 x = _convertPosToCellPos(_clampedPosToBg(curCamRect.top_right())).x();
        const s32 startCellY = _convertPosToCellPos(_clampedPosToBg(curCamRect.top_right())).y();
        const s32 endCellY = _convertPosToCellPos(_clampedPosToBg(curCamRect.bottom_right())).y();
        for (s32 y = startCellY;; y = (y + 1) % ROWS)
        {
            auto& cell = _cells[_mapItem.cell_index(x, y)];
            // WIP: Load & set meta-tile

            if (y == endCellY)
                break;
        }
    }
    // update the left column (if move left enough)
    else if ((curPosiCamRect.left() / 8).floor_integer() < (prevPosiCamRect.left() / 8).floor_integer())
    {
        _cellsReloadRequired = true;
    }
    // update the bottom row (if move down enough)
    if ((curPosiCamRect.bottom() / 8).ceil_integer() > (prevPosiCamRect.bottom() / 8).ceil_integer())
    {
        _cellsReloadRequired = true;
    }
    // update the top row (if move up enough)
    else if ((curPosiCamRect.top() / 8).floor_integer() < (prevPosiCamRect.top() / 8).floor_integer())
    {
        _cellsReloadRequired = true;
    }
}

} // namespace mp::game
