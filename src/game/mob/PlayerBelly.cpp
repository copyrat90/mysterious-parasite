/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/mob/PlayerBelly.hpp"

#include "bn_algorithm.h"
#include "bn_log.h"

namespace mp::game::mob
{

PlayerBelly::PlayerBelly(s32 currentBelly, s32 maxBelly, s32 bellyDecreaseTurns, Hud& hud)
    : _hud(hud), _maxBelly(maxBelly), _bellyDecreaseTurns(bellyDecreaseTurns), _currentBelly(currentBelly),
      _bellyDecreaseCounter(0)
{
    if (_clampBelly())
        BN_LOG("currentBelly(", currentBelly, ") was not in [0..maxBelly(", maxBelly, ")]. clamped to ", _currentBelly);
}

[[nodiscard]] bool PlayerBelly::progressTurn()
{
    if (++_bellyDecreaseCounter == _bellyDecreaseTurns)
    {
        resetBellyDecreaseCounter();
        addBelly(-1);
    }
    return !isStarveToDeath();
}

bool PlayerBelly::isStarveToDeath() const
{
    return _currentBelly <= 0;
}

s32 PlayerBelly::getMaxBelly() const
{
    return _maxBelly;
}

void PlayerBelly::setMaxBelly(s32 belly)
{
    _maxBelly = belly;
    // TODO: update hud
}

s32 PlayerBelly::getBellyDecreaseTurns() const
{
    return _bellyDecreaseTurns;
}

void PlayerBelly::setBellyDecreaseTurns(s32 turn)
{
    _bellyDecreaseTurns = turn;
    resetBellyDecreaseCounter();
}

s32 PlayerBelly::getBelly() const
{
    return _currentBelly;
}

void PlayerBelly::addBelly(s32 amount)
{
    _currentBelly += amount;
    _clampBelly();
    // TODO: update hud
}

void PlayerBelly::setBelly(s32 belly)
{
    _currentBelly = belly;
    if (_clampBelly())
        BN_LOG("belly(", belly, ") was not in [0..maxBelly(", _maxBelly, ")]. clamped to ", _currentBelly);
    // TODO: update hud
}

void PlayerBelly::resetBellyDecreaseCounter()
{
    _bellyDecreaseCounter = 0;
}

bool PlayerBelly::_clampBelly()
{
    const auto prevBelly = _currentBelly;
    _currentBelly = bn::clamp(_currentBelly, 0, _maxBelly);
    return _currentBelly != prevBelly;
}

} // namespace mp::game::mob
