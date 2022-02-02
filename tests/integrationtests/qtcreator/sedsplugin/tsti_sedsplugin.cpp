/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "../../src/qtcreator/sedsplugin/sedsplugin.h"
#include "sedspluginwindow.h"

#include <QObject>
#include <QTest>
#include <QtTest/qtest.h>
#include <QtTest/qtestkeyboard.h>
#include <QtTest/qtestsystem.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <shared/sharedlibrary.h>

namespace sedsplugin {

class SedsPlugin : public QObject
{
    Q_OBJECT

private slots:
    void letMeFail();
};

void SedsPlugin::letMeFail()
{
    shared::initSharedLibrary();

    SedsPluginWindow window;
    window.showMaximized();
    window.activateWindow();

    for (const auto &action : window.actions()) {
        const auto actionMenu = action->menu();
        if (actionMenu != nullptr) {
            qDebug() << "action is not null";
            actionMenu->showMaximized();
        }
    }
    if (window.menuBar() != nullptr) {
        window.menuBar()->show();
    }

    QTest::keyClick(&window, Qt::Key_Alt);
    QTest::keyClick(&window, Qt::Key_T);
    QTest::keyClick(&window, Qt::Key_E);
    QTest::keyClick(&window, Qt::Key_Enter);

    qDebug() << qApp->applicationName();
    for (const auto &window : qApp->allWindows()) {
        if (window != nullptr) {
            qDebug() << "window is NOT null";
            qDebug() << window->title();
            window->showMaximized();
        } else {
            qDebug() << "window is null";
        }
    }

    QTest::qWait(3 * 1000);
}

} // namespace sedsplugin

QTEST_MAIN(sedsplugin::SedsPlugin)
#include "tsti_sedsplugin.moc"
