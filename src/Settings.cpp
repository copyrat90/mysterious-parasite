/*
 * SPDX-FileCopyrightText: Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * See LICENSE file for details.
 */

#include "Settings.hpp"

#include "bn_log.h"

namespace mp
{

Settings::Settings(Language lang) : _lang(lang)
{
}

auto Settings::getLang() const -> Language
{
    return _lang;
}

void Settings::setLang(Language lang)
{
    if (lang == _lang)
        return;

    Language prevLang = _lang;
    _lang = lang;

    for (SettingsObserver* observer : _observers)
        observer->onLangChange(prevLang, lang);
}

void Settings::addObserver(SettingsObserver* observer)
{
    _observers.push_front(observer);
}

void Settings::removeObserver(SettingsObserver* observer)
{
    int removedObservers = 0;

    auto it = _observers.before_begin();
    auto cur = it;
    ++cur;
    while (cur != _observers.end())
    {
        if (*cur == observer)
            cur = _observers.erase_after(it), ++removedObservers;
        else
            ++it, ++cur;
    }

    if (removedObservers >= 2)
        BN_LOG("There were duplicated(", removedObservers, ") SettingsObservers. Removed all of them.");
}

SettingsObserver::~SettingsObserver()
{
    _settings.removeObserver(this);
}

SettingsObserver::SettingsObserver(Settings& settings) : _settings(settings)
{
    settings.addObserver(this);
}

} // namespace mp
