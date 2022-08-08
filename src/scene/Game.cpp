#include "scene/Game.hpp"

#include "bn_keypad.h"

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
    // test

    _miniMap.update();

    return bn::nullopt;
}

} // namespace mp::scene
