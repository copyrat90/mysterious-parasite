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
 *
 * Only exception is the Latin charset from '!'(U+0021) to '~'(U+007E).
 */

#pragma once

#include "bn_array.h"
#include "bn_string_view.h"

#include "Settings.hpp"

namespace mp::texts
{

using LangTexts = bn::array<bn::string_view, Settings::TOTAL_LANGUAGES>;

inline constexpr LangTexts BELLY = {"Belly", "만복도"};

} // namespace mp::texts
