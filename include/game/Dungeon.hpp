/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_forward_list.h"

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

    bool isTurnOngoing() const;

private:
    void _handleInput();

    void _startTurnOngoing();
    /**
     * @brief Update the turn progress delay countdown.
     * @return whether the input should be received now.
     */
    bool _updateTurnOngoing();

    bool _canMoveTo(const mob::Monster&, const BoardPos& destination) const;

#ifdef MP_DEBUG
private:
    void _testMapGen();
#endif

private:
    iso_bn::random& _rng;

    DungeonFloor _floor;
    DungeonBg _bg;
    MiniMap _miniMap;

    mob::Monster _player;
    bn::forward_list<mob::Monster, consts::DUNGEON_MOB_MAX_COUNT> _monsters;
    bn::forward_list<item::Item, consts::DUNGEON_ITEM_MAX_COUNT> _items;

    s32 _turnProgressDelayCounter = 0;
};

} // namespace mp::game
