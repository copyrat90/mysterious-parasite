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
#include "bn_blending.h"
#include "bn_camera_ptr.h"
#include "bn_display.h"
#include "bn_fixed_rect.h"
#include "bn_math.h"
#include "bn_point.h"
#include "bn_utility.h"

#include "constants.hpp"
#include "game/DungeonFloor.hpp"
#include "game/MetaTileset.hpp"
#include "game/ShadowTileset.hpp"
#include "game/mob/Monster.hpp"

namespace mp::game
{

namespace
{
constexpr bn::fixed_point BG_SIZE_HALF = {DungeonBg::COLUMNS * 8 / 2, DungeonBg::ROWS * 8 / 2};
}

// TODO: Pass MetaTilesetKind parameter, and init `_metaTileset` with it.
DungeonBg::DungeonBg(const bn::camera_ptr& camera)
    : _metaTileset(&MetaTileset::fromKind(MetaTilesetKind::PLACEHOLDER)),
      _shadowTileset(ShadowTileset::get()), _dunCells{}, _shadowCells{}, _darkCells{},
      // dungeon bg init
      _dunMapItem(_dunCells[0], bn::size(DungeonBg::COLUMNS, DungeonBg::ROWS)),
      _dunBgItem(_metaTileset->getBgItem().tiles_item(), _metaTileset->getBgItem().palette_item(), _dunMapItem),
      _dunBg(_dunBgItem.create_bg(0, 0)), _dunBgMap(_dunBg.map()),
      // shadow bg init
      _shadowMapItem(_shadowCells[0], bn::size(DungeonBg::COLUMNS, DungeonBg::ROWS)),
      _shadowBgItem(_shadowTileset.getBgItem().tiles_item(), _shadowTileset.getBgItem().palette_item(), _shadowMapItem),
      _shadowBg(_shadowBgItem.create_bg(0, 0)), _shadowBgMap(_shadowBg.map()),
      // dark bg init
      _darkMapItem(_darkCells[0], bn::size(DungeonBg::COLUMNS, DungeonBg::ROWS)),
      _darkBgItem(_shadowTileset.getBgItem().tiles_item(), _shadowTileset.getBgItem().palette_item(), _darkMapItem),
      _darkBg(_darkBgItem.create_bg(0, 0)), _darkBgMap(_darkBg.map())
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
        _dunBgMap.reload_cells_ref();
        _shadowBgMap.reload_cells_ref();
        _darkBgMap.reload_cells_ref();
    }
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
    return _dunBg.visible();
}

void DungeonBg::setVisible(bool isVisible)
{
    _dunBg.set_visible(isVisible);
    _shadowBg.set_visible(isVisible);
    _darkBg.set_visible(isVisible);
}

void DungeonBg::_initGraphics(const bn::camera_ptr& camera)
{
    setVisible(false);

    _dunBg.set_priority(consts::DUNGEON_BG_PRIORITY);
    _shadowBg.set_priority(consts::DUNGEON_BG_PRIORITY);
    _darkBg.set_priority(consts::DUNGEON_BG_PRIORITY);

    _dunBg.set_z_order(consts::DUNGEON_BG_Z_ORDER_WALLS);
    _shadowBg.set_z_order(consts::DUNGEON_BG_Z_ORDER_SHADOWS);
    _darkBg.set_z_order(consts::DUNGEON_BG_Z_ORDER_DARK);

    _shadowBg.set_blending_enabled(true);
    bn::blending::set_transparency_alpha(0.5);

    _dunBg.set_camera(camera);
    _shadowBg.set_camera(camera);
    _darkBg.set_camera(camera);
}

const bn::camera_ptr& DungeonBg::_getCamera() const
{
    BN_ASSERT(_dunBg.camera(), "Camera was not attached to DungeonBg::_bg");
    return *_dunBg.camera();
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

void DungeonBg::redrawAll(const DungeonFloor& dungeonFloor, const mob::Monster& player)
{
    using Neighbor3x3 = DungeonFloor::Neighbor3x3;
    using NeighborBrightness3x3 = DungeonFloor::NeighborBrightness3x3;
    using NeighborDiscover3x3 = DungeonFloor::NeighborDiscover3x3;

    _cellsReloadRequired = true;

    auto camRect = _getCamRect(_getCamera().position());
    const BoardPos& playerBoardPos = player.getBoardPos();

    auto topLeftCell = _convertPosToCellPos(_clampedPosToBg(camRect.top_left() + BG_SIZE_HALF));
    auto bottomRightCell = _convertPosToCellPos(_clampedPosToBg(camRect.bottom_right() + BG_SIZE_HALF));

    s32 updatedTileCount = 0;
    // from left-top to bottom-right tiles on screen
    for (s32 y = topLeftCell.y();; y = (y + 1) % ROWS)
    {
        for (s32 x = topLeftCell.x();; x = (x + 1) % COLUMNS)
        {
            // find the current board pos of this cell's meta-tile.
            const BoardPos metaTilePos = playerBoardPos + BoardPos{(s8)(-8 + (updatedTileCount % COLUMNS + 1) / 2),
                                                                   (s8)(-5 + updatedTileCount / (COLUMNS * 2))};
            // get the neighbors of the meta-tile
            const Neighbor3x3 neighbors = dungeonFloor.getNeighborsOf(metaTilePos);
            // get the right tile within the meta-tile, and assign it to current cell.
            const s32 bgTileX = (updatedTileCount % 2 == 0 ? 1 : 0);
            const s32 bgTileY = updatedTileCount / COLUMNS % 2;
            _dunCells[_dunMapItem.cell_index(x, y)] = _metaTileset->getCell(neighbors, bgTileX, bgTileY);

            // do the same thing with shadow area.
            const NeighborBrightness3x3 brightnesses = dungeonFloor.getNeighborBrightnessOf(metaTilePos);
            _shadowCells[_shadowMapItem.cell_index(x, y)] = _shadowTileset.getCell(brightnesses, bgTileX, bgTileY);

            // do the same thing with dark area.
            const NeighborDiscover3x3 discovers = dungeonFloor.getNeighborDiscoverOf(metaTilePos);
            _darkCells[_darkMapItem.cell_index(x, y)] = _shadowTileset.getCell(discovers, bgTileX, bgTileY);

            ++updatedTileCount;
            if (x == bottomRightCell.x())
                break;
        }
        if (y == bottomRightCell.y())
            break;
    }
    BN_ASSERT(updatedTileCount == 22 * COLUMNS, "updatedTileCount is ", updatedTileCount, ", instead of ",
              22 * COLUMNS);
}

void DungeonBg::_redrawScrolledCells(s32 scrollPhase, const DungeonFloor& dungeonFloor, const mob::Monster& player)
{
    BN_ASSERT(0 <= scrollPhase && scrollPhase < 2, "Invalid scrollPhase(", scrollPhase, ")");
    _cellsReloadRequired = true;

    auto camRect = _getCamRect(_getCamera().position());
    const BoardPos& playerBoardPos = player.getBoardPos();

    // TODO: Clean up messy & copy-pasta code

    // update the camera boundary cells
    switch (_bgScrollDirection)
    {
        using Dir9 = Direction9;
        using Neighbor3x3 = DungeonFloor::Neighbor3x3;
        using NeighborBrightness3x3 = DungeonFloor::NeighborBrightness3x3;
        using NeighborDiscover3x3 = DungeonFloor::NeighborDiscover3x3;
    // update the right column
    case Dir9::RIGHT: {
        const s32 x = _convertPosToCellPos(_clampedPosToBg(camRect.top_right() + BG_SIZE_HALF)).x();
        const s32 startCellY = _convertPosToCellPos(_clampedPosToBg(camRect.top_right() + BG_SIZE_HALF)).y();
        const s32 endCellY = _convertPosToCellPos(_clampedPosToBg(camRect.bottom_right() + BG_SIZE_HALF)).y();
        s32 updatedTileCount = 0;
        for (s32 y = startCellY;; y = (y + 1) % ROWS)
        {
            const BoardPos metaTilePos =
                playerBoardPos + BoardPos{(s8)(scrollPhase == 1 ? 8 : 7), (s8)(-5 + updatedTileCount / 2)};
            const Neighbor3x3 neighbors = dungeonFloor.getNeighborsOf(metaTilePos);
            const s32 bgTileX = (scrollPhase == 0 ? 1 : 0);
            const s32 bgTileY = updatedTileCount % 2;
            _dunCells[_dunMapItem.cell_index(x, y)] = _metaTileset->getCell(neighbors, bgTileX, bgTileY);

            const NeighborBrightness3x3 brightnesses = dungeonFloor.getNeighborBrightnessOf(metaTilePos);
            _shadowCells[_shadowMapItem.cell_index(x, y)] = _shadowTileset.getCell(brightnesses, bgTileX, bgTileY);
            const NeighborDiscover3x3 discovers = dungeonFloor.getNeighborDiscoverOf(metaTilePos);
            _darkCells[_darkMapItem.cell_index(x, y)] = _shadowTileset.getCell(discovers, bgTileX, bgTileY);

            ++updatedTileCount;
            if (y == endCellY)
                break;
        }
        BN_ASSERT(updatedTileCount == 22, "updatedTileCount is ", updatedTileCount, ", instead of 22");
    }
    break;
    // update the left column
    case Dir9::LEFT: {
        const s32 x = _convertPosToCellPos(_clampedPosToBg(camRect.top_left() + BG_SIZE_HALF)).x();
        const s32 startCellY = _convertPosToCellPos(_clampedPosToBg(camRect.top_left() + BG_SIZE_HALF)).y();
        const s32 endCellY = _convertPosToCellPos(_clampedPosToBg(camRect.bottom_left() + BG_SIZE_HALF)).y();
        s32 updatedTileCount = 0;
        for (s32 y = startCellY;; y = (y + 1) % ROWS)
        {
            const BoardPos metaTilePos =
                playerBoardPos + BoardPos{(s8)(scrollPhase == 1 ? -8 : -7), (s8)(-5 + updatedTileCount / 2)};
            const Neighbor3x3 neighbors = dungeonFloor.getNeighborsOf(metaTilePos);
            const s32 bgTileX = (scrollPhase == 0 ? 0 : 1);
            const s32 bgTileY = updatedTileCount % 2;
            _dunCells[_dunMapItem.cell_index(x, y)] = _metaTileset->getCell(neighbors, bgTileX, bgTileY);

            const NeighborBrightness3x3 brightnesses = dungeonFloor.getNeighborBrightnessOf(metaTilePos);
            _shadowCells[_shadowMapItem.cell_index(x, y)] = _shadowTileset.getCell(brightnesses, bgTileX, bgTileY);
            const NeighborDiscover3x3 discovers = dungeonFloor.getNeighborDiscoverOf(metaTilePos);
            _darkCells[_darkMapItem.cell_index(x, y)] = _shadowTileset.getCell(discovers, bgTileX, bgTileY);

            ++updatedTileCount;
            if (y == endCellY)
                break;
        }
        BN_ASSERT(updatedTileCount == 22, "updatedTileCount is ", updatedTileCount, ", instead of 22");
    }
    break;
    // update the top row
    case Dir9::UP: {
        const s32 y = _convertPosToCellPos(_clampedPosToBg(camRect.top_left() + BG_SIZE_HALF)).y();
        const s32 startCellX = _convertPosToCellPos(_clampedPosToBg(camRect.top_left() + BG_SIZE_HALF)).x();
        const s32 endCellX = _convertPosToCellPos(_clampedPosToBg(camRect.top_right() + BG_SIZE_HALF)).x();
        s32 updatedTileCount = 0;
        for (s32 x = startCellX;; x = (x + 1) % COLUMNS)
        {
            const BoardPos metaTilePos = playerBoardPos + BoardPos{(s8)(-8 + (updatedTileCount + 1) / 2), -5};
            const Neighbor3x3 neighbors = dungeonFloor.getNeighborsOf(metaTilePos);
            const s32 bgTileX = (updatedTileCount + 1) % 2;
            const s32 bgTileY = (scrollPhase == 0 ? 1 : 0);
            _dunCells[_dunMapItem.cell_index(x, y)] = _metaTileset->getCell(neighbors, bgTileX, bgTileY);

            const NeighborBrightness3x3 brightnesses = dungeonFloor.getNeighborBrightnessOf(metaTilePos);
            _shadowCells[_shadowMapItem.cell_index(x, y)] = _shadowTileset.getCell(brightnesses, bgTileX, bgTileY);
            const NeighborDiscover3x3 discovers = dungeonFloor.getNeighborDiscoverOf(metaTilePos);
            _darkCells[_darkMapItem.cell_index(x, y)] = _shadowTileset.getCell(discovers, bgTileX, bgTileY);

            ++updatedTileCount;
            if (x == endCellX)
                break;
        }
        BN_ASSERT(updatedTileCount == 32, "updatedTileCount is ", updatedTileCount, ", instead of 32");
    }
    break;
    // update the bottom row
    case Dir9::DOWN: {
        const s32 y = _convertPosToCellPos(_clampedPosToBg(camRect.bottom_left() + BG_SIZE_HALF)).y();
        const s32 startCellX = _convertPosToCellPos(_clampedPosToBg(camRect.bottom_left() + BG_SIZE_HALF)).x();
        const s32 endCellX = _convertPosToCellPos(_clampedPosToBg(camRect.bottom_right() + BG_SIZE_HALF)).x();
        s32 updatedTileCount = 0;
        for (s32 x = startCellX;; x = (x + 1) % COLUMNS)
        {
            const BoardPos metaTilePos = playerBoardPos + BoardPos{(s8)(-8 + (updatedTileCount + 1) / 2), 5};
            const Neighbor3x3 neighbors = dungeonFloor.getNeighborsOf(metaTilePos);
            const s32 bgTileX = (updatedTileCount + 1) % 2;
            const s32 bgTileY = (scrollPhase == 0 ? 0 : 1);
            _dunCells[_dunMapItem.cell_index(x, y)] = _metaTileset->getCell(neighbors, bgTileX, bgTileY);

            const NeighborBrightness3x3 brightnesses = dungeonFloor.getNeighborBrightnessOf(metaTilePos);
            _shadowCells[_shadowMapItem.cell_index(x, y)] = _shadowTileset.getCell(brightnesses, bgTileX, bgTileY);
            const NeighborDiscover3x3 discovers = dungeonFloor.getNeighborDiscoverOf(metaTilePos);
            _darkCells[_darkMapItem.cell_index(x, y)] = _shadowTileset.getCell(discovers, bgTileX, bgTileY);

            ++updatedTileCount;
            if (x == endCellX)
                break;
        }
        BN_ASSERT(updatedTileCount == 32, "updatedTileCount is ", updatedTileCount, ", instead of 32");
    }
    break;

    default:
        BN_ERROR("Invalid scroll direction(", (s32)_bgScrollDirection, ")");
    }
}

} // namespace mp::game
