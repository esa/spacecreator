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

#include <baseitems/common/arrowsign.h>

#include <QtTest>

using namespace msc;

class tst_ArrowSign : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testDefaults();
    void testPointTo();
    void testPath();
    void testCopy();

private:
    void testDefaultLeft(const ArrowSign &arrow) const;
    void testDefaultRight(const ArrowSign &arrow) const;
};

void tst_ArrowSign::testDefaults()
{
    const ArrowSign leftDefault;
    testDefaultLeft(leftDefault);

    const ArrowSign &left(ArrowSign::createArrowLeft());
    testDefaultLeft(left);

    const ArrowSign &right(ArrowSign::createArrowRight());
    testDefaultRight(right);
}

void tst_ArrowSign::testPointTo()
{
    const QPointF offset(123., 321.);

    auto testOffset = [](const ArrowSign &original, const ArrowSign &translated, const QPointF &offset) {
        QCOMPARE(translated.head() - original.head(), offset);
        QCOMPARE(translated.top() - original.top(), offset);
        QCOMPARE(translated.bottom() - original.bottom(), offset);
    };

    auto performTest = [&offset, &testOffset](bool left) {
        const ArrowSign &arrow(left ? ArrowSign::createArrowLeft() : ArrowSign::createArrowRight());
        ArrowSign arrowShifted(arrow);
        arrowShifted.pointTo(offset);

        testOffset(arrow, arrowShifted, offset);
    };

    performTest(true);
    performTest(false);
}

void tst_ArrowSign::testPath()
{
    auto performTest = [](const ArrowSign &arrow) {
        const QPainterPath &path(arrow.path());
        QCOMPARE(path.elementCount(), 4);
        QCOMPARE(QPointF(path.elementAt(0)), arrow.head());
        QCOMPARE(QPointF(path.elementAt(1)), arrow.top());
        QCOMPARE(QPointF(path.elementAt(2)), arrow.bottom());
        QCOMPARE(QPointF(path.elementAt(3)), arrow.head());
    };

    performTest(ArrowSign::createArrowLeft());
    performTest(ArrowSign::createArrowRight());
}

void tst_ArrowSign::testCopy()
{
    const QPointF offset(640., 480.);
    auto performTest = [&offset](bool left) {
        ArrowSign source(left ? ArrowSign::createArrowLeft() : ArrowSign::createArrowRight());
        source.pointTo(offset);
        const ArrowSign &copy(source);

        QCOMPARE(copy.head(), source.head());
        QCOMPARE(copy.top(), source.top());
        QCOMPARE(copy.bottom(), source.bottom());
    };

    performTest(true);
    performTest(false);
}

void tst_ArrowSign::testDefaultLeft(const ArrowSign &arrow) const
{
    QCOMPARE(arrow.head(), QPointF(0., 0.));
    QCOMPARE(arrow.top(), QPointF(ArrowSign::ARROW_WIDTH, -ArrowSign::ARROW_HEIGHT / 2.));
    QCOMPARE(arrow.bottom(), QPointF(ArrowSign::ARROW_WIDTH, -ArrowSign::ARROW_HEIGHT / 2. + ArrowSign::ARROW_HEIGHT));
}

void tst_ArrowSign::testDefaultRight(const ArrowSign &arrow) const
{
    QCOMPARE(arrow.head(), QPointF(0., 0.));
    QCOMPARE(arrow.top(), QPointF(-ArrowSign::ARROW_WIDTH, -ArrowSign::ARROW_HEIGHT / 2.));
    QCOMPARE(arrow.bottom(), QPointF(-ArrowSign::ARROW_WIDTH, -ArrowSign::ARROW_HEIGHT / 2. + ArrowSign::ARROW_HEIGHT));
}

QTEST_APPLESS_MAIN(tst_ArrowSign)

#include "tst_arrowsign.moc"
