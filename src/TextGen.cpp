/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "TextGen.hpp"

#include "bn_assert.h"
#include "bn_fixed_point.h"

#include "galmuri7_sprite_font.h"
#include "galmuri9_sprite_font.h"

namespace mp
{

TextGen::TextGen()
    : _textGens({bn::sprite_text_generator(galmuri7_sprite_font), bn::sprite_text_generator(galmuri9_sprite_font)})
{
}

void TextGen::gen(bn::fixed x, bn::fixed y, Alignment alignment, FontKind fontKind, const bn::string_view& text,
                  bn::ivector<bn::sprite_ptr>& outSprites)
{
    BN_ASSERT(0 <= fontKind && fontKind < TOTAL_FONTS, "Invalid fontKind(", fontKind, ")");

    auto& textGen = _textGens[fontKind];
    textGen.set_alignment(alignment);
    textGen.generate(x, y, text, outSprites);
}

void TextGen::gen(const bn::fixed_point& position, Alignment alignment, FontKind fontKind, const bn::string_view& text,
                  bn::ivector<bn::sprite_ptr>& outSprites)
{
    gen(position.x(), position.y(), alignment, fontKind, text, outSprites);
}

} // namespace mp
