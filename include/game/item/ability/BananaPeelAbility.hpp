/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "game/item/ability/ItemAbility.hpp"

namespace mp::game::item::ability
{

class BananaPeelAbility final : public ItemAbility
{
public:
    constexpr ~BananaPeelAbility(){};

    ItemKind getItemKind() const override
    {
        return ItemKind::BANANA_PEEL;
    }

    void stepOn(mob::Monster& mob, ItemUse&, Dungeon&, iso_bn::random&) const override;
};

} // namespace mp::game::item::ability
