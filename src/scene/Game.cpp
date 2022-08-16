/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "scene/Game.hpp"

#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_profiler.h"
#include "bn_sprite_text_generator.h"

#include "texts.hpp"

namespace mp::scene
{

Game::Game(iso_bn::random& rng, bn::sprite_text_generator& textGen) : _rng(rng)
{
    _miniMap.setVisible(true);
}

bn::optional<SceneType> Game::update()
{
#ifdef MP_DEBUG
    if (_testCounter++ == 100)
    {
        bn::profiler::show();
    }
    else
    {
        _dungeonFloor.generate(_rng);
        _miniMap.redrawAll(_dungeonFloor);
        BN_LOG("Generated dungeon #", _testCounter);
    }
#endif

    _miniMap.update();

    return bn::nullopt;
}

} // namespace mp::scene
