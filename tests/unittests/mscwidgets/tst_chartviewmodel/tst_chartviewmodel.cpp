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
#include <QScopedPointer>
#include <QVector>
#include <QtTest>

using namespace msc;

class tst_ChartViewModel : public QObject
{
    Q_OBJECT

public:
    tst_ChartViewModel();

private:
    void parseMsc(const QString &mscText);

private Q_SLOTS:
    void init();
    void cleanup();
    void testNearestInstanceSimple();
    void testNearestInstanceCreate();

private:
    QScopedPointer<ChartViewModel> m_chartModel;
    QScopedPointer<MscModel> m_mscModel;
    QPointer<MscChart> m_chart;
    QVector<MscInstance *> m_instances;
    QVector<InstanceItem *> m_instanceItems;
    QVector<QRectF> m_instanceRects;
};

tst_ChartViewModel::tst_ChartViewModel() {}

void tst_ChartViewModel::parseMsc(const QString &mscText)
{
    MscFile mscFile;
    m_mscModel.reset(mscFile.parseText(mscText));
    m_chart = m_mscModel->charts().at(0);
    m_chartModel->fillView(m_chart);
    QApplication::processEvents();

    for (MscInstance *instance : m_chart->instances()) {
        InstanceItem *instanceItem = m_chartModel->itemForInstance(instance);
        QVERIFY(instanceItem != nullptr);

        m_instances.append(instance);
        m_instanceItems.append(instanceItem);
        m_instanceRects.append(instanceItem->sceneBoundingRect());
    }
}

void tst_ChartViewModel::init()
{
    m_chartModel.reset(new ChartViewModel);
}

void tst_ChartViewModel::cleanup()
{
    m_instances.clear();
    m_instanceItems.clear();
    m_instanceRects.clear();
}

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
    parseMsc(mscText);

    // point on the left
    MscInstance *inst = m_chartModel->nearestInstance({ m_instanceRects[0].left() - 10., 50. });
    QCOMPARE(inst, m_instances[0]);
    // point on the left
    inst = m_chartModel->nearestInstance({ m_instanceRects[1].left() + 10., 50. });
    QCOMPARE(inst, m_instances[1]);
    // point between itens, but closer to left
    qreal middle = m_instanceRects[0].right() + 20.;
    inst = m_chartModel->nearestInstance({ middle, 50. });
    QCOMPARE(inst, m_instances[0]);

    // point too far on the left
    inst = m_chartModel->nearestInstance({ m_instanceRects[0].left() - 100., 50. });
    QCOMPARE(inst, static_cast<MscInstance *>(nullptr));
    // point too far on the left
    inst = m_chartModel->nearestInstance({ m_instanceRects[1].right() + 100., 50. });
    QCOMPARE(inst, static_cast<MscInstance *>(nullptr));
}

void tst_ChartViewModel::testNearestInstanceCreate()
{
    QString mscText = "MSC msc1; \
                          INSTANCE Instance_A; \
                              ACTION 'Boot'; \
                              CREATE New_Instance1; \
                              IN Msg01 FROM New_Instance1; \
                          ENDINSTANCE; \
                          INSTANCE New_Instance1; \
                              OUT Msg01 TO Instance_A; \
                          ENDINSTANCE; \
                       ENDMSC;";
    parseMsc(mscText);
    QCOMPARE(m_instanceItems.size(), 2);

    // point on the left
    MscInstance *inst = m_chartModel->nearestInstance(m_instanceRects[1].center());
    QCOMPARE(inst, m_instances[1]);

    // point above the create instance (but on height of first instance)
    inst = m_chartModel->nearestInstance({ m_instanceRects[1].center().x(), m_instanceRects[0].top() });
    QCOMPARE(inst, static_cast<MscInstance *>(nullptr));
}

QTEST_MAIN(tst_ChartViewModel)

#include "tst_chartviewmodel.moc"
