/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/state/GameStateArgs.hpp"

#include "bn_assert.h"

namespace mp::game::state
{

GameStateArgs::GameStateArgs(GameStateKind prevKind, GameStateKind nextKind) : _prevKind(prevKind), _nextKind(nextKind)
{
}

bool GameStateArgs::shouldChangeState() const
{
    return _prevKind != _nextKind;
}

auto GameStateArgs::getPrevStateKind() const -> GameStateKind
{
    return _prevKind;
}

auto GameStateArgs::getNextStateKind() const -> GameStateKind
{
    return _nextKind;
}

void GameStateArgs::setNextStateKind(GameStateKind nextKind)
{
    _nextKind = nextKind;
}

auto GameStateArgs::getBoardPos() const -> const BoardPos&
{
    BN_ASSERT(_boardPos.has_value(), "Args::_boardPos is empty");
    return *_boardPos;
}

void GameStateArgs::setBoardPos(const BoardPos& boardPos)
{
    _boardPos = boardPos;
}

auto GameStateArgs::getDirection() const -> Direction9
{
    BN_ASSERT(_direction.has_value(), "Args::_direction is empty");
    return *_direction;
}

void GameStateArgs::setDirection(Direction9 direction)
{
    _direction = direction;
}

auto GameStateArgs::getDeathReason() const -> DeathReason
{
    BN_ASSERT(_deathReason.has_value(), "Args::_deathReason is empty");
    return *_deathReason;
}

void GameStateArgs::setDeathReason(DeathReason deathReason)
{
    _deathReason = deathReason;
}

auto GameStateArgs::getItemAction() const -> ItemAction
{
    BN_ASSERT(_itemAction.has_value(), "Args::_itemAction is empty");
    return *_itemAction;
}

void GameStateArgs::setItemAction(ItemAction itemAction)
{
    _itemAction = itemAction;
}

} // namespace mp::game::state
