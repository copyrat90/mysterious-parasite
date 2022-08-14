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
#include "bn_sprite_text_generator.h"

#include "texts.hpp"

namespace mp::scene
{

Game::Game(iso_bn::random& rng, bn::sprite_text_generator& textGen) : _rng(rng)
{
    _miniMap.setVisible(true);

#ifdef MP_DEBUG
    textGen.set_alignment(bn::sprite_text_generator::alignment_type::CENTER);
    textGen.generate(0, -70, texts::KOR_TEST_STR, _testTexts);
    textGen.generate(0, -55, texts::ENG_TEST_STR, _testTexts);
#endif
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
    }
#endif

    _miniMap.update();

    return bn::nullopt;
}

} // namespace mp::scene
