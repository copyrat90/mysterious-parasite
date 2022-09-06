/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "typedefs.hpp"

namespace mp
{
class TextGen;
}

namespace mp::game
{

class Hud final
{
public:
    Hud(TextGen&);

    bool isVisible() const;
    void setVisible(bool);

    void setBelly(s32 currentBelly, s32 maxBelly);

private:
    void _initGraphics();

private:
    TextGen& _textGen;

    s32 _currentBelly;
    s32 _maxBelly;
    s32 _bellyGaugeGraphicsIndex;

    bn::vector<bn::sprite_ptr, 1> _currentBellySprite;
    bn::vector<bn::sprite_ptr, 2> _bellyMiscSprites;
    bn::vector<bn::sprite_ptr, 1> _maxBellySprite;
    bn::sprite_ptr _bellyGaugeSprite;
};

} // namespace mp::game
