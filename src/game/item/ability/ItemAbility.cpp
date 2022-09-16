/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/item/ability/ItemAbility.hpp"

#include "bn_assert.h"

#include "Settings.hpp"
#include "game/item/ItemInfo.hpp"

namespace mp::game::item::ability
{

void ItemAbility::use(mob::Player& user, ItemUse& itemUse, Dungeon& dungeon, iso_bn::random& rng) const
{
    BN_ERROR("{", ItemInfo::fromKind(getItemKind()).name[Settings::ENGLISH], "}::use() not overrided!");
}

void ItemAbility::stepOn(mob::Monster& mob, ItemUse& itemUse, Dungeon& dungeon, iso_bn::random& rng) const
{
    // default: Nothing happens.
}

} // namespace mp::game::item::ability
