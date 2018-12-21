/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QString>
#include <QVariant>
#include <QVector>

class AppOption
{
public:
    AppOption(const QString &name, const QVariant &defaultValue = QVariant());
    const QString Name;
    const QVariant DefaultValue;

    QVariant read() const;
    void write(const QVariant &val) const;

private:
    AppOption() = delete;
    AppOption(const AppOption &) = delete;
    AppOption &operator=(const AppOption &) = delete;
};

class OptionsGroup
{
public:
    OptionsGroup(const QString &name, const QVector<AppOption *> &options,
                 const QVector<OptionsGroup *> &subroups);

    const QString Name;
    const QVector<AppOption *> Options;
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

    const AppOption *Geometry = Options[0];
    const AppOption *State = Options[1];

private:
    GroupMainWindow(const GroupMainWindow &) = delete;
    GroupMainWindow &operator=(const GroupMainWindow &) = delete;

    static const QString localName() { return "MainWindow"; }
};

class AppOptions
{
public:
    static const GroupMainWindow MainWindow;

private:
    AppOptions() = delete;
    AppOptions(const AppOption &) = delete;
    AppOptions &operator=(const AppOptions &) = delete;
};
