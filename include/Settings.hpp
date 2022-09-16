/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#pragma once

#include "bn_forward_list.h"

#include "typedefs.hpp"

namespace mp
{

class SettingsObserver;

class Settings final
{
public:
    enum Language
    {
        ENGLISH = 0,
        KOREAN,

        // total langs count
        TOTAL_LANGUAGES
    };

public:
    Settings(Language);

    Language getLang() const;
    void setLang(Language);

public:
    void addObserver(SettingsObserver*);
    void removeObserver(SettingsObserver*);

private:
    Language _lang;

private:
    bn::forward_list<SettingsObserver*, 4> _observers;
};

class SettingsObserver
{
public:
    virtual ~SettingsObserver();

    SettingsObserver(Settings&);

public:
    virtual void onLangChange([[maybe_unused]] Settings::Language prevLang, Settings::Language newLang) = 0;

private:
    Settings& _settings;
};

} // namespace mp
