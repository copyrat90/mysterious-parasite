/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "scene/IScene.hpp"

#ifdef MP_DEBUG
#include "bn_sprite_ptr.h"
#include "bn_vector.h"
#endif

#include "game/DungeonFloor.hpp"
#include "game/MiniMap.hpp"

namespace bn
{
class sprite_text_generator;
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
    Game(iso_bn::random& rng, bn::sprite_text_generator& textGen);

    [[nodiscard]] bn::optional<SceneType> update() final;

private:
    game::MiniMap _miniMap;
    game::DungeonFloor _dungeonFloor;

    iso_bn::random& _rng;

#ifdef MP_DEBUG
private:
    s32 _testCounter = 0;
    bn::vector<bn::sprite_ptr, 16> _testTexts;
#endif
};

} // namespace mp::scene
