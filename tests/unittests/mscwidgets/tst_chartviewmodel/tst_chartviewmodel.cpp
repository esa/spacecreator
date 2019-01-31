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

#include "baseitems/common/utils.h"
#include "chartviewmodel.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscchart.h"
#include "mscfile.h"
#include "mscinstance.h"
#include "mscmodel.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtTest>

class tst_ChartViewModel : public QObject
{
    Q_OBJECT

public:
    tst_ChartViewModel();

private slots:
    void testNearestInstanceSimple();
};

using namespace msc;

tst_ChartViewModel::tst_ChartViewModel() {}

void tst_ChartViewModel::testNearestInstanceSimple()
{
    QString mscText = "MSC msc1; \
                          INSTANCE inst1; \
                              OUT Msg01 TO inst2; \
                          ENDINSTANCE; \
                          INSTANCE inst2; \
                              IN Msg01 FROM inst1; \
                          ENDINSTANCE; \
                       ENDMSC;";
    MscFile mscFile;
    QScopedPointer<MscModel> model(mscFile.parseText(mscText));

    MscChart *chart = model->charts().at(0);
    ChartViewModel chartModel;
    chartModel.fillView(chart);
    QApplication::processEvents();

    MscInstance *instance1 = chart->instances().at(0);
    InstanceItem *instance1Item = chartModel.itemForInstance(instance1);
    QVERIFY(instance1Item != nullptr);
    MscInstance *instance2 = chart->instances().at(1);
    InstanceItem *instance2Item = chartModel.itemForInstance(instance2);
    QVERIFY(instance2Item != nullptr);

    QRectF instance1Rect = instance1Item->sceneBoundingRect();
    QRectF instance2Rect = instance2Item->sceneBoundingRect();

    // point on the left
    MscInstance *inst = chartModel.nearestInstance({ instance1Rect.x() - 10., 50. });
    QCOMPARE(inst, instance1);
    // point on the left
    inst = chartModel.nearestInstance({ instance2Rect.x() + 10., 50. });
    QCOMPARE(inst, instance2);
    // point between itens, but closer to left
    qreal middle = instance1Rect.right() + 20.;
    inst = chartModel.nearestInstance({ middle, 50. });
    QCOMPARE(inst, instance1);

    // point too far on the left
    inst = chartModel.nearestInstance({ instance1Rect.x() - 100., 50. });
    QCOMPARE(inst, static_cast<MscInstance *>(nullptr));
    // point too far on the left
    inst = chartModel.nearestInstance({ instance2Rect.x() + 100., 50. });
    QCOMPARE(inst, static_cast<MscInstance *>(nullptr));
}

QTEST_MAIN(tst_ChartViewModel)

#include "tst_chartviewmodel.moc"
