/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "typedefs.hpp"

namespace mp::game::item
{

enum ItemKind : u8
{
    BANANA = 0,
    BANANA_PEEL,

    // total items count
    TOTAL_ITEMS
};

} // namespace mp::game::item
