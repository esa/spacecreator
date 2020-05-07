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

#include "ui/grippoint.h"

#include <QtTest>

using namespace shared::ui;

class tst_GripPoint : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testConstructor();
    void testSideSize();
    void testBorderWidth();
    void testBorderColor();
    void testBodyColor();
    void testGripType();
    void testIsUsed();
};

void tst_GripPoint::testConstructor()
{
    const QMetaEnum e = QMetaEnum::fromType<shared::ui::GripPoint::Location>();
    for (int i = 0; i < e.keyCount(); ++i) {
        GripPoint gp(GripPoint::Location(e.value(i)));
        QCOMPARE(gp.location(), GripPoint::Location(e.value(i)));
        QCOMPARE(gp.parentItem(), static_cast<QGraphicsItem *>(nullptr));
        QCOMPARE(gp.gripType(), GripPoint::GripType::Resizer);
    }
}

void tst_GripPoint::testSideSize()
{
    static constexpr qreal side = 123.;
    const QMetaEnum e = QMetaEnum::fromType<shared::ui::GripPoint::Location>();
    for (int i = 0; i < e.keyCount(); ++i) {
        GripPoint gp(GripPoint::Location(e.value(i)));
        gp.setSideSize(side + i);
        QCOMPARE(gp.sideSize(), side + i);
    }
}

void tst_GripPoint::testBorderWidth()
{
    static constexpr qreal border = 123.;
    const QMetaEnum e = QMetaEnum::fromType<shared::ui::GripPoint::Location>();
    for (int i = 0; i < e.keyCount(); ++i) {
        GripPoint gp(GripPoint::Location(e.value(i)));
        gp.setBorderWidth(border + i);
        QCOMPARE(gp.borderWidth(), border + i);
    }
}

void tst_GripPoint::testBorderColor()
{
    static const QColor color(Qt::red);
    const QMetaEnum e = QMetaEnum::fromType<shared::ui::GripPoint::Location>();
    for (int i = 0; i < e.keyCount(); ++i) {
        GripPoint gp(GripPoint::Location(e.value(i)));
        gp.setBorderColor(color);
        QCOMPARE(gp.borderColor(), color);
    }
}

void tst_GripPoint::testBodyColor()
{
    static const QColor color(Qt::red);
    const QMetaEnum e = QMetaEnum::fromType<shared::ui::GripPoint::Location>();
    for (int i = 0; i < e.keyCount(); ++i) {
        GripPoint gp(GripPoint::Location(e.value(i)));
        gp.setBodyColor(color);
        QCOMPARE(gp.bodyColor(), color);
    }
}

void tst_GripPoint::testGripType()
{
    const QMetaEnum e = QMetaEnum::fromType<shared::ui::GripPoint::Location>();
    for (int i = 0; i < e.keyCount(); ++i) {
        auto l = static_cast<GripPoint::Location>(e.value(i));
        const GripPoint gp(l, nullptr);
        if (l == GripPoint::Location::Center || l == GripPoint::Location::Absolute) {
            QCOMPARE(gp.gripType(), GripPoint::GripType::Mover);
        } else {
            QCOMPARE(gp.gripType(), GripPoint::GripType::Resizer);
        }
    }
}

void tst_GripPoint::testIsUsed()
{
    for (const bool used : { true, false }) {
        const QMetaEnum e = QMetaEnum::fromType<shared::ui::GripPoint::Location>();
        for (int i = 0; i < e.keyCount(); ++i) {
            GripPoint gp(GripPoint::Location(e.value(i)));
            gp.setIsUsed(used);
            QCOMPARE(gp.isUsed(), used);
        }
    }
}

QTEST_MAIN(tst_GripPoint)

#include "tst_grippoint.moc"
