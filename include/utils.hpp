/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_assert.h"

#include "typedefs.hpp"

namespace mp::utils
{

constexpr s32 upperTwoPowOf(s32 num)
{
    BN_ASSERT(0 <= num && num <= (1 << 30));

    s32 result = 1;
    while (result < num)
        result <<= 1;
    return result;
}

constexpr s32 upperEightPowOf(s32 num)
{
    BN_ASSERT(0 <= num && num <= (1 << 27));

    s32 result = 1;
    while (result < num)
        result <<= 3;
    return result;
}

} // namespace mp::utils
