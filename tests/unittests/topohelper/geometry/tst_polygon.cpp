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

class TestPolygon : public QObject
{
    Q_OBJECT

private slots:
    void testPolygon_data()
    {
        QTest::addColumn<QVector<qint32>>("coordinates");
        QTest::addColumn<PointsList>("expectedResult");

        auto fillPoints = [](int count) {
            PointsList points;
            constexpr int step = 10;

            int counter = 0;
            for (int i = 0; i < count; ++i) {
                points << QPointF(counter, counter + step);
                counter += 2 * step;
            }
            return points;
        };

        constexpr int MaxPointCount = 10;

        for (int i = 0; i < MaxPointCount; ++i) {
            const QString &validCaseName = QString("ValidPoints%1").arg(i);

            const PointsList &validPoints = fillPoints(i);
            QVector<qint32> validNumbers;

            for (const auto &pointF : validPoints) {
                const QPoint &point = pointF.toPoint();
                validNumbers << point.x() << point.y();
            }

            QTest::newRow(validCaseName.toUtf8().data()) << validNumbers << validPoints;

#ifdef QT_NO_DEBUG
            const QString &invalidCaseName = QString("InvalidPoints%1").arg(i);
            QVector<qint32> invalidNumbers(validNumbers);
            if (!invalidNumbers.isEmpty()) {
                invalidNumbers.takeLast();
            }

            QTest::newRow(invalidCaseName.toUtf8().data()) << invalidNumbers << PointsList();
#endif
        }
    }

    void testPolygon()
    {
        QFETCH(QVector<qint32>, coordinates);
        QFETCH(PointsList, expectedResult);

        const auto &actualResult = polygon(coordinates);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestPolygon)
#include "tst_polygon.moc"
