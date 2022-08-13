/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "scene/IScene.hpp"

#include "game/DungeonFloor.hpp"
#include "game/MiniMap.hpp"

namespace iso_bn
{
class random;
}

namespace mp::scene
{

class Game final : public IScene
{
public:
    Game(iso_bn::random& rng);

    [[nodiscard]] bn::optional<SceneType> update() final;

private:
    game::MiniMap _miniMap;
    game::DungeonFloor _dungeonFloor;

    iso_bn::random& _rng;

private:
    s32 _testCounter = 0;
};

} // namespace mp::scene
