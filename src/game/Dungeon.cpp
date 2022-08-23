/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/Dungeon.hpp"

#include "bn_keypad.h"
#include "iso_bn_random.h"

#include "constants.hpp"
#include "game/mob/MonsterAction.hpp"
#include "game/mob/MonsterSpecies.hpp"

namespace mp::game
{

Dungeon::Dungeon(iso_bn::random& rng) : _rng(rng), _player(mob::MonsterSpecies::MISSING_NO, {0, 0})
{
#ifdef MP_DEBUG
    _testMapGen();
#endif

    _miniMap.updateBgPos(_player);
    _miniMap.setVisible(true);
    _player.setVisible(true);
}

auto Dungeon::update() -> bn::optional<scene::SceneType>
{
    _handleInput();

    _player.update(*this);
    _miniMap.update();

    return bn::nullopt;
}

bool Dungeon::isTurnOngoing() const
{
    return _turnProgressDelayCounter > 0;
}

#ifdef MP_DEBUG
void Dungeon::_testMapGen()
{
    _player.setBoardPos(DungeonFloor::COLUMNS / 2, DungeonFloor::ROWS / 2);
    _miniMap.updateBgPos(_player);

    _floor.generate(_rng);
    _bg.redrawAll(_floor);
    _miniMap.redrawAll(_floor);
}
#endif

void Dungeon::_handleInput()
{
    // Don't receive any input if the turn is ongoing.
    if (isTurnOngoing())
    {
        if (!_updateTurnOngoing())
            return;
    }

#ifdef MP_DEBUG
    if (bn::keypad::select_held() && bn::keypad::l_pressed())
        _testMapGen();
#endif

    // player movement (with mini-map movement)
    if (bn::keypad::left_held() || bn::keypad::right_held() || bn::keypad::up_held() || bn::keypad::down_held())
    {
        using ActionType = mob::MonsterAction::Type;

        const Direction9 inputDirection = getDirectionFromKeyHold();
        const auto candidatePlayerPos = _player.getBoardPos() + convertDir9ToPos(inputDirection);

        // if player can move to the input direction, move to there.
        if (_canMoveTo(_player, candidatePlayerPos))
        {
            _player.actPlayer(mob::MonsterAction(inputDirection, ActionType::MOVE));
            _miniMap.updateBgPos(_player);
            _startTurnOngoing();
        }
        // if not, just change the player's direction without moving.
        else
        {
            _player.actPlayer(mob::MonsterAction(inputDirection, ActionType::CHANGE_DIRECTION));
        }
    }
}

void Dungeon::_startTurnOngoing()
{
    _turnProgressDelayCounter = consts::TURN_PROGRESS_DELAY;
}

bool Dungeon::_updateTurnOngoing()
{
    --_turnProgressDelayCounter;
    return _turnProgressDelayCounter <= 0;
}

bool Dungeon::_canMoveTo(const mob::Monster& mob, const BoardPos& destination) const
{
    using FloorType = DungeonFloor::Type;
    if (_floor.getTile(destination) == FloorType::WALL)
        return false;
    // check diagonal adjacent wall
    if (_floor.getTile(destination.x, mob.getBoardPos().y) == FloorType::WALL ||
        _floor.getTile(mob.getBoardPos().x, destination.y) == FloorType::WALL)
        return false;

    // collide with player
    if (destination == _player.getBoardPos())
        return false;

    // TODO: Add collide with _monsters

    return true;
}

} // namespace mp::game
