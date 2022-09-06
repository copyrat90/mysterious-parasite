/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "game/Hud.hpp"

#include "bn_assert.h"
#include "bn_display.h"
#include "bn_fixed_point.h"
#include "bn_log.h"
#include "bn_sprite_builder.h"
#include "bn_string.h"

#include "TextGen.hpp"
#include "constants.hpp"
#include "game/mob/PlayerBelly.hpp"

#include "bn_sprite_items_spr_belly_gauge.h"

namespace mp::game
{

namespace
{

constexpr bn::fixed_point CURRENT_BELLY_POS = {
    108 - bn::display::width() / 2,
    0 - bn::display::height() / 2 + 7,
};

constexpr bn::fixed_point MAX_BELLY_POS = {
    139 - bn::display::width() / 2,
    0 - bn::display::height() / 2 + 7,
};

constexpr bn::fixed_point BELLY_SLASH_POS = {
    CURRENT_BELLY_POS.x() + 8,
    CURRENT_BELLY_POS.y(),
};

constexpr bn::fixed_point BELLY_TEXT_POS = {
    81 - bn::display::width() / 2,
    0 - bn::display::height() / 2 + 7,
};

constexpr bn::fixed_point BELLY_GAUGE_POS = {
    141 - bn::display::width() / 2 + bn::sprite_items::spr_belly_gauge.shape_size().width() / 2,
    0 - bn::display::height() / 2 + bn::sprite_items::spr_belly_gauge.shape_size().height() / 2,
};

constexpr s32 BELLY_FALLBACK_VALUE = 100;

} // namespace

static s32 _getBellyGraphicsIndexFromBelly(s32 currentBelly, s32 maxBelly)
{
    return (bn::fixed(currentBelly) / maxBelly * 32).round_integer();
}

Hud::Hud(TextGen& textGen)
    : _textGen(textGen), _currentBelly(BELLY_FALLBACK_VALUE), _maxBelly(BELLY_FALLBACK_VALUE),
      _bellyGaugeGraphicsIndex(_getBellyGraphicsIndexFromBelly(_currentBelly, _maxBelly)),
      _bellyGaugeSprite(bn::sprite_items::spr_belly_gauge.create_sprite(BELLY_GAUGE_POS, _bellyGaugeGraphicsIndex))
{
    _initGraphics();
}

bool Hud::isVisible() const
{
    return _bellyGaugeSprite.visible();
}

void Hud::setVisible(bool newVisible)
{
    if (newVisible != isVisible())
    {
        if (newVisible)
        {
            auto& textGen = _textGen.get(TextGen::FontKind::GALMURI_7);
            textGen.set_alignment(bn::sprite_text_generator::alignment_type::RIGHT);
            textGen.generate(CURRENT_BELLY_POS, bn::to_string<4>(_currentBelly), _currentBellySprite);
            textGen.generate(MAX_BELLY_POS, bn::to_string<4>(_maxBelly), _maxBellySprite);

            if (_bellyMiscSprites.empty())
            {
                textGen.generate(BELLY_SLASH_POS, "/", _bellyMiscSprites);
                textGen.generate(BELLY_TEXT_POS, "Belly", _bellyMiscSprites);
            }
            else
            {
                for (auto& sprite : _bellyMiscSprites)
                    sprite.set_visible(newVisible);
            }
        }
        else
        {
            _currentBellySprite.clear();
            _maxBellySprite.clear();
            for (auto& sprite : _bellyMiscSprites)
                sprite.set_visible(newVisible);
        }
    }

    _bellyGaugeSprite.set_visible(newVisible);
}

void Hud::setBelly(s32 currentBelly, s32 maxBelly)
{
    BN_ASSERT(0 <= currentBelly && currentBelly <= maxBelly, "invalid currentBelly(", currentBelly, ") and maxBelly(",
              maxBelly, ")");

    const s32 newGraphicsIndex = _getBellyGraphicsIndexFromBelly(currentBelly, maxBelly);

    if (isVisible())
    {
        if (currentBelly != _currentBelly)
        {
            _currentBellySprite.clear();

            auto& textGen = _textGen.get(TextGen::FontKind::GALMURI_7);
            textGen.set_alignment(bn::sprite_text_generator::alignment_type::RIGHT);
            textGen.generate(CURRENT_BELLY_POS, bn::to_string<4>(currentBelly), _currentBellySprite);
        }
        if (maxBelly != _maxBelly)
        {
            _maxBellySprite.clear();

            auto& textGen = _textGen.get(TextGen::FontKind::GALMURI_7);
            textGen.set_alignment(bn::sprite_text_generator::alignment_type::RIGHT);
            textGen.generate(MAX_BELLY_POS, bn::to_string<4>(maxBelly), _maxBellySprite);
        }
    }

    // belly gauge should be updated even when not visible
    if (newGraphicsIndex != _bellyGaugeGraphicsIndex)
    {
        bn::sprite_builder builder(bn::sprite_items::spr_belly_gauge, newGraphicsIndex);
        builder.set_visible(isVisible());
        builder.set_position(BELLY_GAUGE_POS);
        _bellyGaugeSprite = builder.build();
    }

    _currentBelly = currentBelly;
    _maxBelly = maxBelly;
    _bellyGaugeGraphicsIndex = newGraphicsIndex;

    BN_LOG("currentBelly: ", _currentBelly);
}

void Hud::_initGraphics()
{
    setVisible(false);
}

} // namespace mp::game
