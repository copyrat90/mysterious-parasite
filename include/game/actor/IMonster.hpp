/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "typedefs.hpp"

namespace mp::game::actor
{

class IMonster
{
public:
    virtual ~IMonster() = default;

    /**
     * @brief Frame update, mostly for animations.
     */
    virtual void update() = 0;

    /**
     * @brief Take a turn, based on their AI.
     */
    virtual void actAI() = 0;
};

} // namespace mp::game::actor
