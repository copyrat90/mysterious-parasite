/*
 * SPDX-FileCopyrightText: Copyright (c) 2017 AtTheMatinee
 * Copyright (c) 2022 Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *
 * See `licenses/dungeon-generation.txt` file for details.
 */

#pragma once

#include "bn_array.h"

#include "iso_bn_random.h"

#include "game/DungeonFloor.hpp"

namespace mp::game
{

class DungeonGenerator final
{
public:
    static constexpr s32 ROWS = DungeonFloor::ROWS;
    static constexpr s32 COLUMNS = DungeonFloor::COLUMNS;

    /**
     * @brief Generate random dungeon floor by Room Addition Algorithm.
     * See https://github.com/AtTheMatinee/dungeon-generation for details.
     * See `licenses/dungeon-generation.txt` file for the license info.
     */
    void generateByRoomAddition(bn::array<bn::array<DungeonFloor::Type, COLUMNS>, ROWS>& board, iso_bn::random& rng);
};

} // namespace mp::game
