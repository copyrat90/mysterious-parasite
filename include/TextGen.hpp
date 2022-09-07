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

namespace mp
{

class TextGen
{
public:
    using Alignment = bn::sprite_text_generator::alignment_type;

    enum FontKind
    {
        GALMURI_7,
        GALMURI_9,
        // total fonts count
        TOTAL_FONTS
    };

public:
    TextGen();

    void gen(bn::fixed x, bn::fixed y, Alignment, FontKind, const bn::string_view& text,
             bn::ivector<bn::sprite_ptr>& outSprites);
    void gen(const bn::fixed_point& position, Alignment, FontKind, const bn::string_view& text,
             bn::ivector<bn::sprite_ptr>& outSprites);

private:
    bn::array<bn::sprite_text_generator, TOTAL_FONTS> _textGens;
};

} // namespace mp
