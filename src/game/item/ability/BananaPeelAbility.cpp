/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/item/ability/BananaPeelAbility.hpp"

namespace mp::game::item::ability
{

void BananaPeelAbility::stepOn(mob::Monster& mob, ItemUse& itemUse, Dungeon& dungeon, iso_bn::random& rng) const
{
    // TODO: Set mob to a slipped state; unable to move for x turns.
}

} // namespace mp::game::item::ability
