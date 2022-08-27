/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/DungeonBg.hpp"

#include "game/DungeonFloor.hpp"

namespace mp::game
{

// TODO: Pass MetaTilesetKind parameter, and init `_metaTileset` with it.
DungeonBg::DungeonBg()
    : _metaTileset(&MetaTileset::fromKind(MetaTilesetKind::PLACEHOLDER)), _cells{},
      _mapItem(_cells[0], bn::size(DungeonBg::COLUMNS, DungeonBg::ROWS)),
      _bgItem(_metaTileset->getBgItem().tiles_item(), _metaTileset->getBgItem().palette_item(), _mapItem),
      _bg(_bgItem.create_bg(0, 0)), _bgMap(_bg.map())
{
}

void DungeonBg::update()
{
    if (isVisible())
    {
    }

    if (_cellsReloadRequired)
    {
        _cellsReloadRequired = false;
        _bgMap.reload_cells_ref();
    }
}

void DungeonBg::redrawAll(const DungeonFloor& board)
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

} // namespace mp::game
