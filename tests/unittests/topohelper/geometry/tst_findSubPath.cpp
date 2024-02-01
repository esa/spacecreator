/*
   Copyright (C) 2024 European Space Agency - <maxime.perrotin@esa.int>

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

#include "topohelper/geometry.h"

#include <QtTest>

using namespace topohelp;
using namespace topohelp::geom;

class TestFindSubPath : public QObject
{
    Q_OBJECT

private slots:
    void testFindSubPath_data()
    {
        QTest::addColumn<QRectF>("itemRect");
        QTest::addColumn<PointsList>("prevPoints");
        QTest::addColumn<PointsList>("nextPoints");
        QTest::addColumn<bool>("strict");
        QTest::addColumn<QList<PointsList>>("expectedResult");

        QTest::newRow("NoTouchNoStrict") << QRectF(30, 30, 40, 40) << PointsList { QPointF(10, 10), QPointF(20, 20) }
                                         << PointsList { QPointF(80, 80), QPointF(90, 90) } << false
                                         << QList<PointsList> {
                                                { QPointF(10, 10), QPointF(20, 20), QPointF(20, 86), QPointF(86, 86) },
                                                { QPointF(10, 10), QPointF(20, 20), QPointF(86, 20), QPointF(86, 86) },
                                                { QPointF(10, 10), QPointF(20, 20), QPointF(20, 14), QPointF(86, 14) },
                                                { QPointF(10, 10), QPointF(20, 20), QPointF(86, 20), QPointF(86, 14) },
                                                { QPointF(10, 10), QPointF(20, 20), QPointF(20, 86), QPointF(14, 86) },
                                                { QPointF(10, 10), QPointF(20, 20), QPointF(14, 20), QPointF(14, 86) },
                                            };

        QTest::newRow("NoTouchStrict") << QRectF(30, 30, 40, 40) << PointsList { QPointF(10, 10), QPointF(20, 20) }
                                       << PointsList { QPointF(80, 80), QPointF(90, 90) } << true
                                       << QList<PointsList> {
                                              { QPointF(10, 10), QPointF(20, 20), QPointF(20, 86), QPointF(86, 86) },
                                              { QPointF(10, 10), QPointF(20, 20), QPointF(86, 20), QPointF(86, 86) },
                                              { QPointF(10, 10), QPointF(20, 20), QPointF(20, 14), QPointF(86, 14) },
                                              { QPointF(10, 10), QPointF(20, 20), QPointF(86, 20), QPointF(86, 14) },
                                              { QPointF(10, 10), QPointF(20, 20), QPointF(20, 86), QPointF(14, 86) },
                                              { QPointF(10, 10), QPointF(20, 20), QPointF(14, 20), QPointF(14, 86) },
                                          };

        QTest::newRow("TouchNoStrict") << QRectF(30, 30, 40, 40) << PointsList { QPointF(10, 10), QPointF(35, 35) }
                                       << PointsList { QPointF(65, 65), QPointF(90, 90) } << false
                                       << QList<PointsList> {
                                              { QPointF(10, 10), QPointF(35, 35), QPointF(35, 86), QPointF(86, 86) },
                                              { QPointF(10, 10), QPointF(35, 35), QPointF(86, 35), QPointF(86, 86) },
                                              { QPointF(10, 10), QPointF(35, 35), QPointF(35, 14), QPointF(86, 14) },
                                              { QPointF(10, 10), QPointF(35, 35), QPointF(86, 35), QPointF(86, 14) },
                                              { QPointF(10, 10), QPointF(35, 35), QPointF(35, 86), QPointF(14, 86) },
                                              { QPointF(10, 10), QPointF(35, 35), QPointF(14, 35), QPointF(14, 86) },
                                          };

        QTest::newRow("TouchStrict") << QRectF(30, 30, 40, 40) << PointsList { QPointF(10, 10), QPointF(35, 35) }
                                     << PointsList { QPointF(65, 65), QPointF(90, 90) } << true << QList<PointsList> {};
    }

    void testFindSubPath()
    {
        QFETCH(QRectF, itemRect);
        QFETCH(PointsList, prevPoints);
        QFETCH(PointsList, nextPoints);
        QFETCH(bool, strict);
        QFETCH(QList<PointsList>, expectedResult);

        const auto &actualResult = findSubPath(itemRect, prevPoints, nextPoints, strict);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestFindSubPath)
#include "tst_findSubPath.moc"
