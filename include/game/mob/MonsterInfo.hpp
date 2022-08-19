/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_sprite_item.h"

namespace mp::game::mob
{

enum class MonsterSpecies;

namespace ai
{
class AI;
}

struct MonsterInfo final
{
public:
    static auto fromSpecies(MonsterSpecies) -> const MonsterInfo&;

public:
    const MonsterSpecies species;
    const bn::sprite_item& spriteItem;

public:
    constexpr MonsterInfo(MonsterSpecies species_, const bn::sprite_item& spriteItem_)
        : species(species_), spriteItem(spriteItem_)
    {
    }
};

} // namespace mp::game::mob
