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

#include "ui/grippointshandler.h"

#include <QtTest>

using namespace shared::ui;

class tst_GripPointsHandler : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testConstructor();
    void testUsedPoints();
};

void tst_GripPointsHandler::testConstructor()
{
    GripPointsHandler handler;
    QCOMPARE(handler.gripPoints().size(), 0);
    QCOMPARE(handler.usedPoints().size(), 0);
}

void tst_GripPointsHandler::testUsedPoints()
{
    GripPointsHandler handler;

    const QMetaEnum &e = QMetaEnum::fromType<shared::ui::GripPoint::Location>();
    GripPoint::Locations locations = { GripPoint::Center, GripPoint::Left };
    handler.setUsedPoints(locations);
    QCOMPARE(handler.usedPoints(), locations);
    for (int i = 0; i < e.keyCount(); ++i) {
        auto location = static_cast<shared::ui::GripPoint::Location>(e.value(i));
        GripPoint *gp = handler.gripPoint(location);
        if (locations.contains(location)) {
            QVERIFY(gp != nullptr);
        } else {
            QVERIFY(gp == nullptr);
        }
    }
}

QTEST_MAIN(tst_GripPointsHandler)

#include "tst_grippointshandler.moc"
