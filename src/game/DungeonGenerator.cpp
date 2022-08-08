/*
 * SPDX-FileCopyrightText: Copyright (c) 2017 AtTheMatinee
 * Copyright (c) 2022 Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *
 * See `licenses/dungeon-generation.txt` file for details.
 */

#include "game/DungeonGenerator.hpp"

namespace mp::game
{

/**
 * @brief Generate random dungeon floor by Room Addition Algorithm.
 * See https://github.com/AtTheMatinee/dungeon-generation for details.
 * See `licenses/dungeon-generation.txt` file for the license info.
 */
void DungeonGenerator::generateByRoomAddition(bn::array<bn::array<DungeonFloor::Type, COLUMNS>, ROWS>& board,
                                              iso_bn::random& rng)
{
    using FloorType = DungeonFloor::Type;

    // test
    for (s32 y = 0; y < ROWS; ++y)
        for (s32 x = 0; x < COLUMNS; ++x)
            board[y][x] = rng.get_int(5) ? FloorType::FLOOR : FloorType::WALL;
    // test
}

} // namespace mp::game
