/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/Dungeon.hpp"

#include "bn_keypad.h"
#include "iso_bn_random.h"

#include "game/mob/MonsterSpecies.hpp"

#ifdef MP_DEBUG
#include "game/mob/MonsterAction.hpp"
#endif

namespace mp::game
{

Dungeon::Dungeon(iso_bn::random& rng) : _rng(rng), _player(mob::MonsterSpecies::LEMMAS, {0, 0})
{
    _miniMap.updateBgPos(_player);
    _miniMap.setVisible(true);
}

auto Dungeon::update() -> bn::optional<scene::SceneType>
{
#ifdef MP_DEBUG
    if (bn::keypad::select_held() && bn::keypad::l_pressed())
    {
        _player.setBoardPos(DungeonFloor::COLUMNS / 2, DungeonFloor::ROWS / 2);
        _miniMap.updateBgPos(_player);

        _floor.generate(_rng);
        _bg.redrawAll(_floor);
        _miniMap.redrawAll(_floor);
    }

    if (bn::keypad::left_pressed() || bn::keypad::right_pressed() || bn::keypad::up_pressed() ||
        bn::keypad::down_pressed())
    {
        using Dir = mob::MonsterAction::Direction;
        using Type = mob::MonsterAction::Type;
        if (bn::keypad::left_pressed())
            _player.actPlayer(mob::MonsterAction(Dir::LEFT, Type::MOVE));
        if (bn::keypad::right_pressed())
            _player.actPlayer(mob::MonsterAction(Dir::RIGHT, Type::MOVE));
        if (bn::keypad::up_pressed())
            _player.actPlayer(mob::MonsterAction(Dir::UP, Type::MOVE));
        if (bn::keypad::down_pressed())
            _player.actPlayer(mob::MonsterAction(Dir::DOWN, Type::MOVE));

        _miniMap.updateBgPos(_player);
    }
#endif

    _miniMap.update();

    return bn::nullopt;
}

} // namespace mp::game
