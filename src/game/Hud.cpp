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
#include "bn_sprite_builder.h"
#include "bn_string.h"

#include "TextGen.hpp"
#include "constants.hpp"
#include "game/item/ItemInfo.hpp"
#include "game/mob/PlayerBelly.hpp"
#include "texts.hpp"

#include "bn_sprite_items_spr_belly_gauge.h"
#include "bn_sprite_items_spr_inventory_square.h"
#include "bn_sprite_palette_items_pal_font_gray.h"
#include "bn_sprite_palette_items_pal_font_white.h"

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
    145 - bn::display::width() / 2 + bn::sprite_items::spr_belly_gauge.shape_size().width() / 2,
    0 - bn::display::height() / 2 + bn::sprite_items::spr_belly_gauge.shape_size().height() / 2,
};

constexpr bn::fixed_point ITEM_HINT_TOP_POS = consts::INVENTORY_POS + bn::fixed_point{18, -4};
constexpr bn::fixed_point ITEM_HINT_BOTTOM_POS = consts::INVENTORY_POS + bn::fixed_point{18, 8};

constexpr s32 BELLY_FALLBACK_VALUE = 100;

} // namespace

HudObserveSettings::HudObserveSettings(Hud& hud, Settings& settings) : SettingsObserver(settings), _hud(hud)
{
}

void HudObserveSettings::onLangChange([[maybe_unused]] Settings::Language prevLang, Settings::Language newLang)
{
    // belly text update
    _hud._bellyMiscSprites.clear();

    auto& textGen = _hud._textGen.get(TextGen::FontKind::GALMURI_7);
    textGen.set_bg_priority(consts::UI_BG_PRIORITY);
    textGen.set_alignment(bn::sprite_text_generator::alignment_type::RIGHT);
    textGen.generate(BELLY_SLASH_POS, "/", _hud._bellyMiscSprites);
    textGen.generate(BELLY_TEXT_POS, texts::BELLY[newLang], _hud._bellyMiscSprites);
    for (auto& sprite : _hud._bellyMiscSprites)
        sprite.set_visible(_hud.isVisible());

    textGen.set_alignment(bn::sprite_text_generator::alignment_type::LEFT);

    // use & toss hint text update
    const auto usePalette = _hud._itemUseHintText[0].palette();
    const auto tossPalette = _hud._itemTossHintText[0].palette();
    _hud._itemUseHintText.clear();
    _hud._itemTossHintText.clear();
    textGen.generate(ITEM_HINT_TOP_POS, texts::ITEM_USE_HINT[newLang], _hud._itemUseHintText);
    textGen.generate(ITEM_HINT_BOTTOM_POS, texts::ITEM_TOSS_HINT[newLang], _hud._itemTossHintText);
    for (auto& sprite : _hud._itemUseHintText)
    {
        sprite.set_palette(usePalette);
        sprite.set_visible(_hud.isVisible());
    }
    for (auto& sprite : _hud._itemTossHintText)
    {
        sprite.set_palette(tossPalette);
        sprite.set_visible(_hud.isVisible());
    }
}

static s32 _getBellyGraphicsIndexFromBelly(s32 currentBelly, s32 maxBelly)
{
    return (bn::fixed(currentBelly) / maxBelly * 32).round_integer();
}

Hud::Hud(TextGen& textGen, Settings& settings)
    : _textGen(textGen), _settings(settings), _currentBelly(BELLY_FALLBACK_VALUE), _maxBelly(BELLY_FALLBACK_VALUE),
      _bellyGaugeGraphicsIndex(_getBellyGraphicsIndexFromBelly(_currentBelly, _maxBelly)),
      _bellyGaugeSprite(bn::sprite_items::spr_belly_gauge.create_sprite(BELLY_GAUGE_POS, _bellyGaugeGraphicsIndex)),
      _settingsObserver(*this, settings),
      _inventorySquareSprite(bn::sprite_items::spr_inventory_square.create_sprite(consts::INVENTORY_POS))
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
            textGen.set_bg_priority(consts::UI_BG_PRIORITY);
            textGen.set_alignment(bn::sprite_text_generator::alignment_type::RIGHT);
            textGen.generate(CURRENT_BELLY_POS, bn::to_string<4>(_currentBelly), _currentBellySprite);
            textGen.generate(MAX_BELLY_POS, bn::to_string<4>(_maxBelly), _maxBellySprite);

            if (_bellyMiscSprites.empty())
            {
                textGen.generate(BELLY_SLASH_POS, "/", _bellyMiscSprites);
                textGen.generate(BELLY_TEXT_POS, texts::BELLY[_settings.getLang()], _bellyMiscSprites);
            }
            else
            {
                for (auto& sprite : _bellyMiscSprites)
                    sprite.set_visible(newVisible);
            }

            textGen.set_alignment(bn::sprite_text_generator::alignment_type::LEFT);
            textGen.set_palette_item(bn::sprite_palette_items::pal_font_gray);

            if (_itemUseHintText.empty())
            {
                textGen.generate(ITEM_HINT_TOP_POS, texts::ITEM_USE_HINT[_settings.getLang()], _itemUseHintText);
                textGen.generate(ITEM_HINT_BOTTOM_POS, texts::ITEM_TOSS_HINT[_settings.getLang()], _itemTossHintText);
            }
            else
            {
                for (auto& sprite : _itemUseHintText)
                    sprite.set_visible(newVisible);
                for (auto& sprite : _itemTossHintText)
                    sprite.set_visible(newVisible);
            }

            textGen.set_palette_item(bn::sprite_palette_items::pal_font_white);
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
    _inventorySquareSprite.set_visible(newVisible);
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
            textGen.set_bg_priority(consts::UI_BG_PRIORITY);
            textGen.set_alignment(bn::sprite_text_generator::alignment_type::RIGHT);
            textGen.generate(CURRENT_BELLY_POS, bn::to_string<4>(currentBelly), _currentBellySprite);
        }
        if (maxBelly != _maxBelly)
        {
            _maxBellySprite.clear();

            auto& textGen = _textGen.get(TextGen::FontKind::GALMURI_7);
            textGen.set_bg_priority(consts::UI_BG_PRIORITY);
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
}

void Hud::clearInventory()
{
    _setInventorySquareEmpty(true);

    const auto& disabledPalette = bn::sprite_palette_items::pal_font_gray;
    for (auto& sprite : _itemUseHintText)
        sprite.set_palette(disabledPalette);
    for (auto& sprite : _itemTossHintText)
        sprite.set_palette(disabledPalette);
}

void Hud::setInventory(const item::ItemInfo& itemInfo)
{
    _setInventorySquareEmpty(false);

    const auto& enabledPalette = bn::sprite_palette_items::pal_font_white;
    const auto& useHintTextPalette =
        itemInfo.canBeUsed ? bn::sprite_palette_items::pal_font_white : bn::sprite_palette_items::pal_font_gray;
    for (auto& sprite : _itemUseHintText)
        sprite.set_palette(useHintTextPalette);
    for (auto& sprite : _itemTossHintText)
        sprite.set_palette(enabledPalette);
}

void Hud::_setInventorySquareEmpty(bool isEmpty)
{
    if (isEmpty != _isInventoryEmpty)
    {
        bn::sprite_builder builder(bn::sprite_items::spr_inventory_square, isEmpty ? 0 : 1);
        builder.set_visible(isVisible());
        builder.set_position(consts::INVENTORY_POS);
        builder.set_bg_priority(consts::UI_BG_PRIORITY);
        builder.set_z_order(consts::INVENTORY_Z_ORDER);
        _inventorySquareSprite = builder.build();
    }
    _isInventoryEmpty = isEmpty;
}

void Hud::_initGraphics()
{
    setVisible(false);
    clearInventory();
    _bellyGaugeSprite.set_bg_priority(consts::UI_BG_PRIORITY);
    _inventorySquareSprite.set_bg_priority(consts::UI_BG_PRIORITY);
    _inventorySquareSprite.set_z_order(consts::INVENTORY_Z_ORDER);
}

} // namespace mp::game
