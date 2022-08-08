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
};

} // namespace mp::scene
