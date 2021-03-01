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

#include "colors/colorhandler.h"

#include <QJsonObject>
#include <QtTest>

class tst_ColorHandler : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSaveLoad();
};

void tst_ColorHandler::testSaveLoad()
{
    shared::ColorHandler ch;
    ch.setFillType(shared::ColorHandler::GradientVertical);
    ch.setPenWidth(2.5);
    ch.setPenColor(QColor(128, 128, 64));
    ch.setBrushColor0(QColor(255, 0, 64));
    ch.setBrushColor1(QColor(0, 255, 64));
    ch.setGroup("Nexus");

    QJsonObject exported = ch.toJson();
    shared::ColorHandler ch2 = shared::ColorHandler::fromJson(exported);

    QCOMPARE(ch.pen(), ch2.pen());
    QCOMPARE(ch.brush(), ch2.brush());
    QCOMPARE(ch.fillType(), ch2.fillType());
    QCOMPARE(ch.penWidth(), ch2.penWidth());
    QCOMPARE(ch.penColor(), ch2.penColor());
    QCOMPARE(ch.brushColor0(), ch2.brushColor0());
    QCOMPARE(ch.brushColor1(), ch2.brushColor1());
    QCOMPARE(ch.group(), ch2.group());
}

QTEST_MAIN(tst_ColorHandler)

#include "tst_colorhandler.moc"
