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

#include "settingsmanager.h"

#include <QMainWindow>
#include <QtTest>

class tst_Settings : public QObject
{
    Q_OBJECT
public:
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testMainWindowGeometry();
    void testMainWindowState();

private:
    QMainWindow *mainwindow = nullptr;
};

void tst_Settings::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    mainwindow = new QMainWindow;
    mainwindow->setGeometry(100, 100, 640, 480);
}

void tst_Settings::cleanupTestCase()
{
    delete mainwindow;
    mainwindow = nullptr;
}

void tst_Settings::testMainWindowGeometry()
{
    const QRect geometry = mainwindow->geometry();
    shared::SettingsManager::store<QByteArray>(shared::SettingsManager::Common::Geometry, mainwindow->saveGeometry());
    mainwindow->setGeometry(QRect());
    mainwindow->restoreGeometry(shared::SettingsManager::load<QByteArray>(shared::SettingsManager::Common::Geometry));
    QCOMPARE(geometry, mainwindow->geometry());
}

void tst_Settings::testMainWindowState()
{
    const QByteArray state = mainwindow->saveState();
    shared::SettingsManager::store<QByteArray>(shared::SettingsManager::Common::State, state);
    const QByteArray bytes = shared::SettingsManager::load<QByteArray>(shared::SettingsManager::Common::State);
    QVERIFY(state == bytes);
}

QTEST_MAIN(tst_Settings)

#include "tst_settings.moc"
