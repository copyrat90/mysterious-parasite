/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

/*
 * Every in-game texts should reside here,
 * so that `gba-free-font` generator can see the texts, and generate the necessary glyphs only.
 */

#pragma once

#include "bn_string_view.h"

namespace mp::texts
{

inline constexpr bn::string_view KOR_TEST_STR = "다람쥐 헌 쳇바퀴에 타고파.";
inline constexpr bn::string_view ENG_TEST_STR = "The quick brown fox jumps over a lazy dog.";

} // namespace mp::texts
