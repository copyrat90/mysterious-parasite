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
#include "bn_utility.h"

#include "Settings.hpp"
#include "game/item/ItemKind.hpp"

namespace mp::texts
{

using LangTexts = bn::array<bn::string_view, Settings::TOTAL_LANGUAGES>;

inline constexpr LangTexts BELLY = {"Belly", "만복도"};

inline constexpr LangTexts ITEM_USE_HINT = {"L+A: Use", "L+A: 사용"};
inline constexpr LangTexts ITEM_TOSS_HINT = {"L+R: Throw", "L+R: 던지기"};

// for use in ItemInfo class.
inline constexpr bn::array<bn::pair<LangTexts, LangTexts>, game::item::TOTAL_ITEMS> ITEM_NAME_AND_DESC = {
    bn::pair{
        LangTexts{"Banana", "바나나"},
        LangTexts{"Sweet fruit. Recovers belly by 50.", "달콤한 과일. 만복도를 50 회복한다."},
    },
    bn::pair{
        LangTexts{"Banana Peel", "바나나 껍질"},
        LangTexts{"What's left after eating a banana. If you throw it to the floor, an enemy may slip.",
                  "바나나를 먹고 남은 것. 바닥에 던지면, 적이 미끄러질지도."},
    },
};

} // namespace mp::texts
