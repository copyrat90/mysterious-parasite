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

class BananaAbility final : public ItemAbility
{
public:
    constexpr ~BananaAbility(){};

    ItemKind getItemKind() const override
    {
        return ItemKind::BANANA;
    }

    void use(mob::Player& user, ItemUse&, Dungeon&, iso_bn::random&) const override;
};

} // namespace mp::game::item::ability
