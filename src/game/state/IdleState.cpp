/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/state/IdleState.hpp"

#include "bn_keypad.h"

#include "Settings.hpp"
#include "game/Dungeon.hpp"
#include "game/item/ItemInfo.hpp"
#include "game/item/ability/ItemAbility.hpp"

namespace mp::game::state
{

IdleState::IdleState(Dungeon& dungeon) : GameState(dungeon)
{
#ifdef MP_DEBUG
    _testMapGen();
#endif
}

auto IdleState::handleInput() -> bn::optional<GameStateArgs>
{
#ifdef MP_DEBUG
    if (bn::keypad::select_held() && bn::keypad::l_pressed())
        _testMapGen();
    // TODO: Move mini-map to seperate screen
    // if (bn::keypad::select_held() && bn::keypad::r_pressed())
    //     _miniMap.setVisible(!_miniMap.isVisible());
    if (bn::keypad::select_held() && bn::keypad::right_pressed())
    {
        auto& settings = _dungeon.getSettings();
        settings.setLang((settings.getLang() == Settings::ENGLISH) ? Settings::KOREAN : Settings::ENGLISH);
    }
#endif

    auto nextState = _handleMovement();
    if (nextState.has_value() && nextState->shouldChangeState())
        return nextState;

    nextState = _handleItemUse();
    if (nextState.has_value() && nextState->shouldChangeState())
        return nextState;

    nextState = _handleItemToss();
    if (nextState.has_value() && nextState->shouldChangeState())
        return nextState;

    return bn::nullopt;
}

auto IdleState::update() -> bn::optional<GameStateArgs>
{
    // TODO: Change to GAME_OVER if game over check == true
    return bn::nullopt;
}

void IdleState::onEnter(const GameStateArgs& args)
{
    _gameOverReason = bn::nullopt;

    auto& playerBelly = _dungeon.getPlayer().getBelly();
    playerBelly.progressTurn();
    if (playerBelly.isStarveToDeath())
        _gameOverReason = GameStateArgs::DeathReason::STARVE;

    // TODO: Check game over on spotted by enemy
}

auto IdleState::_handleMovement() -> bn::optional<GameStateArgs>
{
    if (bn::keypad::left_held() || bn::keypad::right_held() || bn::keypad::up_held() || bn::keypad::down_held())
    {
        using ActionType = mob::MonsterAction::Type;
        using Dir9 = Direction9;

        const Dir9 inputDirection = getDirectionFromKeyHold();

        // ignore diagonal inputs, as diagonal movements are not allowed in this game.
        if (inputDirection == Dir9::UP || inputDirection == Dir9::DOWN || inputDirection == Dir9::LEFT ||
            inputDirection == Dir9::RIGHT)
        {
            auto& player = _dungeon.getPlayer();

            const auto moveDiff = convertDir9ToPos(inputDirection);
            const auto candidatePlayerPos = player.getBoardPos() + moveDiff;

            // if player can move to the input direction, move to there.
            if (_dungeon.canMoveTo(player, candidatePlayerPos))
            {
                // change to `MOVING` state
                GameStateArgs result(GameStateKind::IDLE, GameStateKind::MOVING);
                result.setDirection(inputDirection);
                return result;
            }
            // if not, just change the player's direction without moving.
            else
            {
                player.actPlayer(mob::MonsterAction(inputDirection, ActionType::CHANGE_DIRECTION));
            }
        }
    }

    return bn::nullopt;
}

auto IdleState::_handleItemUse() -> bn::optional<GameStateArgs>
{
    if (bn::keypad::l_held() && bn::keypad::a_pressed())
    {
        auto& itemUse = _dungeon.getItemUse();
        if (itemUse.getInventoryItem().has_value())
        {
            item::Item& item = itemUse.getInventoryItem().value();
            const item::ItemInfo& itemInfo = item.getItemInfo();
            if (itemInfo.canBeUsed)
            {
                // change to `PLAYER_ACT` state (item use)
                GameStateArgs result(GameStateKind::IDLE, GameStateKind::PLAYER_ACT);
                result.setItemAction(GameStateArgs::ItemAction::USE);
                return result;
            }
        }
    }
    return bn::nullopt;
}

auto IdleState::_handleItemToss() -> bn::optional<GameStateArgs>
{
    if (bn::keypad::l_held() && bn::keypad::r_pressed())
    {
        BN_ERROR("TODO: Add item toss");
    }
    return bn::nullopt;
}

#ifdef MP_DEBUG
void IdleState::_testMapGen()
{
    auto& rng = _dungeon.getRng();
    auto& player = _dungeon.getPlayer();
    auto& floor = _dungeon.getFloor();
    auto& bg = _dungeon.getDungeonBg();
    auto& items = _dungeon.getItems();
    auto& camera = _dungeon.getCamera();

    player.setBoardPos(DungeonFloor::COLUMNS / 2, DungeonFloor::ROWS / 2);
    // miniMap.updateBgPos(_player);

    floor.generate(rng);
    bg.redrawAll(floor, player);
    // miniMap.redrawAll(floor);

    items.clear();
    items.emplace_front(item::ItemKind::BANANA, player.getBoardPos() + BoardPos{0, -2}, player, camera);
}
#endif

} // namespace mp::game::state
