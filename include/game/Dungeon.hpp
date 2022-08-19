/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_vector.h"

#include "constants.hpp"
#include "game/DungeonBg.hpp"
#include "game/DungeonFloor.hpp"
#include "game/MiniMap.hpp"
#include "game/mob/Monster.hpp"
#include "scene/SceneType.hpp"

namespace iso_bn
{
class random;
}

namespace mp::game
{

class Dungeon final
{
public:
    Dungeon(iso_bn::random& rng);

    [[nodiscard]] auto update() -> bn::optional<scene::SceneType>;

private:
    iso_bn::random& _rng;

    DungeonFloor _floor;
    DungeonBg _bg;
    MiniMap _miniMap;

    mob::Monster _player;
    bn::vector<mob::Monster, consts::DUNGEON_MOB_MAX_COUNT> _monsters;
    bn::vector<item::Item, consts::DUNGEON_ITEM_MAX_COUNT> _items;
};

} // namespace mp::game
