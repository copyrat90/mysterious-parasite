/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/BoardPos.hpp"

namespace mp::game
{

auto BoardPos::operator+(const BoardPos& other) const -> BoardPos
{
    return BoardPos{(s8)(x + other.x), (s8)(y + other.y)};
}

auto BoardPos::operator-(const BoardPos& other) const -> BoardPos
{
    return BoardPos{(s8)(x - other.x), (s8)(y - other.y)};
}

auto BoardPos::operator-() const -> BoardPos
{
    return BoardPos{(s8)-x, (s8)-y};
}

auto BoardPos::operator*(s8 multiply) const -> BoardPos
{
    return BoardPos{(s8)(x * multiply), (s8)(y * multiply)};
}

auto BoardPos::operator+=(const BoardPos& other) -> BoardPos&
{
    x += other.x;
    y += other.y;
    return *this;
}

auto BoardPos::operator-=(const BoardPos& other) -> BoardPos&
{
    x -= other.x;
    y -= other.y;
    return *this;
}

bool BoardPos::operator==(const BoardPos& other) const
{
    return x == other.x && y == other.y;
}

} // namespace mp::game
