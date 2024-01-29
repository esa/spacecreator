/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "topohelper/connection.h"

#include <QDebug>
#include <QPointF>
#include <QtTest>

class TestConnectionPath : public QObject
{
    Q_OBJECT

private slots:
    void testCreateConnectionPath_data()
    {
        using namespace topohelp::connection;
        using namespace topohelp;

        QTest::addColumn<ConnectionEnvInfo>("info");
        QTest::addColumn<PointsList>("expectedPath");

        const QRectF leftFrame(0, 0, 100, 100);
        const QPointF start(100, 50);
        const QRectF rightFrame(110, 0, 100, 100);
        const QPointF end(110, 50);
        const QPointF noPoint;

        QTest::newRow("ValidPath") << ConnectionEnvInfo { leftFrame, start, rightFrame, end }
                                   << PointsList { start, end };
        QTest::newRow("NoStart") << ConnectionEnvInfo { leftFrame, noPoint, rightFrame, end } << PointsList {};
        QTest::newRow("NoEnd") << ConnectionEnvInfo { leftFrame, start, rightFrame, noPoint } << PointsList {};
    }

    void testCreateConnectionPath()
    {
        using namespace topohelp::connection;
        using namespace topohelp;

        QFETCH(ConnectionEnvInfo, info);
        QFETCH(PointsList, expectedPath);

        const PointsList &actualPath = createConnectionPath(info);

        QCOMPARE(actualPath, expectedPath);
    }
};

QTEST_MAIN(TestConnectionPath)

#include "tst_createConnectionPath.moc"
