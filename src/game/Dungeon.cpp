/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/Dungeon.hpp"

#include "bn_assert.h"

namespace mp::game
{

Dungeon::Dungeon(iso_bn::random& rng, TextGen& textGen, Settings& settings)
    : _rng(rng), _settings(settings), _camera(bn::camera_ptr::create(consts::INIT_CAM_POS)), _bg(_camera),
      _hud(textGen, settings), _itemUse(_hud), _player({0, 0}, _camera, _hud), _idleState(*this), _pickPosState(*this),
      _playerActState(*this), _movingState(*this), _enemyActState(*this), _gameOverState(*this)
{
    _hud.setBelly(_player.getBelly().getCurrentBelly(), _player.getBelly().getMaxBelly());

    // TODO: Move mini-map to seperate screen
    // _miniMap.updateBgPos(_player);
    // _miniMap.setVisible(true);

    _hud.setVisible(true);
    _player.setVisible(true);
    _bg.setVisible(true);
}

auto Dungeon::update() -> bn::optional<scene::SceneType>
{
    // bool isPlayerAlive = _progressTurn();
    auto nextStateArgs = _updateState();

    _player.update(*this);
    for (auto& monster : _monsters)
        monster.update(*this);

    // TODO: Move mini-map to seperate screen
    // _miniMap.update();

    _bg.update(_floor, _player);

    if (nextStateArgs.has_value() && nextStateArgs->shouldChangeState())
    {
        _currentState->onExit();

        switch (nextStateArgs->getNextStateKind())
        {
            using StKind = state::GameStateKind;
        // special states
        case StKind::RESTART_GAME:
            return scene::SceneType::GAME;
        // normal states
        default:
            _changeState(*nextStateArgs);
        }

        _currentState->onEnter(*nextStateArgs);
    }

    return bn::nullopt;
}

bool Dungeon::canMoveTo(const mob::Monster& mob, const BoardPos& destination) const
{
    using FloorType = DungeonFloor::Type;
    if (_floor.getFloorTypeOf(destination) == FloorType::WALL)
        return false;
    // check diagonal adjacent wall
    if (_floor.getFloorTypeOf(destination.x, mob.getBoardPos().y) == FloorType::WALL ||
        _floor.getFloorTypeOf(mob.getBoardPos().x, destination.y) == FloorType::WALL)
        return false;

    // collide with player
    if (destination == _player.getBoardPos())
        return false;

    // TODO: Add collide with _monsters

    return true;
}

auto Dungeon::_updateState() -> bn::optional<state::GameStateArgs>
{
    auto nextState1 = _currentState->handleInput();
    auto nextState2 = _currentState->update();

    BN_ASSERT(!(nextState1.has_value() && nextState1->shouldChangeState() && nextState2.has_value() &&
                nextState2->shouldChangeState()),
              "Both state::handleInput() and state::update() returned nextState");

    if (nextState1.has_value() && nextState1->shouldChangeState())
        return nextState1;
    if (nextState2.has_value() && nextState2->shouldChangeState())
        return nextState2;

    return bn::nullopt;
}

void Dungeon::_changeState(const state::GameStateArgs& nextStateArgs)
{
    switch (nextStateArgs.getNextStateKind())
    {
        using StKind = state::GameStateKind;
    case StKind::IDLE:
        _currentState = &_idleState;
        break;
    case StKind::PICK_POS:
        _currentState = &_pickPosState;
        break;
    case StKind::PLAYER_ACT:
        _currentState = &_playerActState;
        break;
    case StKind::MOVING:
        _currentState = &_movingState;
        break;
    case StKind::ENEMY_ACT:
        _currentState = &_enemyActState;
        break;
    case StKind::GAME_OVER:
        _currentState = &_gameOverState;
        break;

    default:
        BN_ERROR("Invalid nextStateKind(", (int)nextStateArgs.getNextStateKind(), ")");
    }
}

auto Dungeon::getRng() -> iso_bn::random&
{
    return _rng;
}

auto Dungeon::getSettings() -> Settings&
{
    return _settings;
}

auto Dungeon::getCamera() -> bn::camera_ptr&
{
    return _camera;
}

auto Dungeon::getFloor() -> DungeonFloor&
{
    return _floor;
}

auto Dungeon::getDungeonBg() -> DungeonBg&
{
    return _bg;
}

auto Dungeon::getHud() -> Hud&
{
    return _hud;
}

auto Dungeon::getItemUse() -> item::ItemUse&
{
    return _itemUse;
}

auto Dungeon::getPlayer() -> mob::Player&
{
    return _player;
}

auto Dungeon::getMonsters() -> decltype(_monsters)&
{
    return _monsters;
}

auto Dungeon::getItems() -> decltype(_items)&
{
    return _items;
}

auto Dungeon::getCurrentStateKind() const -> state::GameStateKind
{
    return _currentState->getStateKind();
}

} // namespace mp::game
