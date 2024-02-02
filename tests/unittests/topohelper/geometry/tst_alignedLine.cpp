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

#include <QPointF>
#include <QtTest>

using namespace topohelp;
using namespace topohelp::geom;

class TestAlignedLine : public QObject
{
    Q_OBJECT

private slots:

    void testAlignedLine()
    {
        constexpr QLineF srcLine(0, 0, 10, 0);

        auto performRotationTest = [&srcLine](qreal startAngle, int interval, qreal target) {
            QLineF line(srcLine);
            line.setAngle(startAngle);
            for (int a = 1; a <= interval; ++a) {
                QCOMPARE(alignedLine(line, a), false);
            }
            QCOMPARE(alignedLine(line, interval + 1), true);
            QCOMPARE(line.angle(), target);
        };

        performRotationTest(45, 45, 90);

        performRotationTest(80, 10, 90);
        performRotationTest(170, 10, 180);
        performRotationTest(260, 10, 270);
        performRotationTest(350, 10, 0);

        performRotationTest(0, 90, 90);
        performRotationTest(90, 90, 180);
        performRotationTest(180, 90, 270);
        performRotationTest(270, 90, 0);
    }
};

QTEST_MAIN(TestAlignedLine)
#include "tst_alignedLine.moc"
