/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/item/ItemInfo.hpp"

#include "game/item/ItemKind.hpp"

#include "game/item/ability/BananaAbility.hpp"
#include "game/item/ability/BananaPeelAbility.hpp"

#include "bn_sprite_items_spr_banana.h"

namespace mp::game::item
{

namespace
{

constexpr ability::BananaAbility _bananaAbility;
constexpr ability::BananaPeelAbility _bananaPeelAbility;

constexpr ItemInfo _itemInfos[ItemKind::TOTAL_ITEMS] = {
    ItemInfo(ItemKind::BANANA, true, 3, bn::sprite_items::spr_banana, 0, _bananaAbility,
             texts::ITEM_NAME_AND_DESC[0].first, texts::ITEM_NAME_AND_DESC[0].second),
    ItemInfo(ItemKind::BANANA_PEEL, false, 4, bn::sprite_items::spr_banana, 1, _bananaPeelAbility,
             texts::ITEM_NAME_AND_DESC[0].first, texts::ITEM_NAME_AND_DESC[0].second),
};

} // namespace

auto ItemInfo::fromKind(ItemKind itemKind) -> const ItemInfo&
{
    BN_ASSERT(0 <= itemKind && itemKind < ItemKind::TOTAL_ITEMS, "Invalid itemKind(", itemKind, ")");

    return _itemInfos[itemKind];
}

} // namespace mp::game::item
