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

#include "baseitems/grippointshandler.h"

#include <QtTest>

using namespace msc;

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
    const QMetaEnum &e = QMetaEnum::fromType<msc::GripPoint::Location>();

    QCOMPARE(handler.gripPoints().size(), e.keyCount());
    QCOMPARE(handler.usedPoints().size(), e.keyCount());
}

void tst_GripPointsHandler::testUsedPoints()
{
    GripPointsHandler handler;

    const QMetaEnum &e = QMetaEnum::fromType<msc::GripPoint::Location>();
    for (int i = 0; i < e.keyCount(); ++i) {
        GripPoint *gp = handler.gripPoint(GripPoint::Location(e.value(i)));
        GripPoint::Locations points = handler.usedPoints();
        QVERIFY(points.contains(gp->location()));
        points.remove(gp->location());
        handler.setUsedPoints(points);
        QVERIFY(!handler.usedPoints().contains(gp->location()));
    }
}

QTEST_MAIN(tst_GripPointsHandler)

#include "tst_grippointshandler.moc"
