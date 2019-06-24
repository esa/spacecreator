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

#include <QString>
#include <QVariant>
#include <QVector>

namespace taste3 {

class AppOption
{
public:
    explicit AppOption(const QString &name, const QVariant &defaultValue = QVariant());
    const QString Name;
    const QVariant DefaultValue;

    QVariant read() const;
    void write(const QVariant &val) const;

private:
    AppOption() = delete;
    AppOption(const AppOption &) = delete;
    AppOption &operator=(const AppOption &) = delete;
};

class GroupMainWindow
{
public:
    GroupMainWindow();

    const AppOption Geometry;
    const AppOption State;
    const AppOption LastFilePath;
    const AppOption LastTab;

private:
    GroupMainWindow(const GroupMainWindow &) = delete;
    GroupMainWindow &operator=(const GroupMainWindow &) = delete;

    static const QString localName() { return "MainWindow"; }
};

class GroupBugReport
{
public:
    GroupBugReport();

    const AppOption Host;
    const AppOption ProjectID;
    const AppOption AccessToken;

private:
    GroupBugReport(const GroupBugReport &) = delete;
    GroupBugReport &operator=(const GroupBugReport &) = delete;

    static const QString localName() { return "GroupBugReport"; }
};

class AppOptions
{
public:
    static const GroupMainWindow MainWindow;
    static const GroupBugReport BugReport;

private:
    AppOptions() = delete;
    AppOptions(const AppOption &) = delete;
    AppOptions &operator=(const AppOptions &) = delete;
};

} // namespace taste3
