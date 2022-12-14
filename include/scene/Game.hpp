/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "scene/IScene.hpp"

#include "game/Dungeon.hpp"

namespace mp
{
class TextGen;
}

namespace iso_bn
{
class random;
}

namespace mp::scene
{

class Game final : public IScene
{
public:
    Game(iso_bn::random& rng, TextGen&, Settings&);

    [[nodiscard]] bn::optional<SceneType> update() final;

private:
    iso_bn::random& _rng;
    game::Dungeon _dungeon;
};

} // namespace mp::scene
