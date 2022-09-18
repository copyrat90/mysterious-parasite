/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/state/MovingState.hpp"

#include "game/Dungeon.hpp"
#include "game/MetaTileset.hpp"

namespace mp::game::state
{

MovingState::MovingState(Dungeon& dungeon) : GameState(dungeon)
{
}

auto MovingState::update() -> bn::optional<GameStateArgs>
{
    using StKind = GameStateKind;
    GameStateArgs nextStateArgs(StKind::MOVING, StKind::MOVING);

    if (_camMoveAction)
        _updateBgScroll();

    if (_moveCountdown > 0)
        --_moveCountdown;

    // if moving is over, change to the next state
    if (!_camMoveAction.has_value() && _moveCountdown == 0)
    {
        // TODO
        // // if there's any enemy act on screen, transfer to the ENEMY_ACT
        // if (...)
        //     nextStateArgs.setNextStateKind(StKind::ENEMY_ACT);
        // else
        nextStateArgs.setNextStateKind(StKind::IDLE);
    }

    return nextStateArgs;
}

void MovingState::onEnter(const GameStateArgs& args)
{
    _camMoveAction.reset();

    const auto playerMoveDir = args.getDirection();
    if (playerMoveDir != Direction9::NONE)
    {
        using ActionType = mob::MonsterAction::Type;
        _dungeon.getPlayer().actPlayer(mob::MonsterAction(playerMoveDir, ActionType::MOVE));
        // _miniMap.updateBgPos(player);
    }

    // TODO: Add object push

    // if no one moves on screen, skip this moving state immediately
    if (_noOneMoves(playerMoveDir))
    {
        _moveCountdown = 0;
    }
    // if anyone moves on screen
    else
    {
        _startBgScroll(args.getDirection());
        _moveCountdown = consts::ACTOR_MOVE_FRAMES;
    }
}

void MovingState::onExit()
{
    _pickUpItem();
}

void MovingState::_startBgScroll(Direction9 moveDir)
{
    const auto moveDiff = convertDir9ToPos(moveDir);

    const auto& camera = _dungeon.getCamera();
    bn::fixed_point destination = camera.position();
    destination += {moveDiff.x * MetaTile::SIZE_IN_PIXELS.width(), moveDiff.y * MetaTile::SIZE_IN_PIXELS.height()};

    _dungeon.getDungeonBg().startBgScroll(moveDir);

    _camMoveAction = bn::camera_move_to_action(camera, consts::ACTOR_MOVE_FRAMES, destination);
}

void MovingState::_updateBgScroll()
{
    BN_ASSERT(_camMoveAction);
    if (_camMoveAction->done())
        _camMoveAction.reset();
    else
        _camMoveAction->update();
}

void MovingState::_pickUpItem()
{
    auto& itemUse = _dungeon.getItemUse();
    auto& items = _dungeon.getItems();
    auto& player = _dungeon.getPlayer();

    // the player can only pick up an item when they don't already have one.
    if (!itemUse.hasInventoryItem())
    {
        auto it = items.before_begin();
        auto cur = it;
        ++cur;
        while (cur != items.end())
        {
            cur = it;
            ++cur;
            // check if the player stepped on the cur item, and pick it up.
            if (player.getBoardPos() == cur->getBoardPos())
            {
                cur->moveSpriteToInventory();
                itemUse.setInventoryItem(bn::move(*cur));

                items.erase_after(it);
            }
            else
                ++it;
        }
    }
}

bool MovingState::_noOneMoves(Direction9 playerMoveDir) const
{
    // TODO: Check whether or not any enemy on sceen moves.
    return playerMoveDir == Direction9::NONE;
}

} // namespace mp::game::state
