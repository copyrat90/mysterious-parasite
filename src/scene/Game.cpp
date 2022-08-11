/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "scene/Game.hpp"

#include "bn_keypad.h"
#include "bn_profiler.h"

namespace mp::scene
{

Game::Game(iso_bn::random& rng) : _rng(rng)
{
    _miniMap.setVisible(true);
}

bn::optional<SceneType> Game::update()
{
    // test
    if (bn::keypad::select_pressed())
    {
        _dungeonFloor.generate(_rng);
        _miniMap.redrawAll(_dungeonFloor);
    }
    if (bn::keypad::start_pressed())
    {
        bn::profiler::show();
    }
    // test

    _miniMap.update();

    return bn::nullopt;
}

} // namespace mp::scene
