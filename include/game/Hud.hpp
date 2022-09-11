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

#include "Settings.hpp"
#include "typedefs.hpp"

namespace mp
{
class TextGen;
}
namespace mp::game::item
{
struct ItemInfo;
}

namespace mp::game
{

class Hud;

class HudObserveSettings final : public SettingsObserver
{
public:
    HudObserveSettings(Hud&, Settings&);

    void onLangChange([[maybe_unused]] Settings::Language prevLang, Settings::Language newLang) override;

private:
    Hud& _hud;
};

class Hud final
{
public:
    Hud(TextGen&, Settings&);

    bool isVisible() const;
    void setVisible(bool);

    void setBelly(s32 currentBelly, s32 maxBelly);

    void clearInventory();
    void setInventory(const item::ItemInfo&);

private:
    void _initGraphics();
    void _setInventorySquareEmpty(bool isEmpty);

private:
    TextGen& _textGen;
    const Settings& _settings;

    s32 _currentBelly;
    s32 _maxBelly;
    s32 _bellyGaugeGraphicsIndex;

    bn::vector<bn::sprite_ptr, 1> _currentBellySprite;
    bn::vector<bn::sprite_ptr, 2> _bellyMiscSprites;
    bn::vector<bn::sprite_ptr, 1> _maxBellySprite;
    bn::sprite_ptr _bellyGaugeSprite;

    bn::sprite_ptr _inventorySquareSprite;
    bool _isInventoryEmpty = true;

    bn::vector<bn::sprite_ptr, 4> _itemUseHintText;
    bn::vector<bn::sprite_ptr, 4> _itemTossHintText;

private:
    friend class HudObserveSettings;
    HudObserveSettings _settingsObserver;
};

} // namespace mp::game
