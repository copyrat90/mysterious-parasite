/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "game/item/ItemKind.hpp"

namespace iso_bn
{
class random;
}
namespace mp::game
{
class Dungeon;
}
namespace mp::game::item
{
class ItemUse;
}
namespace mp::game::mob
{
class Monster;
class Player;
} // namespace mp::game::mob

namespace mp::game::item::ability
{

class ItemAbility
{
public:
    constexpr virtual ~ItemAbility(){};

    virtual ItemKind getItemKind() const = 0;

    /**
     * @brief This command fires when the player uses the item.
     * (e.g. Consume food, read scroll, etc.)
     */
    virtual void use(mob::Player& user, ItemUse&, Dungeon&, iso_bn::random&) const;

    /**
     * @brief This command fires when the monster steps on the item.
     */
    virtual void stepOn(mob::Monster& mob, ItemUse&, Dungeon&, iso_bn::random&) const;
};

} // namespace mp::game::item::ability
