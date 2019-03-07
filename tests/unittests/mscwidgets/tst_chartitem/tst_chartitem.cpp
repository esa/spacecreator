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

#include "chartitem.h"
#include "mscchart.h"

#include <QScopedPointer>
#include <QtTest>

// add necessary includes here

class tst_ChartItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testChartName();
};

void tst_ChartItem::testChartName()
{
    static const QLatin1String name1("name1");
    static const QLatin1String name2("name2");

    using namespace msc;

    QScopedPointer<MscChart> chart(new MscChart());
    QScopedPointer<ChartItem> chartItem(new ChartItem(chart.data()));

    QCOMPARE(chart->name(), MscEntity::DefaultName);
    QCOMPARE(chartItem->chartName(), chart->name());
    QCOMPARE(chartItem->chartNameGuiText(), chart->name());

    chart->setName(name1);

    QCOMPARE(chartItem->chartName(), name1);
    QCOMPARE(chartItem->chartName(), chart->name());
    QCOMPARE(chartItem->chartNameGuiText(), chart->name());

    chartItem->setName(name2);

    QCOMPARE(chartItem->chartName(), name2);
    QCOMPARE(chartItem->chartName(), chart->name());
    QCOMPARE(chartItem->chartNameGuiText(), chart->name());
}

QTEST_MAIN(tst_ChartItem)

#include "tst_chartitem.moc"
