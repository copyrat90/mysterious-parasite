/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "debug/DebugView.hpp"

#include "bn_core.h"
#include "bn_format.h"
#include "bn_keypad.h"
#include "bn_sprite_text_generator.h"

#include "texts.hpp"

namespace mp::debug
{

#ifdef MP_DEBUG

namespace
{
constexpr s32 UPDATE_FRAMES = 30;
constexpr s32 X_POS = -115;

constexpr s32 IWRAM_BYTES = 32'768, EWRAM_BYTES = 262'144;
} // namespace

DebugView::DebugView(bn::sprite_text_generator& textGen) : _textGen(textGen)
{
}

void DebugView::update()
{
    if ((bn::keypad::start_held() && bn::keypad::select_pressed()) ||
        (bn::keypad::select_held() && bn::keypad::start_pressed()))
        setVisible(!isVisible());

    if (!isVisible())
        return;

    if (--_updateCounter <= 0)
    {
        _updateCounter = UPDATE_FRAMES;
        _usageSprites.clear();

        const s32 cpu = (bn::core::last_cpu_usage() * 100).round_integer();
        const s32 vblank = (bn::core::last_vblank_usage() * 100).round_integer();
        const s32 iwUse =
            (bn::fixed(bn::memory::used_static_iwram() + bn::memory::used_stack_iwram()) / IWRAM_BYTES * 100)
                .round_integer();
        const s32 ewUse =
            (bn::fixed(EWRAM_BYTES - bn::memory::available_alloc_ewram()) / EWRAM_BYTES * 100).round_integer();
        const s32 iwFree = IWRAM_BYTES - bn::memory::used_static_iwram() - bn::memory::used_stack_iwram();
        const s32 ewFree = bn::memory::available_alloc_ewram();

        _textGen.set_alignment(bn::sprite_text_generator::alignment_type::LEFT);
        _textGen.generate(X_POS, -60, bn::format<10>("cpu {}%", cpu), _usageSprites);
        _textGen.generate(X_POS, -50, bn::format<10>("vbl {}%", vblank), _usageSprites);
        _textGen.generate(X_POS, -40, bn::format<17>("  iw {}% {}", iwUse, iwFree), _usageSprites);
        _textGen.generate(X_POS, -30, bn::format<18>("  ew {}% {}", ewUse, ewFree), _usageSprites);
    }
}

bool DebugView::isVisible() const
{
    return !_headingSprites.empty();
}

void DebugView::setVisible(bool isVisible)
{
    if (isVisible)
    {
        _textGen.set_alignment(bn::sprite_text_generator::alignment_type::LEFT);
        _textGen.generate(X_POS, -70, "     use / free", _headingSprites);
    }
    else
    {
        _updateCounter = 0;
        _usageSprites.clear();
        _headingSprites.clear();
    }
}

#endif

} // namespace mp::debug
