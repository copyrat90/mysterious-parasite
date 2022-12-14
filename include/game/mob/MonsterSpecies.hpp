/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "typedefs.hpp"

namespace mp::game::mob
{

/**
 * @brief Monster species.
 * DO NOT change order, as indices are used for querying monster infos.
 */
enum MonsterSpecies : u8
{
    PLAYER = 0,

    LEMMAS,

    // total species count
    TOTAL_SPECIES
};

} // namespace mp::game::mob
