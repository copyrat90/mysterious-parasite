/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/item/Item.hpp"

#include "bn_camera_ptr.h"
#include "bn_log.h"
#include "bn_sprite_item.h"

#include "constants.hpp"
#include "game/item/ItemInfo.hpp"
#include "game/mob/Player.hpp"

namespace mp::game::item
{

Item::Item(ItemKind itemKind, const BoardPos& boardPos, const mob::Player& player, const bn::camera_ptr& camera)
    : _info(ItemInfo::fromKind(itemKind)), _playerPos(player.getBoardPos()),
      _sprite(_info.spriteItem.create_sprite(0, 0)), _pos(boardPos)
{
    _initGraphics(camera);
}

Item::Item(Item&& other)
    : _info(other._info), _playerPos(other._playerPos), _sprite(bn::move(other._sprite)), _pos(other._pos)
{
}

bool Item::isInInventory() const
{
    return !_sprite.camera().has_value();
}

void Item::moveSpriteToDungeonFloor(const BoardPos& boardPos, const bn::camera_ptr& camera)
{
    if (!isInInventory())
        BN_LOG("Item '", _info.name[0], "' is already on the dungeon floor!");

    _pos = boardPos;
    _sprite.set_camera(camera);
    _sprite.set_bg_priority(consts::DUNGEON_BG_PRIORITY);
    _updateSpritePos();
}

void Item::moveSpriteToInventory()
{
    if (isInInventory())
        BN_LOG("Item '", _info.name[0], "' is already in the inventory!");

    _sprite.remove_camera();
    _sprite.set_bg_priority(consts::UI_BG_PRIORITY);
    _updateSpritePos();
}

auto Item::getBoardPos() const -> const BoardPos&
{
    return _pos;
}

void Item::setBoardPos(const BoardPos& pos)
{
    _pos = pos;
    if (!isInInventory())
        _updateSpritePos();
}

auto Item::getItemInfo() const -> const ItemInfo&
{
    return _info;
}

void Item::_initGraphics(const bn::camera_ptr& camera)
{
    _sprite.set_camera(camera);
    _sprite.set_bg_priority(consts::DUNGEON_BG_PRIORITY);
    _sprite.set_z_order(consts::ITEM_Z_ORDER);
    _updateSpritePos();
}

void Item::_updateSpritePos()
{
    if (isInInventory())
    {
        _sprite.set_position(consts::INVENTORY_POS);
    }
    else // item is on dungeon floor
    {
        // update sprite position relative to the player.
        const BoardPos diff = _pos - _playerPos;
        auto spritePos = _sprite.camera()->position();
        spritePos += bn::fixed_point{(s32)diff.x * consts::DUNGEON_META_TILE_SIZE.width(),
                                     (s32)diff.y * consts::DUNGEON_META_TILE_SIZE.height()};
        BN_LOG("spritePos: ", spritePos.x(), ", ", spritePos.y());
        _sprite.set_position(spritePos);
    }
}

} // namespace mp::game::item
