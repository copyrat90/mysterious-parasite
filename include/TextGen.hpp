/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_array.h"
#include "bn_sprite_text_generator.h"
#include "bn_utility.h"

#include "typedefs.hpp"

namespace mp
{

class TextGen
{
public:
    enum FontKind
    {
        GALMURI_7,
        GALMURI_9,
        // total fonts count
        TOTAL_FONTS
    };

public:
    TextGen();

    auto get(FontKind) -> bn::sprite_text_generator&;

private:
    bn::array<bn::sprite_text_generator, TOTAL_FONTS> _textGens;
};

} // namespace mp
