/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "settingsappoption.h"

#include <QVector>

class OptionsGroup
{
public:
    OptionsGroup(const QString &name, const QVector<shared::SettingsAppOption *> &options, const QVector<OptionsGroup *> &subroups);

    const QString Name;
    const QVector<shared::SettingsAppOption *> Options;
    const QVector<OptionsGroup *> Groups;

private:
    OptionsGroup() = delete;
    OptionsGroup(const OptionsGroup &) = delete;
    OptionsGroup &operator=(const OptionsGroup &) = delete;
};

class GroupMainWindow : public OptionsGroup
{
public:
    GroupMainWindow();

    shared::SettingsAppOption *Geometry = Options[0];
    shared::SettingsAppOption *State = Options[1];
    shared::SettingsAppOption *LastFilePath = Options[2];
    shared::SettingsAppOption *DocOrHierarchyViewMode = Options[3];

private:
    GroupMainWindow(const GroupMainWindow &) = delete;
    GroupMainWindow &operator=(const GroupMainWindow &) = delete;

    static const QString localName() { return "MainWindow"; }
};

class AppOptions
{
public:
    static GroupMainWindow MainWindow;

private:
    AppOptions() = delete;
    AppOptions(const AppOptions &) = delete;
    AppOptions &operator=(const AppOptions &) = delete;
};
