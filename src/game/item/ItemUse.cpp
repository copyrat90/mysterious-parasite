/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/item/ItemUse.hpp"

#include "game/Hud.hpp"

namespace mp::game::item
{

ItemUse::ItemUse(Hud& hud) : _hud(hud)
{
}

bool ItemUse::isOngoing() const
{
    // TODO: Return `true` when item use/toss animation is ongoing
    return false;
}

bool ItemUse::hasInventoryItem() const
{
    return _inventoryItem.has_value();
}

auto ItemUse::getInventoryItem() -> bn::optional<Item>&
{
    return _inventoryItem;
}

auto ItemUse::getInventoryItem() const -> const bn::optional<Item>&
{
    return _inventoryItem;
}

void ItemUse::setInventoryItem(Item&& item)
{
    _hud.setInventory(item.getItemInfo());
    _inventoryItem.emplace(bn::move(item));
}

} // namespace mp::game::item
