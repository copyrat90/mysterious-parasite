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

inline constexpr s32 TURN_PROGRESS_DELAY = 15;

inline constexpr bn::size DUNGEON_FLOOR_SIZE = {64, 64};
inline constexpr s32 DUNGEON_FLOOR_CELLS_COUNT = DUNGEON_FLOOR_SIZE.width() * DUNGEON_FLOOR_SIZE.height();

inline constexpr s32 DUNGEON_ITEM_MAX_COUNT = 20;
inline constexpr s32 DUNGEON_MOB_MAX_COUNT = 60;

inline constexpr s32 MOB_ITEM_MAX_COUNT = 8;
inline constexpr s32 MOB_ANIM_MAX_KEYFRAMES = 2;
inline constexpr s32 MOB_ANIM_WAIT_UPDATE = 20;

inline constexpr s32 DUNGEON_BG_PRIORITY = 3;
inline constexpr s32 MINI_MAP_BG_PRIORITY = 1;
inline constexpr s32 UI_BG_PRIORITY = 0;

} // namespace mp::consts
