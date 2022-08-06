#include "bn_assert.h"
#include "bn_core.h"
#include "bn_optional.h"
#include "bn_unique_ptr.h"

#include "scene/Game.hpp"

using namespace mp;

int main()
{
    bn::core::init();

    bn::unique_ptr<scene::IScene> scene;
    bn::optional<scene::SceneType> nextScene;

    // TEST: Game scene
    nextScene = scene::SceneType::GAME;

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
                    scene.reset(new scene::Game);
                    break;
                default:
                    BN_ERROR("Unknown nextScene: ", (s32)*nextScene);
                }
            }
        }

        bn::core::update();
    }
}
