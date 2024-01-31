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

class TestCoordinates_point : public QObject
{
    Q_OBJECT

private slots:
    void testCoordinates_point_data()
    {
        QSKIP("Not implemented yet");
    }

    void testCoordinates_point()
    {
        QSKIP("Not implemented yet");
        // const auto &actualResult = coordinates_point(replaceMe);
    }
};

QTEST_MAIN(TestCoordinates_point)
#include "tst_coordinates_point.moc"
