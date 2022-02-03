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

#include "sedspluginwindow.h"

#include <QObject>
#include <QTest>
#include <QtTest/qtest.h>
#include <QtTest/qtestkeyboard.h>
#include <QtTest/qtestsystem.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <sedsplugin.h>
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

    spctr::SedsPlugin plugin;
    plugin.importInterfaceView("seds_iv.xml");

    // check that ASN.1 files were imported
}

} // namespace sedsplugin

QTEST_MAIN(sedsplugin::SedsPlugin)
#include "tsti_sedsplugin.moc"
