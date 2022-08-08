/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_size.h"

#include "typedefs.hpp"

namespace mp::consts
{

inline constexpr bn::size DUNGEON_FLOOR_SIZE = {128, 128};
inline constexpr s32 DUNGEON_FLOOR_CELLS_COUNT = DUNGEON_FLOOR_SIZE.width() * DUNGEON_FLOOR_SIZE.height();

} // namespace mp::consts
