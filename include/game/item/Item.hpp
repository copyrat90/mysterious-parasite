/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_sprite_ptr.h"

#include "game/BoardPos.hpp"

namespace bn
{
class camera_ptr;
} // namespace bn

namespace mp::game::mob
{
class Player;
}

namespace mp::game::item
{

enum ItemKind : u8;
class ItemInfo;

/**
 * @brief Items can be picked up, used, and thrown.
 */
class Item final
{
public:
    /**
     * @brief Constructor for item on the dungeon floor.
     *
     */
    Item(ItemKind, const BoardPos&, const mob::Player&, const bn::camera_ptr&);

    /**
     * @brief Constructor for item in the inventory.
     *
     */
    Item(ItemKind, const mob::Player&);

    Item(const Item&) = delete;
    Item(Item&&);

    bool isInInventory() const;

    void moveSpriteToDungeonFloor(const BoardPos& boardPos, const bn::camera_ptr& camera);
    void moveSpriteToInventory();

    auto getBoardPos() const -> const BoardPos&;
    void setBoardPos(const BoardPos&);

    auto getItemInfo() const -> const ItemInfo&;

private:
    void _initGraphicsForInventoryItem();
    void _initGraphicsForDungeonFloorItem(const bn::camera_ptr&);
    void _updateSpritePos();

private:
    const ItemInfo& _info;
    const BoardPos& _playerPos;

    bn::sprite_ptr _sprite;
    BoardPos _pos;
};

} // namespace mp::game::item
