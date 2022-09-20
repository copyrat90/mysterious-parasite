/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "typedefs.hpp"

namespace mp::game
{

/**
 * @brief MetaTileset Kind.
 * DO NOT change order, as it is used for getting the right tileset.
 */
enum class MetaTilesetKind : u8
{
    // This order should be same for the `_metaTilesets[]` on the `MetaTileset.cpp`.
    DUNGEON,
    SHADOW,
    TOTAL_TILESETS
};

} // namespace mp::game
