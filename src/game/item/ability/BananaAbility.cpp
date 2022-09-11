/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/item/ability/BananaAbility.hpp"

#include "iso_bn_random.h"

#include "game/Dungeon.hpp"
#include "game/mob/Player.hpp"

namespace mp::game::item::ability
{

void BananaAbility::use(mob::Player& user, ItemUse& itemUse, Dungeon& dungeon, iso_bn::random& rng) const
{
    user.getBelly().addCurrentBelly(50);
    // TODO: Call itemUse's start animation
}

} // namespace mp::game::item::ability
