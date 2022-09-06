/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "scene/Game.hpp"

#include "texts.hpp"

namespace mp::scene
{

Game::Game(iso_bn::random& rng, TextGen& textGen) : _rng(rng), _dungeon(rng, textGen)
{
}

bn::optional<SceneType> Game::update()
{
    auto nextScene = _dungeon.update();

    return nextScene;
}

} // namespace mp::scene
