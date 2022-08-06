#pragma once

#include "bn_optional.h"

#include "scene/SceneType.hpp"

namespace mp::scene
{

class IScene
{
public:
    virtual ~IScene() = default;

    [[nodiscard]] virtual bn::optional<SceneType> update() = 0;
};

} // namespace mp::scene
