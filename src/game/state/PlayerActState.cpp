/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/state/PlayerActState.hpp"

#include "game/Dungeon.hpp"
#include "game/item/ItemInfo.hpp"
#include "game/item/ability/ItemAbility.hpp"

namespace mp::game::state
{

PlayerActState::PlayerActState(Dungeon& dungeon) : GameState(dungeon)
{
}

auto PlayerActState::update() -> bn::optional<GameStateArgs>
{
    // TODO: update player item use/toss animation,
    //       rather then immediate transition to `MOVING`.
    GameStateArgs result(GameStateKind::PLAYER_ACT, GameStateKind::MOVING);
    result.setDirection(Direction9::NONE);
    return result;
}

void PlayerActState::onEnter(const GameStateArgs& args)
{
    switch (args.getItemAction())
    {
        using ItemAction = GameStateArgs::ItemAction;
    case ItemAction::USE:
        _startItemUse();
        break;
    case ItemAction::TOSS:
        _startItemToss();
        break;
    }
}

void PlayerActState::onExit()
{
}

void PlayerActState::_startItemUse()
{
    auto& player = _dungeon.getPlayer();
    auto& itemUse = _dungeon.getItemUse();
    auto& rng = _dungeon.getRng();

    // TODO: Set player item use animation

    // Use item ability
    if (itemUse.getInventoryItem().has_value())
    {
        item::Item& item = itemUse.getInventoryItem().value();
        const item::ItemInfo& itemInfo = item.getItemInfo();

        itemInfo.ability.use(player, itemUse, _dungeon, rng);
    }
}

void PlayerActState::_startItemToss()
{
    BN_ERROR("TODO: Add item toss");

    // TODO: Set player item toss animation

    // TODO: Add item toss effect
}

} // namespace mp::game::state
