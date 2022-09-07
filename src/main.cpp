/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "bn_assert.h"
#include "bn_bg_palettes.h"
#include "bn_core.h"
#include "bn_optional.h"
#include "bn_unique_ptr.h"

#include "iso_bn_random.h"

#include "TextGen.hpp"
#include "debug/DebugView.hpp"
#include "scene/Game.hpp"

using namespace mp;

int main()
{
    bn::core::init();
    // TEST
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    bn::unique_ptr<scene::IScene> scene;
    bn::optional<scene::SceneType> nextScene;

    TextGen textGen;

    // TODO: 세이브로부터 seed 불러오기
    iso_bn::random rng;

    // TEST: Game scene
    nextScene = scene::SceneType::GAME;

#ifdef MP_DEBUG
    debug::DebugView debugView(textGen);
#endif

    while (true)
    {
        if (scene)
            nextScene = scene->update();

        if (nextScene)
        {
            if (scene)
                scene.reset();
            else
            {
                switch (*nextScene)
                {
                    using SceneType = scene::SceneType;
                case SceneType::GAME:
                    scene.reset(new scene::Game(rng, textGen));
                    break;
                default:
                    BN_ERROR("Unknown nextScene: ", (s32)*nextScene);
                }
            }
        }
#ifdef MP_DEBUG
        debugView.update();
#endif
        bn::core::update();
    }
}
