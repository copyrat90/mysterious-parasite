/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_array.h"
#include "bn_assert.h"
#include "bn_string_view.h"

#include "Settings.hpp"
#include "texts.hpp"

namespace bn
{
class sprite_item;
}
namespace mp::game::item::ability
{
class ItemAbility;
}

namespace mp::game::item
{

enum ItemKind : u8;

struct ItemInfo final
{
public:
    static auto fromKind(ItemKind) -> const ItemInfo&;

public:
    static constexpr s32 MAX_TOSS_DISTANCE = 4;

public:
    ItemKind kind;
    const bool canBeUsed;
    const s32 tossDistance;
    const bn::sprite_item& spriteItem;
    const s32 graphicsIndex;
    const ability::ItemAbility& ability;
    const texts::LangTexts& name;
    const texts::LangTexts& description;

public:
    constexpr ItemInfo(ItemKind kind_, bool canBeUsed_, s32 tossDistance_, const bn::sprite_item& spriteItem_,
                       s32 graphicsIndex_, const ability::ItemAbility& ability_, const texts::LangTexts& name_,
                       const texts::LangTexts& description_)
        : kind(kind_), canBeUsed(canBeUsed_), tossDistance(tossDistance_), spriteItem(spriteItem_),
          graphicsIndex(graphicsIndex_), ability(ability_), name(name_), description(description_)
    {
        BN_ASSERT(tossDistance <= 4, "tossDistance(", tossDistance, ") should be less than ", MAX_TOSS_DISTANCE + 1,
                  " to be seen on screen");
    }
};

} // namespace mp::game::item
