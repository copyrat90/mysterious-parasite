/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

namespace mp::game
{

class DungeonFloor;

class DungeonBg final
{
public:
    void redrawAll(const DungeonFloor&);
};

} // namespace mp::game
