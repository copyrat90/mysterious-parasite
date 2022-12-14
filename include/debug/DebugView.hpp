/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_fixed.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "typedefs.hpp"

namespace mp
{
class TextGen;
}

namespace mp::debug
{

#ifdef MP_DEBUG

class DebugView
{
public:
    DebugView(TextGen& textGen);

    void update();

private:
    bool _isVisible() const;
    void _setVisible(bool isVisible);

    void _resetCounter();

private:
    TextGen& _textGen;
    bn::vector<bn::sprite_ptr, 2> _headingSprites;
    bn::vector<bn::sprite_ptr, 16> _usageSprites;

    s32 _updateCounter;
    bn::fixed _lastCpuSum;
    bn::fixed _lastVblankSum;
};

#endif

} // namespace mp::debug
