/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/mob/MonsterInfo.hpp"

#include "bn_assert.h"

#include "game/mob/MonsterSpecies.hpp"
#include "game/mob/ai/AI.hpp"
#include "typedefs.hpp"

#include "bn_sprite_items_spr_lemmas.h"

namespace mp::game::mob
{

namespace
{

constexpr s32 TOTAL_SPECIES = (s32)MonsterSpecies::TOTAL_SPECIES;

constexpr MonsterInfo _monsterInfos[TOTAL_SPECIES] = {
    MonsterInfo(MonsterSpecies::LEMMAS, bn::sprite_items::spr_lemmas),
};

} // namespace

auto MonsterInfo::fromSpecies(MonsterSpecies species) -> const MonsterInfo&
{
    BN_ASSERT(0 <= (s32)species && (s32)species < TOTAL_SPECIES, "species(", (s32)species, ") OOB");
    return _monsterInfos[(s32)species];
}

} // namespace mp::game::mob
