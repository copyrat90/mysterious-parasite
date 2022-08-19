/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "typedefs.hpp"

namespace mp::game
{

struct BoardPos
{
    s8 x = 0, y = 0;

    BoardPos operator+(const BoardPos& other) const;
    BoardPos operator-(const BoardPos& other) const;
    BoardPos operator-() const;
    BoardPos operator*(s8 multiply) const;
    BoardPos& operator+=(const BoardPos& other);
    BoardPos& operator-=(const BoardPos& other);
    bool operator==(const BoardPos& other) const;
};

} // namespace mp::game
