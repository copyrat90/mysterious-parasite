/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "typedefs.hpp"

namespace mp::game
{
class Hud;
}

namespace mp::game::mob
{

class PlayerBelly final
{
public:
    PlayerBelly(s32 currentBelly, s32 maxBelly, s32 bellyDecreaseTurns, Hud&);

    /**
     * @brief Progress a turn for the belly.
     * Decrease belly amount if `_bellyDecreaseTurns` has passed.
     *
     * @return `false` if player starves to death, otherwise `true`.
     */
    [[nodiscard]] bool progressTurn();

    bool isStarveToDeath() const;

    s32 getMaxBelly() const;
    void setMaxBelly(s32 belly);

    s32 getBellyDecreaseTurns() const;
    void setBellyDecreaseTurns(s32 turn);

    s32 getBelly() const;
    void addBelly(s32 amount);
    void setBelly(s32 belly);

    void resetBellyDecreaseCounter();

private:
    /**
     * @brief Clamps `_currentBelly` to [0.. `_maxBelly` ]
     *
     * @return `true` if clamp happened.
     */
    bool _clampBelly();

private:
    Hud& _hud;

    // rarely changed values
    s32 _maxBelly;
    s32 _bellyDecreaseTurns;

    // frequently changed values
    s32 _currentBelly;
    s32 _bellyDecreaseCounter;
};

} // namespace mp::game::mob
