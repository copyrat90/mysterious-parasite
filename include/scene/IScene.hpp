/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

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
