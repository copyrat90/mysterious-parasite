/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_camera_actions.h"
#include "bn_camera_ptr.h"
#include "bn_forward_list.h"
#include "bn_optional.h"

#include "constants.hpp"
#include "game/DungeonBg.hpp"
#include "game/DungeonFloor.hpp"
#include "game/Hud.hpp"
#include "game/MiniMap.hpp"
#include "game/item/Item.hpp"
#include "game/item/ItemUse.hpp"
#include "game/mob/Monster.hpp"
#include "game/mob/Player.hpp"
#include "scene/SceneType.hpp"

namespace iso_bn
{
class random;
}
namespace mp
{
class TextGen;
}

namespace mp::game
{

class Dungeon final
{
public:
    Dungeon(iso_bn::random& rng, TextGen&, Settings&);

    [[nodiscard]] auto update() -> bn::optional<scene::SceneType>;

    bool isTurnOngoing() const;

private:
    /**
     * @brief Receive user input, and progress a turn.
     *
     * @return `true` if player is still alive after the turn.
     */
    [[nodiscard]] bool _progressTurn();

    void _startBgScroll(Direction9 moveDir);
    bool _updateBgScroll();

    bool _canMoveTo(const mob::Monster&, const BoardPos& destination) const;

#ifdef MP_DEBUG
private:
    void _testMapGen();
#endif

private:
    iso_bn::random& _rng;
    Settings& _settings;

    bn::camera_ptr _camera;
    bn::optional<bn::camera_move_to_action> _camMoveAction;

    DungeonFloor _floor;
    DungeonBg _bg;
    MiniMap _miniMap;
    Hud _hud;
    item::ItemUse _itemUse;

    mob::Player _player;
    bn::forward_list<mob::Monster, consts::DUNGEON_MOB_MAX_COUNT> _monsters;
    bn::forward_list<item::Item, consts::DUNGEON_ITEM_MAX_COUNT> _items;
};

} // namespace mp::game
