#pragma once

#include "scene/IScene.hpp"

#include "game/MiniMap.hpp"

namespace mp::scene
{

class Game final : public IScene
{
public:
    Game();

    [[nodiscard]] bn::optional<SceneType> update() final;

private:
    game::MiniMap _miniMap;
};

} // namespace mp::scene
