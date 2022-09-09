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

#include "TextGen.hpp"
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

DebugView::DebugView(TextGen& textGen) : _textGen(textGen)
{
    _resetCounter();
}

void DebugView::update()
{
    if ((bn::keypad::start_held() && bn::keypad::select_pressed()) ||
        (bn::keypad::select_held() && bn::keypad::start_pressed()))
        _setVisible(!_isVisible());

    if (!_isVisible())
        return;

    _lastCpuSum += bn::core::last_cpu_usage();
    _lastVblankSum += bn::core::last_vblank_usage();

    if (--_updateCounter <= 0)
    {
        _usageSprites.clear();

        const s32 cpu = (_lastCpuSum / UPDATE_FRAMES * 100).round_integer();
        const s32 vblank = (_lastVblankSum / UPDATE_FRAMES * 100).round_integer();
        const s32 iwUse =
            (bn::fixed(bn::memory::used_static_iwram() + bn::memory::used_stack_iwram()) / IWRAM_BYTES * 100)
                .round_integer();
        const s32 ewUse =
            (bn::fixed(EWRAM_BYTES - bn::memory::available_alloc_ewram()) / EWRAM_BYTES * 100).round_integer();
        const s32 iwFree = IWRAM_BYTES - bn::memory::used_static_iwram() - bn::memory::used_stack_iwram();
        const s32 ewFree = bn::memory::available_alloc_ewram();

        auto& textGen = _textGen.get(TextGen::FontKind::GALMURI_9);
        textGen.set_alignment(bn::sprite_text_generator::alignment_type::LEFT);
        textGen.generate(X_POS, -60, bn::format<10>("cpu {}%", cpu), _usageSprites);
        textGen.generate(X_POS, -50, bn::format<10>("vbl {}%", vblank), _usageSprites);
        textGen.generate(X_POS, -40, bn::format<17>("  iw {}% {}", iwUse, iwFree), _usageSprites);
        textGen.generate(X_POS, -30, bn::format<18>("  ew {}% {}", ewUse, ewFree), _usageSprites);

        _resetCounter();
    }
}

bool DebugView::_isVisible() const
{
    return !_headingSprites.empty();
}

void DebugView::_setVisible(bool isVisible)
{
    if (isVisible)
    {
        auto& textGen = _textGen.get(TextGen::FontKind::GALMURI_9);
        textGen.set_alignment(bn::sprite_text_generator::alignment_type::LEFT);
        textGen.generate(X_POS, -70, "     use / free", _headingSprites);
    }
    else
    {
        _resetCounter();
        _usageSprites.clear();
        _headingSprites.clear();
    }
}

void DebugView::_resetCounter()
{
    _updateCounter = UPDATE_FRAMES;
    _lastCpuSum = 0;
    _lastVblankSum = 0;
}

#endif

} // namespace mp::debug
