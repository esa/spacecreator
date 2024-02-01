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

class TestFindGeometryForRect : public QObject
{
    Q_OBJECT

private slots:
    void testFindGeometryForRect_data()
    {
        QTest::addColumn<QRectF>("itemRect");
        QTest::addColumn<QRectF>("boundedRect");
        QTest::addColumn<RectsList>("existingRects");
        QTest::addColumn<QMarginsF>("margins");
        QTest::addColumn<QRectF>("expectedItemRect");
        QTest::addColumn<QRectF>("expectedBoundedRect");

        constexpr QMarginsF margins(5, 5, 5, 5);

        QRectF rect(QPointF(0, 0), topohelp::kDefaultGraphicsItemSize);
        static const QRectF rectMin(rect.marginsRemoved(margins));
        static const QRectF top(rect.translated(rect.width(), 0));
        static const QRectF right(top.translated(rect.width(), rect.height()));
        static const QRectF bottom(right.translated(-rect.width(), rect.height()));
        static const QRectF left(right.translated(2 * -rect.width(), 0));
        rect.moveCenter({ top.center().x(), left.center().y() });

        constexpr qreal shift = 25.;

        {
            const QString caseName("WithoutMargins");
            const QRectF &itemRect = rect.adjusted(shift, shift, -shift, -shift);
            const QRectF &boundedRect = QRectF(top.bottomLeft(), bottom.topRight());

            const QRectF &expectedItemRect = QRectF(boundedRect.topLeft(), itemRect.size());
            const QRectF &expectedBoundedRect = QRectF({ 0, 0 }, rect.size() * 3);

            // qDebug() << caseName << "prepared:" << itemRect << boundedRect;
            QTest::newRow(caseName.toUtf8().data()) << itemRect << boundedRect << RectsList { top, right, bottom, left }
                                                    << QMarginsF() << expectedItemRect << expectedBoundedRect;
        }

        {
            const QString caseName("WithMargins");
            const QRectF &itemRect = rect.adjusted(shift, shift, -shift, -shift);
            const QRectF &boundedRect = QRectF(top.bottomLeft(), bottom.topRight());

            const QRectF &expectedItemRect =
                    QRectF(boundedRect.topLeft(), itemRect.size()).translated(2 * margins.left(), 2 * margins.top());
            const QRectF &expectedBoundedRect = QRectF({ 0, 0 }, rect.size() * 3).marginsAdded(margins);

            // qDebug() << caseName << "prepared:" << itemRect << boundedRect;
            QTest::newRow(caseName.toUtf8().data()) << itemRect << boundedRect << RectsList { top, right, bottom, left }
                                                    << margins << expectedItemRect << expectedBoundedRect;
        }

        {
            const QString caseName("NoMarginsNoItem");
            const QRectF &itemRect = QRectF();
            const QRectF &boundedRect = QRectF(top.bottomLeft(), bottom.topRight());

            const QRectF &expectedItemRect = QRectF(boundedRect.topLeft(), rect.size());
            const QRectF &expectedBoundedRect = QRectF({ 0, 0 }, rect.size() * 3);

            // qDebug() << caseName << "prepared:" << itemRect << boundedRect;
            QTest::newRow(caseName.toUtf8().data()) << itemRect << boundedRect << RectsList { top, right, bottom, left }
                                                    << QMarginsF() << expectedItemRect << expectedBoundedRect;
        }

        {
            const QString caseName("NoMarginsNoBounded0Neighbors");
            const QRectF &itemRect = rect.adjusted(shift, shift, -shift, -shift);
            const QRectF &boundedRect = QRectF();

            const QRectF &expectedItemRect = QRectF(itemRect);
            const QRectF &expectedBoundedRect = QRectF({ 0, 0 }, rect.size() * 3);

            // qDebug() << caseName << "prepared:" << itemRect << boundedRect;
            QTest::newRow(caseName.toUtf8().data()) << itemRect << boundedRect << RectsList { top, right, bottom, left }
                                                    << QMarginsF() << expectedItemRect << expectedBoundedRect;
        }

        // cases below are for research/comprehend purposes

        {
            const QString caseName("NoMarginsNoBounded1NeighborTop");
            const QRectF &itemRect = rect.adjusted(shift, shift, -shift, -shift);
            const QRectF &boundedRect = QRectF();

            const QRectF &expectedItemRect = QRectF(itemRect);
            const QRectF &expectedBoundedRect = QRectF(top.topLeft(), QSizeF(rect.width(), rect.height() * 2 - shift));

            // qDebug() << caseName << "prepared:" << itemRect << expectedBoundedRect;
            QTest::newRow(caseName.toUtf8().data()) << itemRect << boundedRect << RectsList { top } << QMarginsF()
                                                    << expectedItemRect << expectedBoundedRect;
        }
        {
            const QString caseName("NoMarginsNoBounded1NeighborRight");
            const QRectF &itemRect = rect.adjusted(shift, shift, -shift, -shift);
            const QRectF &boundedRect = QRectF();

            const QRectF &expectedItemRect = QRectF(itemRect);
            const QRectF &expectedBoundedRect = QRectF(QPointF(itemRect.topLeft().x(), top.bottom()),
                    QSizeF(rect.size().width() * 2 - shift, rect.size().height()));

            // qDebug() << caseName << "prepared:" << itemRect << boundedRect;
            QTest::newRow(caseName.toUtf8().data()) << itemRect << boundedRect << RectsList { right } << QMarginsF()
                                                    << expectedItemRect << expectedBoundedRect;
        }
        {
            const QString caseName("NoMarginsNoBounded1NeighborBottom");

            const QRectF &itemRect = rect.adjusted(shift, shift, -shift, -shift);
            const QRectF &boundedRect = QRectF();

            const QRectF &expectedItemRect = QRectF(itemRect);
            const QRectF &expectedBoundedRect = QRectF(QPointF(top.topLeft().x(), itemRect.topLeft().y()),
                    QSizeF(rect.size().width(), rect.size().height() * 2 - shift));

            // qDebug() << caseName << "prepared:" << itemRect << boundedRect;
            QTest::newRow(caseName.toUtf8().data()) << itemRect << boundedRect << RectsList { bottom } << QMarginsF()
                                                    << expectedItemRect << expectedBoundedRect;
        }
        {
            const QString caseName("NoMarginsNoBounded2Neighbors");
            const QRectF &itemRect = rect.adjusted(shift, shift, -shift, -shift);
            const QRectF &boundedRect = QRectF();

            const QRectF &expectedItemRect = QRectF(itemRect);
            const QRectF &expectedBoundedRect =
                    QRectF(top.topLeft(), QSizeF(rect.size().width() * 2, rect.size().height() * 2));

            // qDebug() << caseName << "prepared:" << itemRect << boundedRect;
            QTest::newRow(caseName.toUtf8().data()) << itemRect << boundedRect << RectsList { top, right }
                                                    << QMarginsF() << expectedItemRect << expectedBoundedRect;
        }

        {
            const QString caseName("NoMarginsNoBounded3Neighbors");
            const QRectF &itemRect = rect.adjusted(shift, shift, -shift, -shift);
            const QRectF &boundedRect = QRectF();

            const QRectF &expectedItemRect = QRectF(itemRect);
            const QRectF &expectedBoundedRect =
                    QRectF(top.topLeft(), QSizeF(rect.size().width() * 2, rect.size().height() * 3));

            // qDebug() << caseName << "prepared:" << itemRect << boundedRect;
            QTest::newRow(caseName.toUtf8().data()) << itemRect << boundedRect << RectsList { top, right, bottom }
                                                    << QMarginsF() << expectedItemRect << expectedBoundedRect;
        }
        {
            const QString caseName("NoMarginsNoBoundedNoNeighbors");
            const QRectF &itemRect = rect.adjusted(shift, shift, -shift, -shift);
            const QRectF &boundedRect = QRectF();

            const QRectF &expectedItemRect = QRectF(itemRect);
            const QRectF &expectedBoundedRect = QRectF(itemRect);

            // qDebug() << caseName << "prepared:" << itemRect << boundedRect;
            QTest::newRow(caseName.toUtf8().data()) << itemRect << boundedRect << RectsList {} << QMarginsF()
                                                    << expectedItemRect << expectedBoundedRect;
        }
    }

    void testFindGeometryForRect()
    {
        QFETCH(QRectF, itemRect);
        QFETCH(QRectF, boundedRect);
        QFETCH(RectsList, existingRects);
        QFETCH(QMarginsF, margins);
        QFETCH(QRectF, expectedItemRect);
        QFETCH(QRectF, expectedBoundedRect);

        // qDebug() << "received:" << itemRect << boundedRect;

        findGeometryForRect(itemRect, boundedRect, existingRects, margins);

        // qDebug() << "processed:" << itemRect << boundedRect;

        // qDebug() << "checking item";
        QCOMPARE(itemRect, expectedItemRect);
        // qDebug() << "checking bounded";
        QCOMPARE(boundedRect, expectedBoundedRect);
    }
};

QTEST_MAIN(TestFindGeometryForRect)
#include "tst_findGeometryForRect.moc"
