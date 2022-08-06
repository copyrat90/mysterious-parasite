#include "scene/Game.hpp"

#include "bn_keypad.h"

namespace mp::scene
{

Game::Game()
{
}

bn::optional<SceneType> Game::update()
{
    // test
    if (bn::keypad::select_pressed())
        _miniMap.setVisible(!_miniMap.isVisible());
    // test

    _miniMap.update();

    return bn::nullopt;
}

} // namespace mp::scene
