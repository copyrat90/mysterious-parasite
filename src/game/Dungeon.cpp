/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/Dungeon.hpp"

#include "bn_assert.h"
#include "bn_keypad.h"
#include "iso_bn_random.h"

#include "constants.hpp"
#include "game/MetaTileset.hpp"
#include "game/item/ItemInfo.hpp"
#include "game/item/ItemKind.hpp"
#include "game/item/ability/ItemAbility.hpp"
#include "game/mob/MonsterAction.hpp"
#include "game/mob/MonsterSpecies.hpp"

namespace mp::game
{

Dungeon::Dungeon(iso_bn::random& rng, TextGen& textGen, Settings& settings)
    : _rng(rng), _settings(settings), _camera(bn::camera_ptr::create(consts::INIT_CAM_POS)), _bg(_camera),
      _hud(textGen, settings), _itemUse(_hud), _player({0, 0}, _camera, _hud)
{
#ifdef MP_DEBUG
    _testMapGen();
#endif

    _hud.setBelly(_player.getBelly().getCurrentBelly(), _player.getBelly().getMaxBelly());

    // TODO: Move mini-map to seperate screen
    _miniMap.updateBgPos(_player);
    // _miniMap.setVisible(true);

    _hud.setVisible(true);
    _player.setVisible(true);
    _bg.setVisible(true);
}

auto Dungeon::update() -> bn::optional<scene::SceneType>
{
    bool isPlayerAlive = _progressTurn();

    _player.update(*this);
    _miniMap.update();

    if (_camMoveAction)
        _updateBgScroll();
    _bg.update(_floor, _player);

    // TODO: Return the GameOver scene when player dies.
    // Or, we do the game over animation first, and return it later.
    return bn::nullopt;
}

bool Dungeon::isTurnOngoing() const
{
    return _camMoveAction.has_value() || _itemUse.isOngoing();
}

#ifdef MP_DEBUG
void Dungeon::_testMapGen()
{
    _player.setBoardPos(DungeonFloor::COLUMNS / 2, DungeonFloor::ROWS / 2);
    _miniMap.updateBgPos(_player);

    _floor.generate(_rng);
    _bg.redrawAll(_floor, _player);
    _miniMap.redrawAll(_floor);

    _items.clear();
    _items.emplace_front(item::ItemKind::BANANA, _player.getBoardPos() + BoardPos{0, -2}, _player, _camera);
}
#endif

bool Dungeon::_progressTurn()
{
    // Don't receive any input if the turn is ongoing.
    if (isTurnOngoing())
        return true;

#ifdef MP_DEBUG
    if (bn::keypad::select_held() && bn::keypad::l_pressed())
        _testMapGen();
    if (bn::keypad::select_held() && bn::keypad::r_pressed())
        _miniMap.setVisible(!_miniMap.isVisible());
    if (bn::keypad::select_held() && bn::keypad::right_pressed())
        _settings.setLang((_settings.getLang() == Settings::ENGLISH) ? Settings::KOREAN : Settings::ENGLISH);
#endif

    bool isPlayerAlive = _progressPlayerTurn();
    if (isPlayerAlive)
        isPlayerAlive = _progressMonsterTurn();

    return isPlayerAlive;
}

bool Dungeon::_progressPlayerTurn()
{
    bool isPlayerAlive = true;

    // player movement (with mini-map movement)
    if (bn::keypad::left_held() || bn::keypad::right_held() || bn::keypad::up_held() || bn::keypad::down_held())
    {
        using ActionType = mob::MonsterAction::Type;
        using Dir9 = Direction9;

        const Dir9 inputDirection = getDirectionFromKeyHold();

        // ignore diagonal inputs, as diagonal movements are not allowed in this game.
        if (inputDirection == Dir9::UP || inputDirection == Dir9::DOWN || inputDirection == Dir9::LEFT ||
            inputDirection == Dir9::RIGHT)
        {
            const auto moveDiff = convertDir9ToPos(inputDirection);
            const auto candidatePlayerPos = _player.getBoardPos() + moveDiff;

            // if player can move to the input direction, move to there.
            if (_canMoveTo(_player, candidatePlayerPos))
            {
                isPlayerAlive =
                    isPlayerAlive && _player.actPlayer(mob::MonsterAction(inputDirection, ActionType::MOVE));
                _miniMap.updateBgPos(_player);
                _startBgScroll(inputDirection);

                // the player can only pick up an item when they don't already have one.
                if (!_itemUse.hasInventoryItem())
                {
                    auto it = _items.before_begin();
                    auto cur = it;
                    ++cur;
                    while (cur != _items.end())
                    {
                        cur = it;
                        ++cur;
                        // check if the player stepped on the cur item, and pick it up.
                        if (_player.getBoardPos() == cur->getBoardPos())
                        {
                            cur->moveSpriteToInventory();
                            _itemUse.setInventoryItem(bn::move(*cur));

                            _items.erase_after(it);
                        }
                        else
                            ++it;
                    }
                }

                return isPlayerAlive;
            }
            // if not, just change the player's direction without moving.
            else
            {
                isPlayerAlive = isPlayerAlive &&
                                _player.actPlayer(mob::MonsterAction(inputDirection, ActionType::CHANGE_DIRECTION));
            }
        }
    }

    if (bn::keypad::l_held())
    {
        // item use
        if (bn::keypad::a_pressed())
        {
            if (_itemUse.getInventoryItem().has_value())
            {
                item::Item& item = _itemUse.getInventoryItem().value();
                const item::ItemInfo& itemInfo = item.getItemInfo();
                if (itemInfo.canBeUsed)
                    itemInfo.ability.use(_player, _itemUse, *this, _rng);
            }
        }

        // item toss
        if (bn::keypad::r_pressed())
        {
            BN_ERROR("TODO: Add item toss");
        }
    }
}

bool Dungeon::_progressMonsterTurn()
{
}

void Dungeon::_startBgScroll(Direction9 moveDir)
{
    const auto moveDiff = convertDir9ToPos(moveDir);

    bn::fixed_point destination = _camera.position();
    destination += {moveDiff.x * MetaTile::SIZE_IN_PIXELS.width(), moveDiff.y * MetaTile::SIZE_IN_PIXELS.height()};

    _bg.startBgScroll(moveDir);

    _camMoveAction = bn::camera_move_to_action(_camera, consts::ACTOR_MOVE_FRAMES, destination);
}

bool Dungeon::_updateBgScroll()
{
    BN_ASSERT(_camMoveAction);
    if (_camMoveAction->done())
    {
        _camMoveAction.reset();
        return true;
    }
    _camMoveAction->update();
    return false;
}

bool Dungeon::_canMoveTo(const mob::Monster& mob, const BoardPos& destination) const
{
    using FloorType = DungeonFloor::Type;
    if (_floor.getFloorTypeOf(destination) == FloorType::WALL)
        return false;
    // check diagonal adjacent wall
    if (_floor.getFloorTypeOf(destination.x, mob.getBoardPos().y) == FloorType::WALL ||
        _floor.getFloorTypeOf(mob.getBoardPos().x, destination.y) == FloorType::WALL)
        return false;

    // collide with player
    if (destination == _player.getBoardPos())
        return false;

    // TODO: Add collide with _monsters

    return true;
}

} // namespace mp::game
