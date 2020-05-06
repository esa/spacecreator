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

#include "ui/drawrectinfo.h"

#include <QtTest>

using namespace shared::ui;

class tst_DrawRectInfo : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testCtorDefault();
    void testCtorParam();
    void testRectSize();
    void testBorderWidth();
    void testBorderColor();
    void testBodyColor();
};

static const qreal w1 = 1.;
static const qreal w10 = 10.;
static const QSizeF sz100 = { 100., 100. };
static const QSizeF sz200 = { 200., 200. };

void tst_DrawRectInfo::testCtorDefault()
{
    const DrawRectInfo defaultInfo;
    QCOMPARE(defaultInfo, DrawRectInfo());
}

void tst_DrawRectInfo::testCtorParam()
{
    const DrawRectInfo info(sz100, w10, Qt::red, Qt::green);
    const DrawRectInfo infoSame(sz100, w10, Qt::red, Qt::green);
    QCOMPARE(infoSame, info);
}

void tst_DrawRectInfo::testRectSize()
{
    DrawRectInfo info(sz100, w10, Qt::red, Qt::green);
    QCOMPARE(info.rectSize(), sz100);

    info.setRectSize(sz200);
    QCOMPARE(info.rectSize(), sz200);
}

void tst_DrawRectInfo::testBorderWidth()
{
    DrawRectInfo info(sz100, w1, Qt::red, Qt::green);
    QCOMPARE(info.borderWidth(), w1);

    info.setBorderWidth(w10);
    QCOMPARE(info.borderWidth(), w10);
}

void tst_DrawRectInfo::testBorderColor()
{
    const QColor borderColorGreen(Qt::green);
    const QColor borderColorRed(Qt::red);
    DrawRectInfo info(sz100, w1, borderColorGreen, Qt::green);
    QCOMPARE(info.borderColor(), borderColorGreen);

    info.setBorderColor(borderColorRed);
    QCOMPARE(info.borderColor(), borderColorRed);
}

void tst_DrawRectInfo::testBodyColor()
{
    const QColor bodyColorGreen(Qt::green);
    const QColor bodyColorRed(Qt::red);
    DrawRectInfo info(sz100, w1, Qt::green, bodyColorGreen);
    QCOMPARE(info.bodyColor(), bodyColorGreen);

    info.setBodyColor(bodyColorRed);
    QCOMPARE(info.bodyColor(), bodyColorRed);
}

QTEST_APPLESS_MAIN(tst_DrawRectInfo)

#include "tst_drawrectinfo.moc"
