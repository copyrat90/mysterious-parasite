/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_optional.h"

#include "game/item/Item.hpp"

namespace mp::game
{
class Hud;
}

namespace mp::game::item
{

/**
 * @brief Manages everything related to item pickup/use/toss.
 * 1. inventory management
 * 2. use/toss animation
 * 3. choosing where to toss
 * 4. item ability applied to monsters
 */
class ItemUse final
{
public:
    ItemUse(Hud&);

    bool isOngoing() const;

    bool hasInventoryItem() const;
    auto getInventoryItem() -> bn::optional<Item>&;
    auto getInventoryItem() const -> const bn::optional<Item>&;
    void setInventoryItem(Item&& item);

private:
    Hud& _hud;

    bn::optional<Item> _inventoryItem;

    // TODO: Add item animations
};

} // namespace mp::game::item
