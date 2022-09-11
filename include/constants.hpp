/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_display.h"
#include "bn_fixed_point.h"
#include "bn_size.h"

#include "typedefs.hpp"

namespace mp::consts
{

inline constexpr s32 ACTOR_MOVE_FRAMES = 8;

constexpr bn::fixed_point INIT_CAM_POS = {-8, -8};
constexpr bn::fixed_point INVENTORY_POS = {16 - bn::display::width() / 2, 144 - bn::display::height() / 2};

inline constexpr bn::size DUNGEON_FLOOR_SIZE = {64, 64};
inline constexpr bn::size DUNGEON_META_TILE_SIZE = {16, 16};
inline constexpr s32 DUNGEON_FLOOR_CELLS_COUNT = DUNGEON_FLOOR_SIZE.width() * DUNGEON_FLOOR_SIZE.height();

inline constexpr s32 DUNGEON_ITEM_MAX_COUNT = 30;
inline constexpr s32 DUNGEON_MOB_MAX_COUNT = 30;

inline constexpr s32 MOB_ITEM_MAX_COUNT = 8;
inline constexpr s32 MOB_ANIM_MAX_KEYFRAMES = 2;
inline constexpr s32 MOB_ANIM_WAIT_UPDATE = 20;

inline constexpr s32 DUNGEON_BG_PRIORITY = 3;
inline constexpr s32 MINI_MAP_BG_PRIORITY = 1;
inline constexpr s32 UI_BG_PRIORITY = 0;

inline constexpr s32 DUNGEON_BG_Z_ORDER_WALLS = 50;
inline constexpr s32 DUNGEON_BG_Z_ORDER_SHADOWS = 40;

inline constexpr s32 MOB_Z_ORDER = 40;
inline constexpr s32 ITEM_Z_ORDER = 50;
inline constexpr s32 INVENTORY_Z_ORDER = 60;

} // namespace mp::consts
