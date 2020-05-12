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

#include "settings/appoptions.h"
#include "settingsappoption.h"

#include <QAction>
#include <QCoreApplication>
#include <QDockWidget>
#include <QtTest>

class tst_Settings : public QObject
{
    Q_OBJECT
public:
private Q_SLOTS:
    void testMainWindowGeometry();
    void testMainWindowState();

private:
    static const QByteArray TestDataEmpty;
    static const QByteArray TestData1;
    static const QByteArray TestData2;

    void checkOption(shared::SettingsAppOption *option, const QVariant &data) const;
    void checkOptionByteArray(shared::SettingsAppOption *option) const;
};

const QByteArray tst_Settings::TestDataEmpty = { "" };
const QByteArray tst_Settings::TestData1 = { "data1" };
const QByteArray tst_Settings::TestData2 = { "data2" };

void tst_Settings::checkOption(shared::SettingsAppOption *option, const QVariant &data) const
{
    option->write(data);
    QCOMPARE(option->read(), data);
}

void tst_Settings::checkOptionByteArray(shared::SettingsAppOption *option) const
{
    checkOption(option, TestData1);
    checkOption(option, TestData2);
    checkOption(option, TestDataEmpty);
}

void tst_Settings::testMainWindowGeometry()
{
    // it stores result of QWidget::saveGeometry() - a QByteArray,
    // so the actual geometry does not matter here

    checkOptionByteArray(AppOptions::MainWindow.Geometry);
}

void tst_Settings::testMainWindowState()
{
    // it stores result of QMainWindow::saveState() - a QByteArray,
    // so the actual data does not matter here

    checkOptionByteArray(AppOptions::MainWindow.State);
}

QTEST_MAIN(tst_Settings)

#include "tst_settings.moc"
