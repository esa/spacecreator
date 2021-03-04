/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "baseitems/common/coordinatesconverter.h"
#include "chartitem.h"
#include "sharedlibrary.h"

#include <QGraphicsScene>
#include <QObject>
#include <QtTest>

class tst_CoordinatesConverter : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testConversionCycle();

private:
};

void tst_CoordinatesConverter::testConversionCycle()
{
    shared::initSharedLibrary();
    QGraphicsView view;
    view.setGeometry(10, 10, 400, 400);
    QGraphicsScene scene;
    view.setScene(&scene);
    msc::ChartItem chartItem(nullptr, nullptr);
    msc::CoordinatesConverter::init(&scene, &chartItem);
    auto converter = msc::CoordinatesConverter::instance();
    converter->setDPI(QPointF(109., 109.), QPointF(96., 96.));

    // Point conversion
    QPointF originalPos(1070., 724.);
    QPoint cifPos;
    converter->sceneToCif(originalPos, cifPos);
    QPointF restoredScenePos;
    restoredScenePos = converter->cifToScene(cifPos);
    QCOMPARE(originalPos, restoredScenePos);

    // Point vector conversion
    bool ok;
    QVector<QPointF> originalPoints { { 0., 0. }, { 1070., 724. } };
    QVector<QPoint> cifPoints;
    cifPoints = converter->sceneToCif(originalPoints);
    QVector<QPointF> restoredScenePoints;
    restoredScenePoints = converter->cifToScene(cifPoints, &ok);
    QCOMPARE(originalPoints, restoredScenePoints);

    // Rectangle conversion
    QRectF originalSceneRect(0., 0., 1070., 724.);
    QRect cifRect;
    converter->sceneToCif(originalSceneRect, cifRect);
    QRectF restoredSceneRect;
    converter->cifToScene(cifRect, restoredSceneRect);
    QCOMPARE(originalSceneRect, restoredSceneRect);
}

QTEST_MAIN(tst_CoordinatesConverter)

#include "tst_coordinatesconverter.moc"
