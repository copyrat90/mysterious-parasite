/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include <cstdint>

using s8 = int8_t;
using s16 = int16_t;
using s32 = int; // `int32_t` is `long`, and this results in ambiguous implicit conversion errors
using s64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = unsigned;
using u64 = uint64_t;
