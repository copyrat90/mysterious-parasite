/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_camera_ptr.h"
#include "bn_forward_list.h"

#include "constants.hpp"
#include "game/DungeonBg.hpp"
#include "game/DungeonFloor.hpp"
#include "game/Hud.hpp"
#include "game/item/Item.hpp"
#include "game/item/ItemUse.hpp"
#include "game/mob/Monster.hpp"
#include "game/mob/Player.hpp"
#include "game/state/EnemyActState.hpp"
#include "game/state/GameOverState.hpp"
#include "game/state/IdleState.hpp"
#include "game/state/MovingState.hpp"
#include "game/state/PickPosState.hpp"
#include "game/state/PlayerActState.hpp"
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

    bool canMoveTo(const mob::Monster&, const BoardPos& destination) const;

private:
    auto _updateState() -> bn::optional<state::GameStateArgs>;
    void _changeState(const state::GameStateArgs& nextStateArgs);

private:
    iso_bn::random& _rng;
    Settings& _settings;

    bn::camera_ptr _camera;

    DungeonFloor _floor;
    DungeonBg _bg;
    Hud _hud;
    item::ItemUse _itemUse;

    mob::Player _player;
    bn::forward_list<mob::Monster, consts::DUNGEON_MOB_MAX_COUNT> _monsters;
    bn::forward_list<item::Item, consts::DUNGEON_ITEM_MAX_COUNT> _items;

public:
    auto getRng() -> iso_bn::random&;
    auto getSettings() -> Settings&;

    auto getCamera() -> bn::camera_ptr&;

    auto getFloor() -> DungeonFloor&;
    auto getDungeonBg() -> DungeonBg&;
    auto getHud() -> Hud&;
    auto getItemUse() -> item::ItemUse&;

    auto getPlayer() -> mob::Player&;
    auto getMonsters() -> decltype(_monsters)&;
    auto getItems() -> decltype(_items)&;

    auto getCurrentStateKind() const -> state::GameStateKind;

private:
    state::GameState* _currentState = &_idleState;
    state::IdleState _idleState;
    state::PickPosState _pickPosState;
    state::PlayerActState _playerActState;
    state::MovingState _movingState;
    state::EnemyActState _enemyActState;
    state::GameOverState _gameOverState;
};

} // namespace mp::game
